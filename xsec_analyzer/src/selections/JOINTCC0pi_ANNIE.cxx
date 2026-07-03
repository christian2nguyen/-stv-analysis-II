// XSecAnalyzer includes
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// XSecAnalyzer includes
#include "XSecAnalyzer/Selections/JOINTCC0pi_ANNIE.hh"
#include "XSecAnalyzer/Selections/EventCategoriesJOINTCC0pi.hh"
#include "XSecAnalyzer/FiducialVolume.hh"
#include "XSecAnalyzer/Functions.hh"
#include "XSecAnalyzer/TreeUtils.hh"
//#include "XSecAnalyzer/MCSTools.hh"

#include "XSecAnalyzer/PMTPositionInfo.hh"
#include <xgboost/c_api.h>
bool DeBugg = false; 


double Z_reco_AlineShift_ANNIE = -168.1; //cm
double Y_reco_AlineShift_ANNIE = 14.46; //cm
double LowerPCut = 600.; //MeV
double UpperPCut = 1200.;  //MeV

namespace {

const char* kANNIETankBDTModelPath =
    "/exp/uboone/app/users/cnguyen/stv-analysis-new/stv_BDT_ANNIE/bdt_cpp_outputs_5millsample_simple_v6/ANNIEPhaseIIBDT.json";
BoosterHandle* booster = nullptr;
bool g_annie_tank_bdt_loaded = false;
bool g_annie_tank_bdt_load_attempted = false;
PMTPositionInfo g_pmt_position_info(0.0);
bool g_pmt_position_loaded = false;
double g_tank_bdt_no_pion_score = -9999.0;
double g_tank_bdt_pion_score = -9999.0;

struct BDTSummaryStats {
  float count = 0.0f;
  float sum = 0.0f;
  float mean = 0.0f;
  float min = 0.0f;
  float max = 0.0f;
  float stddev = 0.0f;
};

struct TankBDTPMTFeatures {
  float q_asym_ud = 0.0f;
  float pe_asym_ud = 0.0f;
  float q_asym_tb = 0.0f;
  float pe_asym_tb = 0.0f;
  float q_r_mean = 0.0f;
  float pe_r_mean = 0.0f;
  float q_phi_mean = 0.0f;
  float pe_phi_mean = 0.0f;
  float q_phi_c1 = 0.0f;
  float q_phi_s1 = 0.0f;
  float q_phi_a1 = 0.0f;
  float q_phi_c2 = 0.0f;
  float q_phi_s2 = 0.0f;
  float q_phi_a2 = 0.0f;
  float pe_phi_c1 = 0.0f;
  float pe_phi_s1 = 0.0f;
  float pe_phi_a1 = 0.0f;
  float pe_phi_c2 = 0.0f;
  float pe_phi_s2 = 0.0f;
  float pe_phi_a2 = 0.0f;
  float n_unique_pmts_hit = 0.0f;
  float max_pe_fraction = 0.0f;
  float top5_pe_fraction = 0.0f;
  float pmt_pe_entropy = 0.0f;
  float pmt_pe_effective_n = 0.0f;
  float q_center_x = 0.0f;
  float q_center_y = 0.0f;
  float q_center_z = 0.0f;
  float q_rms_x = 0.0f;
  float q_rms_y = 0.0f;
  float q_rms_z = 0.0f;
  float pe_center_x = 0.0f;
  float pe_center_y = 0.0f;
  float pe_center_z = 0.0f;
  float pe_rms_x = 0.0f;
  float pe_rms_y = 0.0f;
  float pe_rms_z = 0.0f;
};

struct TankBDTPMTGeometry {
  int detid = -1;
  bool valid = false;
  bool is_upstream = false;
  bool is_top = false;
  bool is_bottom = false;
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;
  double r = 0.0;
  double phi = 0.0;
  double cos_phi = 0.0;
  double sin_phi = 0.0;
  double cos_2phi = 0.0;
  double sin_2phi = 0.0;
};

std::vector<TankBDTPMTGeometry> g_tank_bdt_pmt_geometry_cache;

bool IsFiniteValue(double value) {
  return std::isfinite(value);
}

bool XGBCallWithMessage(int code, const std::string& step) {
  if (code == 0) return true;

  std::cerr << "[ANNIE BDT] XGBoost failure during " << step;
  const char* last_error = XGBGetLastError();
  if (last_error) {
    std::cerr << " :: " << last_error;
  }
  std::cerr << std::endl;
  return false;
}

BDTSummaryStats SummarizeVector(
    const MyPointer<std::vector<double> >& values) {
  BDTSummaryStats stats;
  if (!values || values->empty()) {
    return stats;
  }

  double sum = 0.0;
  double sq_sum = 0.0;
  bool has_value = false;

  for (double value : *values) {
    if (!IsFiniteValue(value)) continue;

    if (!has_value) {
      stats.min = static_cast<float>(value);
      stats.max = static_cast<float>(value);
      has_value = true;
    } else {
      stats.min = std::min(stats.min, static_cast<float>(value));
      stats.max = std::max(stats.max, static_cast<float>(value));
    }

    sum += value;
    sq_sum += value * value;
    stats.count += 1.0f;
  }

  if (!has_value || stats.count <= 0.0f) {
    return BDTSummaryStats{};
  }

  const double mean = sum / stats.count;
  const double variance = std::max(0.0, (sq_sum / stats.count) - mean * mean);
  stats.sum = static_cast<float>(sum);
  stats.mean = static_cast<float>(mean);
  stats.stddev = static_cast<float>(std::sqrt(variance));
  return stats;
}

const TankBDTPMTGeometry* GetTankBDTPMTGeometry(int detid) {
  for (int i = 0; i < static_cast<int>(g_tank_bdt_pmt_geometry_cache.size()); ++i) {
    const TankBDTPMTGeometry& existing = g_tank_bdt_pmt_geometry_cache.at(i);
    if (existing.detid == detid) {
      return existing.valid ? &existing : nullptr;
    }
  }

  TankBDTPMTGeometry cached;
  cached.detid = detid;
  const PMTPositionRecord* pmt = g_pmt_position_info.GetByPMTId(detid);
  if (pmt) {
    cached.valid = true;
    cached.is_upstream = pmt->z_pos < g_pmt_position_info.GetBeamSplitZ();
    cached.is_top = g_pmt_position_info.PassPositionCut(detid, "top_only");
    cached.is_bottom = g_pmt_position_info.PassPositionCut(detid, "bottom_only");
    cached.x = pmt->x_pos;
    cached.y = pmt->y_pos;
    cached.z = pmt->z_pos;
    cached.r = std::sqrt(cached.x * cached.x + cached.z * cached.z);
    cached.phi = std::atan2(cached.x, cached.z);
    cached.cos_phi = std::cos(cached.phi);
    cached.sin_phi = std::sin(cached.phi);
    cached.cos_2phi = std::cos(2.0 * cached.phi);
    cached.sin_2phi = std::sin(2.0 * cached.phi);
  }

  g_tank_bdt_pmt_geometry_cache.push_back(cached);
  const TankBDTPMTGeometry& inserted = g_tank_bdt_pmt_geometry_cache.back();
  return inserted.valid ? &inserted : nullptr;
}

void AddPEToPMTSum(std::vector<std::pair<int, double> >& pe_by_pmt,
                   int detid,
                   double pe) {
  for (int i = 0; i < static_cast<int>(pe_by_pmt.size()); ++i) {
    if (pe_by_pmt.at(i).first == detid) {
      pe_by_pmt.at(i).second += pe;
      return;
    }
  }
  pe_by_pmt.push_back(std::make_pair(detid, pe));
}

std::pair<float, float> ComputeTimingGeometryCorrelations(
    const MyPointer<std::vector<int> >& hit_detid,
    const MyPointer<std::vector<double> >& hit_t,
    const MyPointer<std::vector<double> >& hit_pe) {
  if (!g_pmt_position_loaded || !hit_detid || !hit_t || !hit_pe) {
    return std::make_pair(0.0f, 0.0f);
  }

  const int n_hits = static_cast<int>(
      std::min(hit_detid->size(), std::min(hit_t->size(), hit_pe->size())));
  double weight_sum = 0.0;
  double t_sum = 0.0;
  double r_sum = 0.0;
  double z_sum = 0.0;
  double t2_sum = 0.0;
  double r2_sum = 0.0;
  double z2_sum = 0.0;
  double tr_sum = 0.0;
  double tz_sum = 0.0;

  for (int i = 0; i < n_hits; ++i) {
    const double t = hit_t->at(i);
    const double pe = hit_pe->at(i);
    if (!IsFiniteValue(t) || !IsFiniteValue(pe) || pe <= 0.0) {
      continue;
    }

    const TankBDTPMTGeometry* pmt = GetTankBDTPMTGeometry(hit_detid->at(i));
    if (!pmt) {
      continue;
    }

    weight_sum += pe;
    t_sum += pe * t;
    r_sum += pe * pmt->r;
    z_sum += pe * pmt->z;
    t2_sum += pe * t * t;
    r2_sum += pe * pmt->r * pmt->r;
    z2_sum += pe * pmt->z * pmt->z;
    tr_sum += pe * t * pmt->r;
    tz_sum += pe * t * pmt->z;
  }

  if (weight_sum <= 0.0) {
    return std::make_pair(0.0f, 0.0f);
  }

  const double mean_t = t_sum / weight_sum;
  const double mean_r = r_sum / weight_sum;
  const double mean_z = z_sum / weight_sum;
  const double var_t = std::max(0.0, t2_sum / weight_sum - mean_t * mean_t);
  const double var_r = std::max(0.0, r2_sum / weight_sum - mean_r * mean_r);
  const double var_z = std::max(0.0, z2_sum / weight_sum - mean_z * mean_z);
  const double cov_tr = tr_sum / weight_sum - mean_t * mean_r;
  const double cov_tz = tz_sum / weight_sum - mean_t * mean_z;

  float corr_r = 0.0f;
  float corr_z = 0.0f;
  if (var_t > 0.0 && var_r > 0.0) {
    corr_r = static_cast<float>(cov_tr / std::sqrt(var_t * var_r));
  }
  if (var_t > 0.0 && var_z > 0.0) {
    corr_z = static_cast<float>(cov_tz / std::sqrt(var_t * var_z));
  }
  return std::make_pair(corr_r, corr_z);
}

TankBDTPMTFeatures ComputeTankBDTPMTFeatures(
    const MyPointer<std::vector<int> >& hit_detid,
    const MyPointer<std::vector<double> >& hit_q,
    const MyPointer<std::vector<double> >& hit_pe) {
  TankBDTPMTFeatures out;
  if (!g_pmt_position_loaded || !hit_detid || !hit_q || !hit_pe) {
    return out;
  }

  const int n_hits = static_cast<int>(
      std::min(hit_detid->size(), std::min(hit_q->size(), hit_pe->size())));
  if (n_hits == 0) return out;

  double q_weight_sum = 0.0;
  double pe_weight_sum = 0.0;
  double qx_sum = 0.0;
  double qy_sum = 0.0;
  double qz_sum = 0.0;
  double qx2_sum = 0.0;
  double qy2_sum = 0.0;
  double qz2_sum = 0.0;
  double q_upstream_sum = 0.0;
  double q_downstream_sum = 0.0;
  double q_top_sum = 0.0;
  double q_bottom_sum = 0.0;
  double qr_sum = 0.0;
  double qcos_sum = 0.0;
  double qsin_sum = 0.0;
  double qcos2_sum = 0.0;
  double qsin2_sum = 0.0;

  double pex_sum = 0.0;
  double pey_sum = 0.0;
  double pez_sum = 0.0;
  double pex2_sum = 0.0;
  double pey2_sum = 0.0;
  double pez2_sum = 0.0;
  double pe_upstream_sum = 0.0;
  double pe_downstream_sum = 0.0;
  double pe_top_sum = 0.0;
  double pe_bottom_sum = 0.0;
  double per_sum = 0.0;
  double pecos_sum = 0.0;
  double pesin_sum = 0.0;
  double pecos2_sum = 0.0;
  double pesin2_sum = 0.0;
  std::vector<std::pair<int, double> > pe_by_pmt;
  pe_by_pmt.reserve(n_hits);

  for (int i = 0; i < n_hits; ++i) {
    const int detid = hit_detid->at(i);
    const TankBDTPMTGeometry* pmt = GetTankBDTPMTGeometry(detid);
    if (!pmt) continue;

    const double q = hit_q->at(i);
    const double pe = hit_pe->at(i);
    if (IsFiniteValue(q) && q > 0.0) {
      q_weight_sum += q;
      qx_sum += q * pmt->x;
      qy_sum += q * pmt->y;
      qz_sum += q * pmt->z;
      qx2_sum += q * pmt->x * pmt->x;
      qy2_sum += q * pmt->y * pmt->y;
      qz2_sum += q * pmt->z * pmt->z;
      if (pmt->is_upstream) q_upstream_sum += q;
      else q_downstream_sum += q;
      if (pmt->is_top) q_top_sum += q;
      else if (pmt->is_bottom) q_bottom_sum += q;
      qr_sum += q * pmt->r;
      qcos_sum += q * pmt->cos_phi;
      qsin_sum += q * pmt->sin_phi;
      qcos2_sum += q * pmt->cos_2phi;
      qsin2_sum += q * pmt->sin_2phi;
    }
    if (IsFiniteValue(pe) && pe > 0.0) {
      pe_weight_sum += pe;
      pex_sum += pe * pmt->x;
      pey_sum += pe * pmt->y;
      pez_sum += pe * pmt->z;
      pex2_sum += pe * pmt->x * pmt->x;
      pey2_sum += pe * pmt->y * pmt->y;
      pez2_sum += pe * pmt->z * pmt->z;
      if (pmt->is_upstream) pe_upstream_sum += pe;
      else pe_downstream_sum += pe;
      if (pmt->is_top) pe_top_sum += pe;
      else if (pmt->is_bottom) pe_bottom_sum += pe;
      per_sum += pe * pmt->r;
      pecos_sum += pe * pmt->cos_phi;
      pesin_sum += pe * pmt->sin_phi;
      pecos2_sum += pe * pmt->cos_2phi;
      pesin2_sum += pe * pmt->sin_2phi;
      AddPEToPMTSum(pe_by_pmt, detid, pe);
    }
  }

  if (q_weight_sum > 0.0) {
    out.q_center_x = static_cast<float>(qx_sum / q_weight_sum);
    out.q_center_y = static_cast<float>(qy_sum / q_weight_sum);
    out.q_center_z = static_cast<float>(qz_sum / q_weight_sum);
    out.q_r_mean = static_cast<float>(qr_sum / q_weight_sum);
    out.q_phi_c1 = static_cast<float>(qcos_sum / q_weight_sum);
    out.q_phi_s1 = static_cast<float>(qsin_sum / q_weight_sum);
    out.q_phi_a1 = static_cast<float>(
        std::sqrt(out.q_phi_c1 * out.q_phi_c1 +
                  out.q_phi_s1 * out.q_phi_s1));
    out.q_phi_mean = static_cast<float>(std::atan2(out.q_phi_s1, out.q_phi_c1));
    out.q_phi_c2 = static_cast<float>(qcos2_sum / q_weight_sum);
    out.q_phi_s2 = static_cast<float>(qsin2_sum / q_weight_sum);
    out.q_phi_a2 = static_cast<float>(
        std::sqrt(out.q_phi_c2 * out.q_phi_c2 +
                  out.q_phi_s2 * out.q_phi_s2));
    out.q_rms_x = static_cast<float>(std::sqrt(std::max(
        0.0, qx2_sum / q_weight_sum - out.q_center_x * out.q_center_x)));
    out.q_rms_y = static_cast<float>(std::sqrt(std::max(
        0.0, qy2_sum / q_weight_sum - out.q_center_y * out.q_center_y)));
    out.q_rms_z = static_cast<float>(std::sqrt(std::max(
        0.0, qz2_sum / q_weight_sum - out.q_center_z * out.q_center_z)));
  }

  if (pe_weight_sum > 0.0) {
    out.pe_center_x = static_cast<float>(pex_sum / pe_weight_sum);
    out.pe_center_y = static_cast<float>(pey_sum / pe_weight_sum);
    out.pe_center_z = static_cast<float>(pez_sum / pe_weight_sum);
    out.pe_r_mean = static_cast<float>(per_sum / pe_weight_sum);
    out.pe_phi_c1 = static_cast<float>(pecos_sum / pe_weight_sum);
    out.pe_phi_s1 = static_cast<float>(pesin_sum / pe_weight_sum);
    out.pe_phi_a1 = static_cast<float>(
        std::sqrt(out.pe_phi_c1 * out.pe_phi_c1 +
                  out.pe_phi_s1 * out.pe_phi_s1));
    out.pe_phi_mean =
        static_cast<float>(std::atan2(out.pe_phi_s1, out.pe_phi_c1));
    out.pe_phi_c2 = static_cast<float>(pecos2_sum / pe_weight_sum);
    out.pe_phi_s2 = static_cast<float>(pesin2_sum / pe_weight_sum);
    out.pe_phi_a2 = static_cast<float>(
        std::sqrt(out.pe_phi_c2 * out.pe_phi_c2 +
                  out.pe_phi_s2 * out.pe_phi_s2));
    out.pe_rms_x = static_cast<float>(std::sqrt(std::max(
        0.0, pex2_sum / pe_weight_sum - out.pe_center_x * out.pe_center_x)));
    out.pe_rms_y = static_cast<float>(std::sqrt(std::max(
        0.0, pey2_sum / pe_weight_sum - out.pe_center_y * out.pe_center_y)));
    out.pe_rms_z = static_cast<float>(std::sqrt(std::max(
        0.0, pez2_sum / pe_weight_sum - out.pe_center_z * out.pe_center_z)));

    std::vector<double> pmt_pe_sums;
    pmt_pe_sums.reserve(pe_by_pmt.size());
    for (int i = 0; i < static_cast<int>(pe_by_pmt.size()); ++i) {
      if (pe_by_pmt.at(i).second > 0.0) {
        pmt_pe_sums.push_back(pe_by_pmt.at(i).second);
      }
    }
    std::sort(pmt_pe_sums.begin(), pmt_pe_sums.end(),
              [](double a, double b) { return a > b; });
    out.n_unique_pmts_hit = static_cast<float>(pmt_pe_sums.size());
    if (!pmt_pe_sums.empty()) {
      out.max_pe_fraction =
          static_cast<float>(pmt_pe_sums.front() / pe_weight_sum);
      double top5_pe_sum = 0.0;
      double entropy = 0.0;
      double sum_fraction_sq = 0.0;
      const int n_top = std::min(5, static_cast<int>(pmt_pe_sums.size()));
      for (int i = 0; i < n_top; ++i) top5_pe_sum += pmt_pe_sums.at(i);
      for (double pmt_pe : pmt_pe_sums) {
        const double fraction = pmt_pe / pe_weight_sum;
        if (fraction <= 0.0) continue;
        entropy += -fraction * std::log(fraction);
        sum_fraction_sq += fraction * fraction;
      }
      out.top5_pe_fraction = static_cast<float>(top5_pe_sum / pe_weight_sum);
      out.pmt_pe_entropy = static_cast<float>(entropy);
      if (sum_fraction_sq > 0.0) {
        out.pmt_pe_effective_n = static_cast<float>(1.0 / sum_fraction_sq);
      }
    }
  }

  const double q_ud_total = q_downstream_sum + q_upstream_sum;
  if (q_ud_total > 0.0) {
    out.q_asym_ud =
        static_cast<float>((q_downstream_sum - q_upstream_sum) / q_ud_total);
  }
  const double pe_ud_total = pe_downstream_sum + pe_upstream_sum;
  if (pe_ud_total > 0.0) {
    out.pe_asym_ud =
        static_cast<float>((pe_downstream_sum - pe_upstream_sum) / pe_ud_total);
  }
  const double q_tb_total = q_top_sum + q_bottom_sum;
  if (q_tb_total > 0.0) {
    out.q_asym_tb = static_cast<float>((q_top_sum - q_bottom_sum) / q_tb_total);
  }
  const double pe_tb_total = pe_top_sum + pe_bottom_sum;
  if (pe_tb_total > 0.0) {
    out.pe_asym_tb =
        static_cast<float>((pe_top_sum - pe_bottom_sum) / pe_tb_total);
  }

  return out;
}

bool EnsureTankBDTLoaded() {
  if (g_annie_tank_bdt_loaded && booster && *booster) {
    return true;
  }
  std::cerr << "[ANNIE BDT] Booster was not loaded before prediction"
            << std::endl;
  return false;
}

bool BuildTankBDTFeatures(const AnalysisANNIE_Event* event,
                          std::vector<float>& features) {
  if (!event) return false;
  if (!event->hitT_ || !event->hitQ_ || !event->hitPE_ ||
      !event->hitDetID_) {
    return false;
  }

  features.clear();
  features.reserve(52);
  features.push_back(static_cast<float>(event->promptMuonTotalPE_));
  features.push_back(static_cast<float>(event->Qij_));

  const TankBDTPMTFeatures pmt = ComputeTankBDTPMTFeatures(
      event->hitDetID_, event->hitQ_, event->hitPE_);
  features.push_back(pmt.q_asym_ud);
  features.push_back(pmt.pe_asym_ud);
  features.push_back(pmt.q_asym_tb);
  features.push_back(pmt.pe_asym_tb);
  features.push_back(pmt.q_r_mean);
  features.push_back(pmt.pe_r_mean);
  features.push_back(pmt.q_phi_mean);
  features.push_back(pmt.pe_phi_mean);
  features.push_back(pmt.q_phi_c1);
  features.push_back(pmt.q_phi_s1);
  features.push_back(pmt.q_phi_a1);
  features.push_back(pmt.q_phi_c2);
  features.push_back(pmt.q_phi_s2);
  features.push_back(pmt.q_phi_a2);
  features.push_back(pmt.pe_phi_c1);
  features.push_back(pmt.pe_phi_s1);
  features.push_back(pmt.pe_phi_a1);
  features.push_back(pmt.pe_phi_c2);
  features.push_back(pmt.pe_phi_s2);
  features.push_back(pmt.pe_phi_a2);
  features.push_back(pmt.n_unique_pmts_hit);
  features.push_back(pmt.max_pe_fraction);
  features.push_back(pmt.top5_pe_fraction);
  features.push_back(pmt.pmt_pe_entropy);
  features.push_back(pmt.pmt_pe_effective_n);

  const BDTSummaryStats hit_t_stats = SummarizeVector(event->hitT_);
  const BDTSummaryStats hit_q_stats = SummarizeVector(event->hitQ_);
  const BDTSummaryStats hit_pe_stats = SummarizeVector(event->hitPE_);
  const std::pair<float, float> timing_corr =
      ComputeTimingGeometryCorrelations(event->hitDetID_, event->hitT_,
                                        event->hitPE_);

  features.push_back(pmt.q_center_x);
  features.push_back(pmt.q_center_y);
  features.push_back(pmt.q_center_z);
  features.push_back(pmt.q_rms_x);
  features.push_back(pmt.q_rms_y);
  features.push_back(pmt.q_rms_z);
  features.push_back(pmt.pe_center_x);
  features.push_back(pmt.pe_center_y);
  features.push_back(pmt.pe_center_z);
  features.push_back(pmt.pe_rms_x);
  features.push_back(pmt.pe_rms_y);
  features.push_back(pmt.pe_rms_z);
  features.push_back(hit_t_stats.mean);
  features.push_back(hit_t_stats.stddev);
  features.push_back(hit_q_stats.sum);
  features.push_back(hit_q_stats.mean);
  features.push_back(hit_q_stats.stddev);
  features.push_back(hit_pe_stats.sum);
  features.push_back(hit_pe_stats.mean);
  features.push_back(hit_pe_stats.stddev);
  features.push_back(hit_t_stats.min);
  features.push_back(hit_t_stats.max);
  features.push_back(hit_t_stats.max - hit_t_stats.min);
  features.push_back(timing_corr.first);
  features.push_back(timing_corr.second);

  return features.size() == 52;
}

bool EvaluateTankBDT(const AnalysisANNIE_Event* event,
                     float& no_pion_probability,
                     float& pion_probability) {
  no_pion_probability = -9999.f;
  pion_probability = -9999.f;

  if (!EnsureTankBDTLoaded()) {
    return false;
  }

  std::vector<float> features;
  if (!BuildTankBDTFeatures(event, features)) {
    std::cerr << "[ANNIE BDT] Missing event inputs for tank BDT evaluation"
              << std::endl;
    return false;
  }

  DMatrixHandle dmat = nullptr;
  if (!XGBCallWithMessage(
          XGDMatrixCreateFromMat(features.data(), 1,
                                 static_cast<unsigned long>(features.size()),
                                 -999.f, &dmat),
          "creating input matrix")) {
    return false;
  }

  const char* predict_config =
      "{\"type\":0,\"training\":false,\"iteration_begin\":0,"
      "\"iteration_end\":0,\"strict_shape\":false}";
  const unsigned long* out_shape = nullptr;
  unsigned long out_dim = 0;
  const float* out_result = nullptr;
  const bool ok = XGBCallWithMessage(
      XGBoosterPredictFromDMatrix(*booster, dmat, predict_config,
                                  &out_shape, &out_dim, &out_result),
      "running prediction");
  XGDMatrixFree(dmat);

  if (!ok || !out_result || out_dim < 1) {
    return false;
  }

  no_pion_probability = std::max(0.0f, std::min(1.0f, out_result[0]));
  pion_probability = 1.0f - no_pion_probability;
    if(DeBugg==true){
  std::cout << "[ANNIE BDT] Evaluated tank BDT: no_pion=" << no_pion_probability
            << ", pion=" << pion_probability << std::endl;}
  return true;
}

}  // namespace


