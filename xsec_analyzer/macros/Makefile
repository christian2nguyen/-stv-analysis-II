CXX = g++
CXXFLAGS = -g -o -Wall -fPIC -Wno-unused-variable
ROOTFLAGS = `root-config --cflags --glibs --libs` -lTreePlayer -lEG -lMinuit
XGBOOST=/exp/uboone/app/users/mastbaum/xgboost/build
CC0PI_ANALYZER_FLAGS = -I${XGBOOST}/include -L${XGBOOST}/lib64 -lxgboost -g
LIB_DIR := ../lib
ROOT_DICTIONARY := $(LIB_DIR)/dictionaries_rdict.pcm
SHARED_LIB := $(LIB_DIR)/libXSecAnalyzer.so

# make a binary for every .cxx file
 all : $(patsubst %.cpp, %.o, $(wildcard *.cpp)) tutorial_slice_plots_test 

# cc0pi_analyzer_org
# cc0pi_analyzer
# # rule for each one
# 
%: %.cpp
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -o $(CC0PI_ANALYZER_FLAGS) $@ $< includes/*.o
	
 %.o : %.cpp
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -o $*.o  -c $*.cpp 
	$(CXX( $(CXXFLAGS) $(ROOTFLAGS) -o $* $*.o includes/*.o 

cc0pi_analyzer : cc0pi_analyzer.C
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -o $@ $< includes/*.o

cc0pi_analyzer_org: cc0pi_analyzer_org.C
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -o $@ $< includes/*.o

univmake: univmake.C
	$(CXX) $(ROOTFLAGS) -O3 -o $@ $^ includes/*.o

stv_root_dict.o:
	$(RM) stv_root_dict*.*
	rootcling -f stv_root_dict.cc -c LinkDef.h
	$(CXX) $(shell root-config --cflags --libs) -O3 \
	-fPIC -o stv_root_dict.o -c stv_root_dict.cc
	$(RM) stv_root_dict.cc
	
HistMaker: HistMaker.cpp
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o
	
HistMaker_test: HistMaker_test.cpp 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o
	
HistMaker_ForData: HistMaker_ForData.cpp
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o

HistMaker_For_FAKEData: HistMaker_For_FAKEData.cpp
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o

HistMaker_ForData_Test: HistMaker_ForData_Test.cpp 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o

Make_config_mcc9_2D_muon_CC0pi: Make_config_mcc9_2D_muon_CC0pi.cpp 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o

Make_Plots: Make_Plots.cpp 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o
	
Make_Plots_2D: Make_Plots_2D.cpp 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o
	
Make_cc0pi_effpur_pmu: Make_cc0pi_effpur_pmu.cpp 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o

Make_cc0pi_effpur: Make_cc0pi_effpur.cpp 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o
		
Make_cc0pi_effpur_test: Make_cc0pi_effpur_test.cpp 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o		
		
Make_Plots_test: Make_Plots_test.cpp 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o

tutorial_slice_plots_test: tutorial_slice_plots_test.C
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) $(SHARED_LIB) $(ROOT_DICTIONARY) -O3 -o $@ $^ ../include/CC_Joint_cc0pi/*.o
	
migration_matrix_cc0pi: migration_matrix_cc0pi.cpp
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o	

HistMaker_newTuples: HistMaker_newTuples.cpp
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o	

HistMaker_For_FAKEData_newTuples: HistMaker_For_FAKEData_newTuples.cpp 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o
	
HistMaker_newTuples_SideBandStudy: HistMaker_newTuples_SideBandStudy.cpp 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) -O3 -o $@ $^ includes/*.o


.PHONY: clean

.INTERMEDIATE: stv_root_dict.o

clean:
	rm -f $(wildcard *.o) $(patsubst %.cpp, %, $(wildcard *.cpp))  cc0pi_analyzer cc0pi_analyzer_org HistMaker univmake HistMaker_test HistMaker_ForData HistMaker_ForData_Test Make_Plots Make_Plots_2D Make_cc0pi_effpur_pmu Make_cc0pi_effpur Make_Plots_test tutorial_slice_plots_test migration_matrix_cc0pi HistMaker_For_FAKEData Make_config_mcc9_2D_muon_CC0pi Make_cc0pi_effpur_test analyzer HistMaker_newTuples_SideBandStudy cc0pi_analyzer
