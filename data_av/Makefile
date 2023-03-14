###############################################################################################
# Make file template
# Author:	Ahmad Abdullah
#					University of Victoria
#					Computer Science Department
#					CSC 360 Introduction to
#					Operating Systems
#					Spring 2023
#
# Modified by: Sara Subedi (V00986656)
# Last Updated: March 13, 2023
###############################################################################################

COMPILER=gcc
OPTIONS=-g -Wall -Werror -Wunreachable-code -std=c11 -DDEBUG  -pthread -D_GNU_SOURCE

COMPILE=$(COMPILER) $(OPTIONS)
BUILD=build
PROGRAM=data_av

all: program

program: data_av.c
	$(COMPILE) $< -o $(PROGRAM)

build:
	mkdir -p $(BUILD)

# Delete the build directory and program
clean:
	rm -rf $(BUILD) $(PROGRAM)

# These rules do not correspond to a specific file
.PHONY: build clean