JOINTCC0pi_ANNIE::JOINTCC0pi_ANNIE() : SelectionBase( "" ) {
  if (!g_annie_tank_bdt_load_attempted) {
    g_annie_tank_bdt_load_attempted = true;
    booster = new BoosterHandle;
    assert(XGBoosterCreate(NULL, 0, booster) == 0);
    assert(XGBoosterLoadModel(*booster, kANNIETankBDTModelPath) == 0);
    g_annie_tank_bdt_loaded = true;
    std::cout << "[ANNIE BDT] Loaded tank pion model from "
              << kANNIETankBDTModelPath << std::endl;
  }
  g_pmt_position_loaded = g_pmt_position_info.LoadFromFile("/exp/uboone/app/users/cnguyen/stv-analysis-II/xsec_analyzer/include/XSecAnalyzer/PMT_position_id_info.cvs");
  if (g_pmt_position_loaded) {
    g_tank_bdt_pmt_geometry_cache.reserve(g_pmt_position_info.Size());
    std::cout << "[ANNIE BDT] Loaded PMT geometry from PMT_position_id_info.cvs"
              << std::endl;
  } else {
    std::cout << "[ANNIE BDT] WARNING: PMT geometry was not loaded. "
              << "PMT-geometry BDT features will be zero." << std::endl;
  }

this->define_category_map();  
  //weight_All_UBGenie_inhouse_.reset(new std::vector<double>());
// this is for bg remove if not making a bg file 
 
}

