CXX = g++
CXXFLAGS = -g -o -Wall -fPIC -Wno-unused-variable
ROOTFLAGS = `root-config --cflags --glibs --libs` -lTreePlayer -lEG -lMinuit
#XGBOOST=/exp/uboone/app/users/mastbaum/xgboost/build
#CC0PI_ANALYZER_FLAGS = -I${XGBOOST}/include -L${XGBOOST}/lib64 -lxgboost -g

CXXFLAGS += -I../include
LIB_DIR := /exp/uboone/app/users/cnguyen/stv-analysis-II/xsec_analyzer/lib

ROOT_DICTIONARY := $(LIB_DIR)/dictionaries.o
#SHARED_LIB := $(LIB_DIR)/libXSecAnalyzer.so

LDFLAGS := $(shell root-config --libs) -L$(LIB_DIR) -lXSecAnalyzer
LDFLAGS += -Wl,-rpath,/exp/uboone/app/users/cnguyen/stv-analysis-II/xsec_analyzer/lib


# make a binary for every .cxx file
 all : $(patsubst %.cpp, %.o, $(wildcard *.cpp)) tutorial_slice_plots_test 

# cc0pi_analyzer_org
# cc0pi_analyzer
# # rule for each one
# 

%: %.cpp
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -o $(CC0PI_ANALYZER_FLAGS) $(LDFLAGS) $@ $<  includes/*.o
	
 %.o : %.cpp
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS)  $(LDFLAGS)   -o $*.o  -c $*.cpp 
	$(CXX( $(CXXFLAGS) $(ROOTFLAGS) -o $* $*.o  $(LDFLAGS) includes/*.o 

stv_root_dict.o:
	$(RM) stv_root_dict*.*
	rootcling -f stv_root_dict.cc -c LinkDef.h
	$(CXX) $(shell root-config --cflags --libs) -O3 \
	-fPIC -o stv_root_dict.o -c stv_root_dict.cc
	$(RM) stv_root_dict.cc
	

tutorial_slice_plots_test: tutorial_slice_plots_test.C 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(CC0PI_ANALYZER_FLAGS) $(LDFLAGS) -O3 -g -o $@ $^ include/*.o

.PHONY: clean

.INTERMEDIATE: stv_root_dict.o

clean:
	rm -f $(wildcard *.o) $(patsubst %.cpp, %, $(wildcard *.cpp)) tutorial_slice_plots_test
