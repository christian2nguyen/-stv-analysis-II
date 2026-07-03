#pragma once

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct PMTPositionRecord {
    int detector_num = -1;
    int channel_num = -1;
    std::string detector_tank_location;
    int panel_number = -1;
    double x_pos = 0.0;
    double y_pos = 0.0;
    double z_pos = 0.0;
    double x_dir = 0.0;
    double y_dir = 0.0;
    double z_dir = 0.0;
    std::string pmt_type;
    std::string cable_label;
    std::string detector_status;
    double nominal_hv = 0.0;
    std::string notes;
};

// Cylindrical coordinates for detector convention:
// y = vertical axis (up/down), z = beam direction, x = left/right.
// We therefore use:
//   r   = sqrt(x^2 + z^2)
//   phi = atan2(x, z)   (phi=0 along +z beam, +phi toward +x)
//   y   = unchanged axial coordinate
struct PMTCylindricalCoord {
    double r = 0.0;
    double phi = 0.0;
    double y = 0.0;
};

struct PMTBeamSideSplit {
    std::vector<int> upstream_ids;
    std::vector<int> downstream_ids;
};

class PMTPositionInfo {
  public:
    explicit PMTPositionInfo(double beam_split_z = 0.0) : beam_split_z_(beam_split_z) {}

    bool LoadFromFile(const std::string& file_path) {
        records_.clear();
        by_id_.clear();

        std::ifstream in(file_path.c_str());
        if (!in.is_open()) return false;

        std::string line;
        while (std::getline(in, line)) {
            const std::string tline = Trim(line);
            if (tline.empty()) continue;
            if (tline[0] == '#') continue;

            const std::vector<std::string> cols = SplitCSVLine(tline);
            if (cols.size() < 26) continue;

            PMTPositionRecord rec;
            if (!TryParseInt(cols[0], rec.detector_num)) continue;
            if (!TryParseInt(cols[1], rec.channel_num)) continue;

            rec.detector_tank_location = Trim(cols[2]);
            TryParseInt(cols[3], rec.panel_number);
            TryParseDouble(cols[4], rec.x_pos);
            TryParseDouble(cols[5], rec.y_pos);
            TryParseDouble(cols[6], rec.z_pos);
            TryParseDouble(cols[7], rec.x_dir);
            TryParseDouble(cols[8], rec.y_dir);
            TryParseDouble(cols[9], rec.z_dir);
            rec.pmt_type = Trim(cols[10]);
            rec.cable_label = Trim(cols[11]);
            rec.detector_status = Trim(cols[12]);
            TryParseDouble(cols[24], rec.nominal_hv);
            rec.notes = Trim(cols[25]);

            records_.push_back(rec);
            by_id_[rec.detector_num] = rec;
        }

        return !records_.empty();
    }

    size_t Size() const { return records_.size(); }

    bool HasPMT(int pmt_id) const { return by_id_.find(pmt_id) != by_id_.end(); }

    const PMTPositionRecord* GetByPMTId(int pmt_id) const {
        const auto it = by_id_.find(pmt_id);
        if (it == by_id_.end()) return nullptr;
        return &it->second;
    }

    std::vector<int> GetPMTIdsByTankLocation(const std::string& location) const {
        std::vector<int> ids;
        const std::string want = ToLower(Trim(location));
        for (const auto& rec : records_) {
            if (ToLower(rec.detector_tank_location) == want) {
                ids.push_back(rec.detector_num);
            }
        }
        return ids;
    }

    std::vector<int> GetPMTIdsByStatus(const std::string& status) const {
        std::vector<int> ids;
        const std::string want = ToLower(Trim(status));
        for (const auto& rec : records_) {
            if (ToLower(rec.detector_status) == want) {
                ids.push_back(rec.detector_num);
            }
        }
        return ids;
    }

    std::vector<int> GetPMTIdsInYRange(double y_min, double y_max) const {
        std::vector<int> ids;
        for (const auto& rec : records_) {
            if (rec.y_pos >= y_min && rec.y_pos <= y_max) {
                ids.push_back(rec.detector_num);
            }
        }
        return ids;
    }

    std::vector<int> GetPMTIdsInRadiusRange(double r_min, double r_max) const {
        std::vector<int> ids;
        for (const auto& rec : records_) {
            const double r = std::sqrt(rec.x_pos * rec.x_pos + rec.z_pos * rec.z_pos);
            if (r >= r_min && r <= r_max) {
                ids.push_back(rec.detector_num);
            }
        }
        return ids;
    }