void JOINTCC0pi_ANNIE::define_constants() {
  // Define reco and true fiducial volumes alongside any other constants used
  // within selection cuts
   std::cout<<"Finished ::DefineConstants "<< std::endl;
}

void JOINTCC0pi_ANNIE::compute_reco_observables( AnalysisEvent* event ) {
  std::cout<<"THis function shouldn't be used"<< std::endl;
  exit(1);
  // Calculate reconstructed kinematic variables to be saved in the output
}

void JOINTCC0pi_ANNIE::compute_true_observables( AnalysisEvent* event ) {
  std::cout<<"THis function shouldn't be used"<< std::endl;
  exit(1);
  // Calculate true kinematic variables to be saved in the output
}

int JOINTCC0pi_ANNIE::categorize_event( AnalysisEvent* event ) {
std::cout<<"Inside categorize_event for UbOONE , something is WRONG "<< std::endl;
std::cout<<"THis function shouldn't be used"<< std::endl;
exit(1);
  return 0;
}

bool JOINTCC0pi_ANNIE::define_signal(AnalysisEvent* event) {
std::cout<<"THis function shouldn't be used"<< std::endl;
exit(1);
  // Determine whether an input MC event fulfills the signal definition.
  // Only truth information should be used to determine the answer.
  return false;
}

