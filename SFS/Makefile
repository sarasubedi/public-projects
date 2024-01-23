###############################################################################################
# Make file template
# Author:	Ahmad Abdullah
#					University of Victoria
#					Computer Science Department
#					CSC 360 Introduction to Operating Systems
#					Spring 2023
#
# Modified by: Sara Subedi (V00986656)
# Last Updated: April 4th, 2023
###############################################################################################
COMPILER=gcc
OPTIONS=-g -Wall -Werror -Wunreachable-code -std=c11 -DDEBUG
COMPILE=$(COMPILER) $(OPTIONS)
BUILD=build
PROGRAM=diskinfo
PROGRAM2=disklist
PROGRAM3=diskget
PROGRAM4=diskput

# Compile main by default
all: info list get put

info: diskinfo.c $(BUILD)/linked-list.o $(BUILD)/emalloc.o input-structs.c
	$(COMPILE) $< $(BUILD)/*.o -o $(PROGRAM) 

list: disklist.c $(BUILD)/linked-list.o $(BUILD)/emalloc.o input-structs.c
	$(COMPILE) $< $(BUILD)/*.o -o $(PROGRAM2) -lm

get: diskget.c $(BUILD)/linked-list.o $(BUILD)/emalloc.o input-structs.c
	$(COMPILE) $< $(BUILD)/*.o -o $(PROGRAM3)

put: diskput.c $(BUILD)/linked-list.o $(BUILD)/emalloc.o input-structs.c
	$(COMPILE) $< $(BUILD)/*.o -o $(PROGRAM4)

$(BUILD)/linked-list.o: linked-list.c  build
	$(COMPILE) -c $< -o $@

$(BUILD)/emalloc.o: emalloc.c build
	$(COMPILE) -c $< -o $@

$(BUILD)/diskFunc.o: diskFunc.c build
	$(COMPILE) -c $< -o $@

$(BUILD)/strparse.o: strparse.c build
	$(COMPILE) -c $< -o $@

# Make the build directory if it doesn't exist
build:
	mkdir -p $(BUILD)

# Delete the build directory and program
clean:
	rm -rf $(BUILD) $(PROGRAM) $(PROGRAM2) $(PROGRAM3) $(PROGRAM4)

# These rules do not correspond to a specific file
.PHONY: build clean