    bool PassPositionCut(int pmt_id, const std::string& cut_name) const {
        const PMTPositionRecord* rec = GetByPMTId(pmt_id);
        if (!rec) return false;

        const std::string cut = ToLower(Trim(cut_name));
        const std::string loc = ToLower(rec->detector_tank_location);
        const bool is_on = (ToLower(rec->detector_status) == "on");
        const bool is_top = (loc == "top");
        const bool is_bottom = (loc == "bottom");
        const bool is_barrel = (loc != "top" && loc != "bottom");

        if (cut == "all") return true;
        if (cut == "on_only") return is_on;
        if (cut == "top_only") return is_top;
        if (cut == "bottom_only") return is_bottom;
        if (cut == "barrel_only") return is_barrel;
        if (cut == "on_bottom_only") return is_on && is_bottom;
        if (cut == "on_top_only") return is_on && is_top;
        if (cut == "on_barrel_only") return is_on && is_barrel;
        return false;
    }

    double PositionWeight(int pmt_id, const std::string& model_name) const {
        const PMTPositionRecord* rec = GetByPMTId(pmt_id);
        if (!rec) return 1.0;

        const std::string model = ToLower(Trim(model_name));
        const std::string loc = ToLower(rec->detector_tank_location);
        const bool is_top = (loc == "top");
        const bool is_bottom = (loc == "bottom");
        const bool is_barrel = (loc != "top" && loc != "bottom");

        if (model.empty() || model == "uniform") return 1.0;
        if (model == "top_boost") return is_top ? 1.10 : 1.0;
        if (model == "bottom_boost") return is_bottom ? 1.10 : 1.0;
        if (model == "barrel_boost") return is_barrel ? 1.10 : 1.0;
        if (model == "top_suppress") return is_top ? 0.90 : 1.0;
        if (model == "bottom_suppress") return is_bottom ? 0.90 : 1.0;

        // Smooth radial model in x-z: slightly boosts outer PMTs.
        if (model == "radial_outer_boost") {
            const double r = std::sqrt(rec->x_pos * rec->x_pos + rec->z_pos * rec->z_pos);
            const double r0 = 1.0;
            return 1.0 + 0.15 * std::tanh((r - r0) / 0.5);
        }

        return 1.0;
    }

    void SetBeamSplitZ(double z_split) { beam_split_z_ = z_split; }
    double GetBeamSplitZ() const { return beam_split_z_; }

    bool IsUpstreamPMT(int pmt_id) const {
        const PMTPositionRecord* rec = GetByPMTId(pmt_id);
        if (!rec) return false;
        return rec->z_pos < beam_split_z_;
    }

    bool IsDownstreamPMT(int pmt_id) const {
        const PMTPositionRecord* rec = GetByPMTId(pmt_id);
        if (!rec) return false;
        return rec->z_pos >= beam_split_z_;
    }

    PMTBeamSideSplit SplitPMTIdsByBeamZ() const {
        return SplitPMTIdsByBeamZ(beam_split_z_);
    }

    PMTBeamSideSplit SplitPMTIdsByBeamZ(double z_split) const {
        PMTBeamSideSplit out;
        for (const auto& rec : records_) {
            if (rec.z_pos < z_split) out.upstream_ids.push_back(rec.detector_num);
            else out.downstream_ids.push_back(rec.detector_num);
        }
        return out;
    }

    std::vector<int> GetUpstreamPMTIds() const {
        return SplitPMTIdsByBeamZ().upstream_ids;
    }

    std::vector<int> GetDownstreamPMTIds() const {
        return SplitPMTIdsByBeamZ().downstream_ids;
    }

    static PMTCylindricalCoord XYZToCylindricalY(double x, double y, double z) {
        PMTCylindricalCoord c;
        c.r = std::sqrt(x * x + z * z);
        c.phi = std::atan2(x, z);
        c.y = y;
        return c;
    }

    bool GetCylindricalByPMTId(int pmt_id, PMTCylindricalCoord& out) const {
        const PMTPositionRecord* rec = GetByPMTId(pmt_id);
        if (!rec) return false;
        out = XYZToCylindricalY(rec->x_pos, rec->y_pos, rec->z_pos);
        return true;
    }

  private:
    static std::string Trim(const std::string& s) {
        size_t b = 0;
        while (b < s.size() && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
        size_t e = s.size();
        while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) --e;
        return s.substr(b, e - b);
    }

    static std::string ToLower(const std::string& s) {
        std::string out = s;
        std::transform(out.begin(), out.end(), out.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return out;
    }

    static bool TryParseInt(const std::string& s, int& out) {
        try {
            size_t idx = 0;
            const std::string t = Trim(s);
            out = std::stoi(t, &idx);
            return idx == t.size();
        } catch (...) {
            return false;
        }
    }

    static bool TryParseDouble(const std::string& s, double& out) {
        try {
            size_t idx = 0;
            const std::string t = Trim(s);
            out = std::stod(t, &idx);
            return idx == t.size();
        } catch (...) {
            return false;
        }
    }

    static std::vector<std::string> SplitCSVLine(const std::string& line) {
        std::vector<std::string> cols;
        std::stringstream ss(line);
        std::string item;
        while (std::getline(ss, item, ',')) cols.push_back(item);
        return cols;
    }

    std::vector<PMTPositionRecord> records_;
    std::map<int, PMTPositionRecord> by_id_;
    double beam_split_z_ = 0.0;
};