bool JOINTCC0pi_ANNIE::selection( AnalysisEvent* event ) {
  std::cout<<"THis function shouldn't be used"<< std::endl;
  exit(1);
  // Determine whether an input event satisfies the selection criteria.
  // Only reco information should be used to determine the answer.
  return false;
}
/////////////////////////////////////////////////////////////


void JOINTCC0pi_ANNIE::define_output_branches() {
  // Call set_branch() for every new variable to be saved to the output TTree
  //set_branch( &mc_is_cc0pi_signal_, "mc_is_cc0pi_signal" );
   //set_branch_nolabel( &mc_is_cc0pi_signal_, "mc_is_cc0pi_signal" );
   set_branch_nolabel( &mc_is_cc0pi_signal_, "mc_is_cc0pi_signal" );
   set_branch_nolabel( &sel_CC0pi_wc_, "sel_CC0pi_wc" );
   set_branch( &category_, "category" );
   
   set_branch_nolabel( &mc_interaction_withO16_,"mc_interaction_withO16" );
   set_branch_nolabel( &mc_vertex_in_FV_,"mc_vertex_in_FV" );
   set_branch_nolabel( &mc_muon_in_mom_range_,"mc_muon_in_mom_range" );
   set_branch( &mc_neutrino_is_numu_,"mc_neutrino_is_numu" );
   set_branch_nolabel( &mc_no_charged_pi_above_threshold_,"mc_no_charged_pi_above_threshold" );
   set_branch_nolabel( &mc_muon_costheta_threshold_,"mc_muon_costheta_threshold" );
   set_branch_nolabel( &mc_no_fs_mesons_, "mc_no_fs_mesons" );
   set_branch_nolabel( &mc_no_fs_pi0_, "mc_no_fs_pi0" );
   set_branch_nolabel( &mc_pmu_gev_, "mc_pmu_gev" );
   
   
   
   
   
   set_branch_nolabel( &sel_muon_passed_mom_cuts_, "sel_muon_passed_mom_cuts");
   //set_branch_nolabel( &sel_has_pion_candidate_, "sel_has_pion_candidate");
   set_branch_nolabel( &sel_costheta_threshold_, "sel_costheta_threshold");
   set_branch_nolabel( &sel_contained_in_MRD_, "sel_contained_in_MRD");
   set_branch_nolabel( &sel_vertex_in_FV_, "sel_vertex_in_FV");
   set_branch_nolabel( &sel_has_pion_candidate_Qij_, "sel_has_pion_candidate_Qij");
   set_branch_nolabel( &sel_HasTank_, "sel_HasTank");
   
   
   set_branch_nolabel( &single_ring_score_, "single_ring_score");
   set_branch_nolabel( &multi_ring_score_, "multi_ring_score");
   set_branch_nolabel( &g_tank_bdt_no_pion_score, "tank_bdt_no_pion_score");
   set_branch_nolabel( &g_tank_bdt_pion_score, "tank_bdt_pion_score");

   set_branch_nolabel( &pmu_gev_, "pmu_gev" );
   set_branch_nolabel( &vertexRR_, "vertexRR" );
   set_branch_nolabel( &vertexR_, "vertexR" );
   set_branch_nolabel( &sel_nu_mu_cc_, "sel_nu_mu_cc" );
   set_branch_nolabel( &mc_num_protons_, "mc_num_protons" );
   set_branch_nolabel( &mc_num_neutrons_, "mc_num_neutrons" );
   set_branch_nolabel( &mc_num_charged_pions_, "mc_num_charged_pions" );
   set_branch_nolabel( &mc_num_neutral_pions_, "mc_num_neutral_pions" );
   set_branch_nolabel( &mc_num_wc_charged_pions_, "mc_num_wc_charged_pions" );
   set_branch_nolabel( &mc_num_kaons_, "mc_num_kaons" );
   set_branch_nolabel( &mc_num_wc_kaons_, "mc_num_wc_kaons" );  
   set_branch_nolabel( &mc_true_no_followers_, "mc_true_no_followers" );
   
   
   set_branch_nolabel( &sel_trigword_, "sel_trigword" );
     

   set_branch_nolabel( &sel_HasMRD_,"sel_HasMRD");
   set_branch_nolabel( &sel_TankMRDCoinc_,"sel_TankMRDCoinc");
   set_branch_nolabel( &sel_noVeto_,"sel_noVeto");
   set_branch_nolabel( &sel_simpleRecoFlag_,"sel_simpleRecoFlag");
   set_branch_nolabel( &sel_promptMuonTotalPE_,"sel_promptMuonTotalPE");
   
   
    set_branch_nolabel( &vertexRR_, "vertexRR" );
   set_branch_nolabel( &vertexR_, "vertexR" );
   
   set_branch_nolabel( &vertexX_, "vertexX" );
   set_branch_nolabel( &vertexY_, "vertexY" );
   set_branch_nolabel( &vertexZ_, "vertexZ" );
   

   
   set_branch_nolabel( &tracklenghtMRD_cm_,"tracklenghtMRD_cm");
   set_branch_nolabel( &lenghtvertextoStartMRD_cm_,"lenghtvertextoStartMRD_cm");  
   set_branch_nolabel( &sel_Atleast1track_MRDSTOP_,"sel_Atleast1track_MRDSTOP");
   
   //set_branch( weight_All_UBGenie_inhouse_, "weight_All_UBGenie_inhouse" );  

      
   
}

void JOINTCC0pi_ANNIE::reset() {
  // Set variables managed by this class to their default values. This function
  // is called in preparation for analyzing each new input event.
    mc_is_cc0pi_signal_ = false;
    sel_CC0pi_wc_ = false;
    
    mc_vertex_in_FV_= false;
    mc_muon_in_mom_range_= false;
    mc_neutrino_is_numu_ = false;
    mc_no_charged_pi_above_threshold_= false;
    mc_interaction_withO16_=false; 
    
    mc_muon_costheta_threshold_ = false;
    mc_no_fs_mesons_ = false; 
    mc_no_fs_pi0_ = false; 
    mc_true_no_followers_ = false; 
    category_  = 9999;
  
   sel_trigword_ = false; 
   sel_muon_passed_mom_cuts_ = false;
   sel_has_pion_candidate_ = false;
   sel_has_pion_candidate_Qij_ = false;
   sel_costheta_threshold_ = false;
   sel_contained_in_MRD_ = false;
   sel_vertex_in_FV_ = false;

   sel_nu_mu_cc_ = false; 
   sel_HasTank_ = false;
  
   sel_HasMRD_= false;
   sel_TankMRDCoinc_= false;
   sel_noVeto_= false;
   sel_simpleRecoFlag_= false;
   sel_promptMuonTotalPE_= false;
   sel_Atleast1track_MRDSTOP_ = false; 
  
  
   mc_num_protons_ = 0;
   mc_num_neutrons_ = 0;
   mc_num_charged_pions_ = 0;
   mc_num_neutral_pions_ = 0;
   mc_num_wc_charged_pions_ = 0;

   single_ring_score_ = -9999;
   multi_ring_score_ = -9999;
   g_tank_bdt_no_pion_score = -9999.0;
   g_tank_bdt_pion_score = -9999.0;
  
   mc_pmu_gev_ = -9999.;
   pmu_gev_ = -9999.;
      
   vertexRR_= -9999.;
   vertexR_= -9999.;
   vertexX_= -9999.;
   vertexY_= -9999.;
   vertexZ_= -9999.;
   tracklenghtMRD_cm_ = -9999.; 
   lenghtvertextoStartMRD_cm_ = -9999.; 
  //weight_All_UBGenie_inhouse_->clear();
    
}

