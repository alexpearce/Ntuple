#######################################################################
# Makefile                                                            #
# Author: Alex Pearce                                                 #
# Email: alex@alexpearce.me                                           #
# Date: 21-11-2012                                                    #
#                                                                     #
# This Makefile is quite flexible. Any .cpp files you have which you  #
# want to be made in to executables, just give them a lowercase name. #
# The Makefile will then look for .cpp and .h files in the SRC        #
# directory, compile them to libraries in the LIB directory, and then #
# link the libraries in to executables in the current directory.      #
#                                                                     #
# If you'd like to keep your source and libraries in one folder, set  #
# SRC and LIB to ".".                                                 #
#                                                                     #
# By default, running `make` will generate all executables.           #
# To remake a specific executable, just run `make executable_name`.   #
#                                                                     #
# Running `make clean` will remove all library files (.o) and all     #
# executables.                                                        #
#######################################################################

##### Edit below to your liking #####

# Set any of these to . to describe the directory the Makefile resides in
# Source .cpp location
SRC = src
# Source .h location
INCLUDE = include
# Where to put generated .o files
LIB = lib
# Where to put generated executables
BIN = bin

# Extensions for implementation files...
C_EXT = cpp
# ... and header files
H_EXT = h

# Any extra libraries you want to load
# MY_LIBS = -lRooFitCore -lRooFit -lRooStats

##### Everything down here can be left alone #####

# Names of the .cpp programs to be compiled to executables
PROGRAMS := $(shell ls $(SRC) | egrep '^[a-z]+' | sed 's/\(.*\)\.$(C_EXT)/\1/')

# List of .cpp files to be compiled to executables
EXECUTABLES := $(addprefix $(SRC)/, $(addsuffix .$(C_EXT), $(PROGRAMS)))

# List of .cpp files that won't be compiled to executables
CPPFILES := $(filter-out $(EXECUTABLES), $(wildcard $(SRC)/*.$(C_EXT)))

# List of object libraries, made by changing the file extensions in the above list
OBJS := $(patsubst $(SRC)/%.$(C_EXT), $(LIB)/%.o, $(CPPFILES))

# Clang compiler w/ debug info, optimisation, warnings, and compile as a sharable option
# I would use -pedantic but it throws a billion warnings about the ROOT libraries
CXX = g++
CXXFLAGS = -g -O -fPIC -Wall# -pedantic

# ROOT compiler flags
CXXFLAGS += $(shell root-config --cflags)

# Use Clang as the linker
LD = g++

# ROOT linker flags, including the non-default TMVA, RooFit, and RooStats libraries
LDFLAGS := -O $(shell root-config --glibs) 
LDFLAGS += $(MY_LIBS)

# The name "default" is superfluous; running `make` just runs the first rule.
# FACT.
default: $(PROGRAMS)

all: default

# $@ name of target
# $^ name of all prerequisites with duplicates removed
# $< name of the first prerequisite

# Static pattern rule
# Effectively defines a macro for each word in the PROGRAMS list
$(PROGRAMS): %: $(LIB)/%.o $(OBJS)
	@echo LD $<
	@$(LD) $^ $(LDFLAGS) -o $(BIN)/$@

$(LIB)/%.o: $(SRC)/%.$(C_EXT)
	@echo CXX $<
	@$(CXX) $(CXXFLAGS) -I $(INCLUDE) -o $@ -c $<

data: create_sample
	@echo "Creating data..."
	./bin/create_sample

time: data $(PROGRAMS)
	time ./bin/example_ntuple
	time ./bin/example_traditional
	time python python/example_ntuple.py
	time python python/example_traditional.py

.PHONY: clean

clean:
	@$(RM) -f $(LIB)/*.o
	@$(RM) -f $(BIN)/*
	@$(RM) -f *.pdf
	@$(RM) -f data.root
