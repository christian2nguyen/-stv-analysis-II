XX = g++
CXXFLAGS = -g -Wall -fPIC
ROOTFLAGS = `root-config --cflags --glibs --libs` -lTreePlayer -lEG -lMinuit
XGBOOST=/exp/uboone/app/users/mastbaum/xgboost/build

LIB_DIR := /exp/uboone/app/users/cnguyen/stv-analysis-II/xsec_analyzer/lib

ROOT_DICTIONARY := $(LIB_DIR)/dictionaries.o
SHARED_LIB := $(LIB_DIR)/libXSecAnalyzer.$(SHARED_LIB_SUFFIX)

# make a binary for every .cxx file
 all : $(patsubst %.cpp, %.o, $(wildcard *.cpp)) stv_root_dict_includes.o

# # rule for each one
 %.o : %.cpp
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -o $*.o $(LDLIBS) -c  $*.cpp #compile
#       $(CXX) $(INCLUDE) $(CXXFLAGS) $(ROOTFLAGS) $(LDLIBS) -o $* $*.o        #link

stv_root_dict_includes.o:
	$(RM) stv_root_dict*.*
	rootcling -f stv_root_dict_includes.cc -c LinkDef.h
	$(CXX) $(shell root-config --cflags --libs) -O3 \
	-fPIC -o stv_root_dict_includes.o -c stv_root_dict_includes.cc
	$(RM) stv_root_dict_includes.cc
	
.PHONY: clean

.INTERMEDIATE: stv_root_dict_includes.o	

clean:
	$(RM) -f $(wildcard *.o) $(patsubst %.cpp, %, $(wildcard *.cpp))