void JOINTCC0pi_ANNIE::define_category_map() {
  // Define the mapping between each integer event category and
  // a string label / color integer code pair
  // The color codes are documented at
  // https://root.cern.ch/doc/master/classTColor.html
  // Keep this local selection independent of EventCategoriesJOINTCC0pi.hh.
  // That header pulls in category maps that can break ROOT dictionary builds
  // in this environment. Add manual labels here later if plotting needs them.
  std::cout<<"Finished :: DefineCategoryMap "<< std::endl;
}

void JOINTCC0pi_ANNIE::define_additional_input_branches(TTree& etree){
}

void JOINTCC0pi_ANNIE::compute_reco_observables( AnalysisANNIE_Event* event ) {

  // Calculate reconstructed kinematic variables to be saved in the output
return; 
}

void JOINTCC0pi_ANNIE::compute_true_observables( AnalysisANNIE_Event* event ) {

  if(mc_neutrino_is_numu_==false && event->trueCC_==1 && event->trueFSLPdg_ == 13 && event->trueTrackLengthInMRD_ > 0.){
      if(DeBugg)std::cout<<" ~~~~~~~~~~~~~~~~~~~"<< std::endl;
      
      if(DeBugg)std::cout<<"Checking::inside compute_true_observables"<< std::endl;
      if(DeBugg) std::cout << "mc_is_cc0pi_signal_: " << (mc_is_cc0pi_signal_ ? "true" : "false") << std::endl;
       if(DeBugg) std::cout << "trueCC: " << event->trueCC_ << std::endl;
       if(DeBugg) std::cout << "trueFSLPdg: " << event->trueFSLPdg_ << std::endl;
       if(DeBugg) std::cout << "trueTrackLengthInMRD: " << event->trueTrackLengthInMRD_ << std::endl;
       if(DeBugg) std::cout << "mc_neutrino_is_numu: " << mc_neutrino_is_numu_ << std::endl;
       
       
       if(DeBugg) std::cout << "trueFV: " << event->trueFV_ << std::endl;
       if(DeBugg) std::cout << "true_no_mesons: " << event->true_no_mesons_ << std::endl;
       if(DeBugg) std::cout << "trueCosTheta: " << event->trueCosTheta_ << std::endl;
       if(DeBugg) std::cout << "trueMuonMomentum: " << event->trueMuonMomentum_ << std::endl;
       if(DeBugg)std::cout << "first Entry of flux weights " << event->weight_flux_all_->at(0) << std::endl;
       if(DeBugg) std::cout << "category_: " << category_ << std::endl;
       if(DeBugg) std::cout<<"~~~~~~~~~~~~~~~~~~~~~~ "<< std::endl;
  }
       //Audit_result(  event );
       
        if(DeBugg==true){
          
          std::cout<<"audit Failed Case II "<< std::endl;
           std::cout << "Event details:" << std::endl;
           std::cout << "  trueCC: " << event->trueCC_ << std::endl;
           std::cout << "  trueFSLPdg: " << event->trueFSLPdg_ << std::endl;
           std::cout << "  trueTrackLengthInMRD: " << event->trueTrackLengthInMRD_ << std::endl;
           std::cout << "  trueFV: " << event->trueFV_ << std::endl;
           std::cout << "  true_no_mesons: " << event->true_no_mesons_ << std::endl;
           std::cout << "  trueCosTheta: " << event->trueCosTheta_ << std::endl;
           std::cout << "  trueMuonMomentum: " << event->trueMuonMomentum_ << std::endl;
           std::cout << "event->trueQEL = " << event->trueQEL_ << std::endl;
           std::cout << "event->trueRES_ = " << event->trueRES_ << std::endl;
           std::cout << "event->trueDIS_ = " << event->trueDIS_ << std::endl;
           std::cout << "event->trueCOH_ = " << event->trueCOH_ << std::endl;
           std::cout << "event->trueMEC_ = " << event->trueMEC_ << std::endl;
           std::cout<<"mc_neutrino_is_numu  ="<< (mc_neutrino_is_numu_ ? "true " : "false ")  << std::endl; 
           std::cout<<"mc_vertex_in_FV  ="<< (mc_vertex_in_FV_ ? "true " : "false ")  << std::endl; 
           std::cout<<"mc_interaction_withO16_ ="<< (mc_interaction_withO16_ ? "true " : "false ")  << std::endl; 
           std::cout<<"mc_no_fs_mesons  ="<< (mc_no_fs_mesons_ ? "true " : "false ")  << std::endl; 
           std::cout << "mc_is_cc0pi_signal_: " << (mc_is_cc0pi_signal_ ? "true" : "false") << std::endl;
        
          
          
          }

        
        
        
        
        //for(auto pdg_vector:event->truePrimaryPdgs){
        //std::cout<<"Loop : ev.truePrimaryPdgs"<<pdg_vector<< std::endl; }
 return; 
 
}

//////////////////////////////////
// New methods
///////////////////////////////

int JOINTCC0pi_ANNIE::categorize_event( AnalysisANNIE_Event* event ) {
 
  // Real data has a bogus true neutrino PDG code that is not one of the
  // allowed values (±12, ±14, ±16)
 //bool fTruemc_is_cc0pi_signal;
 // 0 is false 
 // 1 true
 //if(DeBugg)std::cout<<"inside::categorize_event"<< std::endl;
  
  //if ( false ) {
  //category_  = static_cast<int>(kUnknown);
  //return kUnknown;
  //}

  //bool fTruemc_vertex_in_FV = event->trueFV_;
  //int fTruemc_neutrino_is_numu = (event->trueFSLPdg_ == 13 )?1:0;
  
  //bool fTruemc_vertex_in_FV = mc_vertex_in_FV_;
  //bool fTruemc_neutrino_is_numu = mc_neutrino_is_numu_;
  
  
  //if(DeBugg)std::cout<<"event->trueFSLPdg = " << event->trueFSLPdg_ << std::endl;
  //if(DeBugg)std::cout<<"event->truePrimaryPdg = " << event->truePrimaryPdg_ << std::endl;
  //if(DeBugg)std::cout<<"event->trueFV_ = " << event->trueFV_ << std::endl;

  if ( !mc_vertex_in_FV_ ) {
    category_  = static_cast<int>(kOOFV);
    return kOOFV;
  }
  
  else if ( event->trueNC_==1 ) {
  category_  = static_cast<int>(kNC);
    return  kNC; 
  }
  
  
  
  else if ( !(event->trueCC_==1 && /*event->trueNuPDG_ == 14*/ event->trueFSLPdg_ == 13 /*|| mc_interaction_withO16_==false*/ )){ 
  
    if ( event->trueFSLPdg_ == 11 /*event->trueNuPDG_ == 12 */&& event->trueCC_==1 ) {    
             category_  = static_cast<int>(kNuECC);
             return kNuECC;
      }
      
      
    /*
       else if (event->trueCC_==1 && event->trueNuPDG_ == -14){
    category_  = static_cast<int>(kNuMuBarCC);
    return kNuMuBarCC;  
      }
    */

  
    else { 
          category_  = static_cast<int>(kOther);
           return kOther;
         }
  }

  // Should have already set defaults
  //mc_muon_in_mom_range_ = false;
  //mc_lead_p_in_mom_range_ = false;
  //mc_no_fs_pi0_ = true;
  //mc_no_charged_pi_above_threshold_ = true;
  //mc_no_fs_mesons_ = true;

  //double lead_p_mom = LOW_FLOAT;
  //bool fTruemc_no_fs_mesons= !event->true_no_mesons_;
  //bool fTruemc_no_charged_pi_above_threshold = mc_no_charged_pi_above_threshold_;
  
  //fTruemc_is_cc0pi_signal = (fTruemc_vertex_in_FV==1 && fTruemc_neutrino_is_numu==1 && 
    // fTruemc_no_fs_mesons ==1  && fTruemc_no_charged_pi_above_threshold ==1)?1:0;

  // Sort signal by interaction mode
  else if ( mc_is_cc0pi_signal_ ) {
  
   if ( event->trueQEL_==1 && event->trueRES_== 0 && event->trueDIS_==0 && event->trueCOH_==0 && event->trueMEC_==0) {      category_  = static_cast<int>(kSignalCCQE);return kSignalCCQE;  }// QE}
    else if ( event->trueQEL_==0 && event->trueRES_== 0 && event->trueDIS_==0 && event->trueCOH_==0 && event->trueMEC_==1 ) {category_  = static_cast<int>(kSignalCCMEC);return kSignalCCMEC; } // MEC}
    else if ( event->trueQEL_==0 && event->trueRES_== 1 && event->trueDIS_==0 && event->trueCOH_==0 && event->trueMEC_==0 ) {category_  = static_cast<int>(kSignalCCRES);return kSignalCCRES; } // RES}
    else{  category_  = static_cast<int>(kSignalOther);
    return kSignalOther;}
 }
  
       //else if ( mc_nu_interaction_type_ == 2 ) // DIS
    //else if ( mc_nu_interaction_type_ == 3 ) // COH
  
  else if ( mc_no_fs_mesons_==false || mc_no_charged_pi_above_threshold_==false || mc_no_fs_pi0_==false) {
             category_  = static_cast<int>(kNuMuCCNpi);
            return kNuMuCCNpi; }
 else if(mc_muon_in_mom_range_==false || 
        mc_muon_costheta_threshold_==false){
        category_  = static_cast<int>(kNuMuCCOther);
        return kNuMuCCOther;
        
        }
  else if (mc_true_no_followers_ == false){
          category_  = static_cast<int>(kOther);
           return kOther;
 }
  else {
  category_  = static_cast<int>(kUnknown);
  return kUnknown;}
       
}




bool JOINTCC0pi_ANNIE::define_signal(AnalysisANNIE_Event* event) {
  // Determine whether an input MC event fulfills the signal definition.
  // Only truth information should be used to determine the answer.
  //if(DeBugg)std::cout<< "inside:define_signal "<< std::endl;
  
  	//weight_All_UBGenie_inhouse_->insert(weight_All_UBGenie_inhouse_->end(), event->weight_All1_UBGenie_->begin(), event->weight_All1_UBGenie_->end());
		//weight_All_UBGenie_inhouse_->insert(weight_All_UBGenie_inhouse_->end(), event->weight_All2_UBGenie_->begin(), event->weight_All2_UBGenie_->end());
		//weight_All_UBGenie_inhouse_->insert(weight_All_UBGenie_inhouse_->end(), event->weight_All3_UBGenie_->begin(), event->weight_All3_UBGenie_->end());
		//weight_All_UBGenie_inhouse_->insert(weight_All_UBGenie_inhouse_->end(), event->weight_All4_UBGenie_->begin(), event->weight_All4_UBGenie_->end());
		//weight_All_UBGenie_inhouse_->insert(weight_All_UBGenie_inhouse_->end(), event->weight_All5_UBGenie_->begin(), event->weight_All5_UBGenie_->end());

    
    
    
  //mc_neutrino_is_numu_ = event->trueCC_==1 && event->trueNuPDG_ == 14; // changing this for 40 k sample 
  
  mc_neutrino_is_numu_ = event->trueCC_==1 && event->trueFSLPdg_ == 13;
  
  mc_vertex_in_FV_  = event->trueFV_;
  //mc_interaction_withO16_ = event->trueTargetZ_==8;
  mc_muon_in_mom_range_ = event->trueMuonMomentum_ >=  LowerPCut && event->trueMuonMomentum_ < UpperPCut ;
  mc_muon_costheta_threshold_= event->trueCosTheta_ > 0.8;
  
  mc_num_protons_ = event->trueProtons_;
  mc_num_neutrons_ = event->trueNeutrons_;
  mc_num_charged_pions_ = event->truePiPlus_ + event->truePiMinus_;
  mc_num_neutral_pions_ = event->truePi0_;
  mc_num_wc_charged_pions_ = event->truePiPlusCher_ + event->truePiMinusCher_;
  mc_num_kaons_ = event->trueKPlus_ + event->trueKMinus_; 
  mc_num_wc_kaons_ = event->trueKPlusCher_ + event->trueKMinusCher_;
  mc_no_charged_pi_above_threshold_ = mc_num_wc_charged_pions_== 0;
  mc_no_fs_pi0_ = mc_num_neutral_pions_==0;
  mc_no_fs_mesons_ =  mc_no_charged_pi_above_threshold_ &&  (mc_num_wc_kaons_==0) && mc_no_fs_pi0_;
  mc_true_no_followers_ = event->true_no_followers_;
  mc_pmu_gev_ = event->trueMuonMomentum_ * .001; // MeV -> GeV
  
  mc_is_cc0pi_signal_ =  mc_neutrino_is_numu_ &&
                        // mc_interaction_withO16_ && 
                         mc_vertex_in_FV_ &&
                         mc_no_fs_mesons_  && 
                         mc_true_no_followers_ &&
                         mc_muon_in_mom_range_ && 
                         mc_muon_costheta_threshold_;
                  
         
  // Print the result
  
   //if(mc_neutrino_is_numu_==false && event->trueCC_==1 && event->trueFSLPdg_ == 13 && event->trueTrackLengthInMRD_ > 0.){
   
   if(DeBugg) std::cout<<" "<< std::endl; 

    
   if(DeBugg) std::cout<<"mc_neutrino_is_numu  ="<< (mc_neutrino_is_numu_ ? "true " : "false ")  << std::endl; 
   if(DeBugg) std::cout<<"mc_vertex_in_FV  ="<< (mc_vertex_in_FV_ ? "true " : "false ")  << std::endl; 
   if(DeBugg) std::cout<<"mc_no_fs_mesons  ="<< (mc_no_fs_mesons_ ? "true " : "false ")  << std::endl; 
   if(DeBugg) std::cout << "mc_is_cc0pi_signal_: " << (mc_is_cc0pi_signal_ ? "true" : "false") << std::endl;
    // Optionally, print the values of all involved variables for debugging
   
    
   if(DeBugg) std::cout << "Event details:" << std::endl;
   if(DeBugg) std::cout << "  trueCC: " << event->trueCC_ << std::endl;
   if(DeBugg) std::cout << "  trueFSLPdg: " << event->trueFSLPdg_ << std::endl;
   if(DeBugg) std::cout << "  trueTrackLengthInMRD: " << event->trueTrackLengthInMRD_ << std::endl;
   if(DeBugg) std::cout << "  trueFV: " << event->trueFV_ << std::endl;
   if(DeBugg) std::cout << "  true_no_mesons: " << event->true_no_mesons_ << std::endl;
   if(DeBugg) std::cout << "  trueCosTheta: " << event->trueCosTheta_ << std::endl;
   if(DeBugg) std::cout << "  trueMuonMomentum: " << event->trueMuonMomentum_ << std::endl;
   if(DeBugg) std::cout << "event->trueQEL = " << event->trueQEL_ << std::endl;
   if(DeBugg) std::cout << "event->trueRES_ = " << event->trueRES_ << std::endl;
   if(DeBugg) std::cout << "event->trueDIS_ = " << event->trueDIS_ << std::endl;
   if(DeBugg) std::cout << "event->trueCOH_ = " << event->trueCOH_ << std::endl;
   if(DeBugg) std::cout << "event->trueMEC_ = " << event->trueMEC_ << std::endl;
   
   if(DeBugg) std::cout<<" "<< std::endl;
    
   //}
    
    
    
  
  return mc_is_cc0pi_signal_;
}




bool JOINTCC0pi_ANNIE::selection( AnalysisANNIE_Event* event ) {
  // Determine whether an input event satisfies the selection criteria.
  // Only reco information should be used to determine the answer.
  //if(DeBugg)std::cout<<" "<< std::endl;
  //if(DeBugg)std::cout<< "Inside::Selection"<<std::endl;
   //bool fallback = false; 

    float no_pion_probability = -9999.f;
    float pion_probability = -9999.f;
    if (EvaluateTankBDT(event, no_pion_probability, pion_probability)) {
      g_tank_bdt_no_pion_score = no_pion_probability;
      g_tank_bdt_pion_score = pion_probability;
    }
  
    //reco_contained_in_MRD_ = event->numMRDTracks_ == 1 ?  first_or(event->MRDStop_, fallback) : false;
 
    sel_muon_passed_mom_cuts_ = event->simpleRecoMomentumCor_ >= LowerPCut && event->simpleRecoMomentumCor_ < UpperPCut;
    // = event->RCSRPred_ <= 0.2;
    
    //sel_contained_in_MRD_ = event->reco_contained_in_MRD_;
    sel_contained_in_MRD_ =  event->reco0pi_contained_in_MRD_;
    sel_vertex_in_FV_ = event->recoFV_ ;
    
    //single_ring_score_ = event->RCSRPred_;
    //multi_ring_score_ = event->RCMRPred_;
    
    pmu_gev_ = event->simpleRecoMomentumCor_ * .001; // MeV -> GeV
    
     // James cuts 
    //trigword==5 && HasTank==1 && HasMRD==1 && TankMRDCoinc==1 && NoVeto==1 && 
    //simpleRecoFlag==1 && reco0pi_contained_in_MRD && recoFV && recoPE && reco_0pi && simpleRecoCosTheta > 0.8 
    //trigword==5 && HasTank==1 && HasMRD==1 && TankMRDCoinc==1 && NoVeto==1 && simpleRecoFlag==1 && reco0pi_contained_in_MRD && recoFV && recoPE && reco_0p
    sel_costheta_threshold_ =event->simpleRecoCosTheta_ > 0.8;
    sel_trigword_ = event->trigword_ == 5; 
    sel_HasTank_ = event->HasTank_ ==1;
    sel_HasMRD_ = event->HasMRD_==1; 
    sel_TankMRDCoinc_ = event->TankMRDCoinc_==1; 
    sel_noVeto_ = event->NoVeto_ ==1; 
    sel_simpleRecoFlag_ = event->simpleRecoFlag_==1;
    sel_promptMuonTotalPE_ = (event->promptMuonTotalPE_ > 500.0) && (event->promptMuonTotalPE_ < 3000.0);
    
     //std::cout<<" event->MRDStop_->size()  = "<< event->MRDStop_->size() << std::endl;
     for (int i = 0; i < static_cast<int>(event->MRDStop_->size()); ++i) {
     // std::cout<<"*event->MRDStop_)[ "<<i  << "]"<< ((*event->MRDStop_)[i] ? "true " : "false ")  << std::endl;
      if ((*event->MRDStop_)[i]) {
            sel_Atleast1track_MRDSTOP_ = true;
            break;
        }
      }
    
    double vertexX =  finite_or_sentinel(event->simpleRecoVtxX_*100, 99999);
    double vertexY =  finite_or_sentinel(event->simpleRecoVtxY_*100, 99999);
    double vertexZ =  finite_or_sentinel(event->simpleRecoVtxZ_*100, 99999);
    
    
    double vertexX_MRDStart =  finite_or_sentinel(event->simpleRecoMRDStartX_*100, 99999);
    double vertexY_MRDStart =  finite_or_sentinel(event->simpleRecoMRDStartY_*100, 99999);
    double vertexZ_MRDStart =  finite_or_sentinel(event->simpleRecoMRDStartZ_*100, 99999);
    //
    double vertexX_MRDStop =  finite_or_sentinel(event->simpleRecoMRDStopX_*100, 99999);
    double vertexY_MRDStop =  finite_or_sentinel(event->simpleRecoMRDStopY_*100, 99999);
    double vertexZ_MRDStop =  finite_or_sentinel(event->simpleRecoMRDStopZ_*100, 99999);
    

    lenghtvertextoStartMRD_cm_= DistanceWithCheck(vertexX ,vertexY+Y_reco_AlineShift_ANNIE,vertexZ + Z_reco_AlineShift_ANNIE,
                                                  vertexX_MRDStart, vertexY_MRDStart, vertexZ_MRDStart);
    tracklenghtMRD_cm_=DistanceWithCheck(vertexX_MRDStart, vertexY_MRDStart, vertexZ_MRDStart,
                                         vertexX_MRDStop, vertexY_MRDStop, vertexZ_MRDStop);                                     
                                                  
    
      vertexRR_ = vertexX*vertexX+(vertexZ+Z_reco_AlineShift_ANNIE)*(vertexZ+Z_reco_AlineShift_ANNIE);
      vertexR_ = std::sqrt(vertexRR_);
                                         
      vertexX_ = vertexX;
      vertexY_ = vertexY + Y_reco_AlineShift_ANNIE;
      vertexZ_ = vertexZ + Z_reco_AlineShift_ANNIE;
       
    
    //sel_promptMuonTotalPE_ = ((event->promptMuonTotalPE_ > 500.0) || (event->promptMuonTotalPE_ < 3500.0)) ? true : false;
    
    //std::cout<<"promptMuonTotalPE_ = " << event->promptMuonTotalPE_ << std::endl;
    //std::cout<<"sel_promptMuonTotalPE_  ="<< (sel_promptMuonTotalPE_ ? "true " : "false ")  << std::endl;
    
    //sel_promptMuonTotalPE_ = event->recoPE_;
    //sel_nu_mu_cc_ = event->simpleRecoFlag_==1 && sel_muon_passed_mom_cuts_ && sel_contained_in_MRD_ && sel_costheta_threshold_;
  
    sel_nu_mu_cc_ = sel_simpleRecoFlag_ && 
                    sel_contained_in_MRD_ && 
                    sel_trigword_  && 
                    sel_HasTank_ && 
                    sel_HasMRD_ && 
                    sel_TankMRDCoinc_ && 
                    sel_noVeto_ ; 
     
    sel_has_pion_candidate_Qij_ = (event->promptMuonTotalPE_ > 200.0*event->Qij_*event->Qij_) && (event->promptMuonTotalPE_ < 2000.0*std::cbrt(4.5-event->Qij_)+1500.0);
   //reco0pi = ((PE > 200.*Qij*Qij) && (PE < 2000.*std::cbrt(4.5-Qij)+1500.)) ? true : false;
  //sel_has_pion_candidate_Qij_ = event->reco_0pi_;
                                       
  //  sel_CC0pi_wc_ = sel_nu_mu_cc_ && sel_vertex_in_FV_ && 
  //                 !sel_has_pion_candidate_ && sel_costheta_threshold_ && 
  //                  sel_muon_passed_mom_cuts_;
  
     sel_CC0pi_wc_ = sel_nu_mu_cc_ && 
                    sel_vertex_in_FV_ && 
                    //sel_has_pion_candidate_Qij_ &&
                    sel_promptMuonTotalPE_ && 
                    sel_costheta_threshold_ && 
                    sel_muon_passed_mom_cuts_ ;
  
  

  
      //if(DeBugg)std::cout << "sel_CC0pi_wc_: " << (sel_CC0pi_wc_ ? "true" : "false") << std::endl;
 
   //  if(sel_CC0pi_wc_==true){
 //
   //  // Optionally, print the values of all involved variables for debugging
   //  if(DeBugg)std::cout << "Event details:" << std::endl;
   //  if(DeBugg)std::cout << "  simpleRecoFlag: " << event->simpleRecoFlag_ << std::endl;
   //  if(DeBugg)std::cout << "  reco_contained_in_MRD: " << event->reco_contained_in_MRD_ << std::endl;
   //  if(DeBugg)std::cout << "  recoFV: " << event->recoFV_ << std::endl;
   //  if(DeBugg)std::cout << "  RCSRPred: " << event->RCSRPred_ << std::endl;
   //  if(DeBugg)std::cout << "  simpleRecoCosTheta: " << event->simpleRecoCosTheta_ << std::endl;
   //  if(DeBugg)std::cout << "  simpleRecoMomentumCor: " << event->simpleRecoMomentumCor_ << std::endl;
   //  if(DeBugg)std::cout<<" "<< std::endl;
   //}     
  
  return sel_CC0pi_wc_;
}


/*
void JOINTCC0pi_ANNIE::Audit_result( AnalysisANNIE_Event* event ) {

  if(mc_is_cc0pi_signal_== true && 
   (category_== kOOFV || 
   category_== kNC  ||
   category_== kNuECC ||
   category_== kNuMuCCNpi ||
   category_== kNuMuCCOther ||
   category_== kOther)){
    std::cout<<"audit Failed Case I "<< std::endl;
   }
  else if(mc_is_cc0pi_signal_==false && 
        (category_==1 ||
          category_==2 ||
          category_==3 ||
          category_==4)){
          
          std::cout<<"audit Failed Case II "<< std::endl;
           std::cout << "Event details:" << std::endl;
           std::cout << "  trueCC: " << event->trueCC_ << std::endl;
           std::cout << "  trueFSLPdg: " << event->trueFSLPdg_ << std::endl;
           std::cout << "  trueTrackLengthInMRD: " << event->trueTrackLengthInMRD_ << std::endl;
           std::cout << "  trueFV: " << event->trueFV_ << std::endl;
           std::cout << "  true_no_mesons: " << event->true_no_mesons_ << std::endl;
           std::cout << "  trueCosTheta: " << event->trueCosTheta_ << std::endl;
           std::cout << "  trueMuonMomentum: " << event->trueMuonMomentum_ << std::endl;
           std::cout << "event->trueQEL = " << event->trueQEL_ << std::endl;
           std::cout << "event->trueRES_ = " << event->trueRES_ << std::endl;
           std::cout << "event->trueDIS_ = " << event->trueDIS_ << std::endl;
           std::cout << "event->trueCOH_ = " << event->trueCOH_ << std::endl;
           std::cout << "event->trueMEC_ = " << event->trueMEC_ << std::endl;
           std::cout<<"mc_neutrino_is_numu  ="<< (mc_neutrino_is_numu_ ? "true " : "false ")  << std::endl; 
           std::cout<<"mc_vertex_in_FV  ="<< (mc_vertex_in_FV_ ? "true " : "false ")  << std::endl; 
           std::cout<<"mc_no_fs_mesons  ="<< (mc_no_fs_mesons_ ? "true " : "false ")  << std::endl; 
           std::cout << "mc_is_cc0pi_signal_: " << (mc_is_cc0pi_signal_ ? "true" : "false") << std::endl;
        
          
          
          }







}*/




    int JOINTCC0pi_ANNIE::categorize_event(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
        return 0;  // Return a default integer value
    }

    bool JOINTCC0pi_ANNIE::selection(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
        return false;  // Return a default boolean value
    }

    bool JOINTCC0pi_ANNIE::define_signal(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
        return false;  // Return a default boolean value
    }

    void JOINTCC0pi_ANNIE::compute_reco_observables(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
    }

    void JOINTCC0pi_ANNIE::compute_true_observables(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
    }
    
    /*
   ////////////////////////////////////////////////////
   //Testing old 
   ////////////////////////////////////////////////////
  bool JOINTCC0pi_ANNIE::selection( AnalysisANNIE_Event* event ) {
    
   // reco_contained_in_MRD_ = event->numMRDTracks_ == 1 ?  first_or(event->MRDStop_, false) : false;
 
    sel_muon_passed_mom_cuts_ = event->simpleRecoMomentumCor_ >= 600.0 && event->simpleRecoMomentumCor_ < 1200.0;
     sel_has_pion_candidate_ = event->RCSRPred_ <= 0.2;
     
    sel_costheta_threshold_ =event->simpleRecoCosTheta_ > 0.8;
    //sel_contained_in_MRD_ = event->reco_contained_in_MRD_;
    sel_contained_in_MRD_ =  event->reco0pi_contained_in_MRD_;
    sel_vertex_in_FV_ = event->recoFV_ ;
    
    //single_ring_score_ = event->RCSRPred_;
    //multi_ring_score_ = event->RCMRPred_;
    pmu_gev_ = event->simpleRecoMomentumCor_ * .001; // MeV -> GeV
     
    sel_nu_mu_cc_ = event->simpleRecoFlag_==1 && sel_contained_in_MRD_ ;

  
      sel_CC0pi_wc_ = sel_nu_mu_cc_ && sel_vertex_in_FV_ && 
                   !sel_has_pion_candidate_ && sel_costheta_threshold_ && 
                  sel_muon_passed_mom_cuts_;

  
  
  
   return sel_CC0pi_wc_;
}



int JOINTCC0pi_ANNIE::categorize_event( AnalysisANNIE_Event* event ) {
 

  if ( !mc_vertex_in_FV_ ) {
    //fTruemc_is_cc0pi_signal = 0;
    category_  = static_cast<int>(kOOFV);
  ///mc_is_cc0pi_signal_= false; 
    return kOOFV;
  }
  
  else if ( event->trueNC_==1 ) {
  category_  = static_cast<int>(kNC);
  //mc_is_cc0pi_signal_= false; 
    return  kNC; 
  }

  else if ( !(event->trueCC_==1 && event->trueFSLPdg_ == 13) ) {
  
    if ( event->trueNuPDG_ == 12 && event->trueCC_==1 ) {
             //mc_is_cc0pi_signal_= false; 
             category_  = static_cast<int>(kNuECC);
             return kNuECC;
      }
    
    else { //mc_is_cc0pi_signal_= false; 
           category_  = static_cast<int>(kOther);
           return kOther;
         }
  }



  // Sort signal by interaction mode
  else if ( mc_is_cc0pi_signal_ ) {
  
   if ( event->trueQEL_==1 && event->trueRES_== 0 && event->trueDIS_==0 && event->trueCOH_==0 && event->trueMEC_==0) {      category_  = static_cast<int>(kSignalCCQE);return kSignalCCQE;  }// QE}
    else if ( event->trueQEL_==0 && event->trueRES_== 0 && event->trueDIS_==0 && event->trueCOH_==0 && event->trueMEC_==1 ) {category_  = static_cast<int>(kSignalCCMEC);return kSignalCCMEC; } // MEC}
    else if ( event->trueQEL_==0 && event->trueRES_== 1 && event->trueDIS_==0 && event->trueCOH_==0 && event->trueMEC_==0 ) {category_  = static_cast<int>(kSignalCCRES);return kSignalCCRES; } // RES}
    else{  category_  = static_cast<int>(kSignalOther);
    return kSignalOther;}
 }
  
       //else if ( mc_nu_interaction_type_ == 2 ) // DIS
    //else if ( mc_nu_interaction_type_ == 3 ) // COH
  
  else if ( mc_no_fs_mesons_==false || mc_no_charged_pi_above_threshold_==false || mc_no_fs_pi0_==false) {
             category_  = static_cast<int>(kNuMuCCNpi);
            return kNuMuCCNpi; }
 else if(mc_muon_in_mom_range_==false || 
        mc_muon_costheta_threshold_==false){
        category_  = static_cast<int>(kNuMuCCOther);
        return kNuMuCCOther;
        
        }
  else {
  category_  = static_cast<int>(kUnknown);
  return kUnknown;}
       
}


bool JOINTCC0pi_ANNIE::define_signal(AnalysisANNIE_Event* event) {
  // Determine whether an input MC event fulfills the signal definition.
  // Only truth information should be used to determine the answer.
  //if(DeBugg)std::cout<< "inside:define_signal "<< std::endl;
  
  mc_neutrino_is_numu_ = event->trueCC_==1 && event->trueFSLPdg_ == 13;
  mc_vertex_in_FV_  = event->trueFV_;
  mc_muon_in_mom_range_ = event->trueMuonMomentum_ >= 600.0 && event->trueMuonMomentum_ < 1200.0;
  mc_muon_costheta_threshold_= event->trueCosTheta_ > 0.8;
  
  mc_num_protons_ = event->trueProtons_;
  mc_num_neutrons_ = event->trueNeutrons_;
  mc_num_charged_pions_ = event->truePiPlus_ + event->truePiMinus_;
  mc_num_neutral_pions_ = event->truePi0_;
  mc_num_wc_charged_pions_ = event->truePiPlusCher_ + event->truePiMinusCher_;
  mc_num_kaons_ = event->trueKPlus_ + event->trueKMinus_; 
  mc_num_wc_kaons_ = event->trueKPlusCher_ + event->trueKMinusCher_;
  mc_no_charged_pi_above_threshold_ = mc_num_wc_charged_pions_== 0;
  mc_no_fs_pi0_ = mc_num_neutral_pions_==0;
  mc_no_fs_mesons_ =  mc_no_charged_pi_above_threshold_ &&  (mc_num_wc_kaons_==0) && mc_no_fs_pi0_;
  mc_true_no_followers_ = event->true_no_followers_;
  mc_pmu_gev_ = event->trueMuonMomentum_ * .001; // MeV -> GeV
  
  mc_is_cc0pi_signal_ =  mc_neutrino_is_numu_ &&
                         mc_vertex_in_FV_ &&
                         mc_no_fs_mesons_  && 
                         //mc_true_no_followers_ &&
                         mc_muon_in_mom_range_ && 
                         mc_muon_costheta_threshold_;
                  
  
         
  // Print the result
  
   //if(mc_neutrino_is_numu_==false && event->trueCC_==1 && event->trueFSLPdg_ == 13 && event->trueTrackLengthInMRD_ > 0.){
   
   if(DeBugg) std::cout<<" "<< std::endl; 

    
   if(DeBugg) std::cout<<"mc_neutrino_is_numu  ="<< (mc_neutrino_is_numu_ ? "true " : "false ")  << std::endl; 
   if(DeBugg) std::cout<<"mc_vertex_in_FV  ="<< (mc_vertex_in_FV_ ? "true " : "false ")  << std::endl; 
   if(DeBugg) std::cout<<"mc_no_fs_mesons  ="<< (mc_no_fs_mesons_ ? "true " : "false ")  << std::endl; 
   if(DeBugg) std::cout << "mc_is_cc0pi_signal_: " << (mc_is_cc0pi_signal_ ? "true" : "false") << std::endl;
    // Optionally, print the values of all involved variables for debugging
   
    
   if(DeBugg) std::cout << "Event details:" << std::endl;
   if(DeBugg) std::cout << "  trueCC: " << event->trueCC_ << std::endl;
   if(DeBugg) std::cout << "  trueFSLPdg: " << event->trueFSLPdg_ << std::endl;
   if(DeBugg) std::cout << "  trueTrackLengthInMRD: " << event->trueTrackLengthInMRD_ << std::endl;
   if(DeBugg) std::cout << "  trueFV: " << event->trueFV_ << std::endl;
   if(DeBugg) std::cout << "  true_no_mesons: " << event->true_no_mesons_ << std::endl;
   if(DeBugg) std::cout << "  trueCosTheta: " << event->trueCosTheta_ << std::endl;
   if(DeBugg) std::cout << "  trueMuonMomentum: " << event->trueMuonMomentum_ << std::endl;
   if(DeBugg) std::cout << "event->trueQEL = " << event->trueQEL_ << std::endl;
   if(DeBugg) std::cout << "event->trueRES_ = " << event->trueRES_ << std::endl;
   if(DeBugg) std::cout << "event->trueDIS_ = " << event->trueDIS_ << std::endl;
   if(DeBugg) std::cout << "event->trueCOH_ = " << event->trueCOH_ << std::endl;
   if(DeBugg) std::cout << "event->trueMEC_ = " << event->trueMEC_ << std::endl;
   
   if(DeBugg) std::cout<<" "<< std::endl;
    
   //}
    
    
    
  
  return mc_is_cc0pi_signal_;
}

*/
