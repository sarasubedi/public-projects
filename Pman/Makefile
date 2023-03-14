###############################################################################################
# Make file template
# Author:	Ahmad Abdullah
#					University of Victoria
#					Computer Science Department
#					CSC 360 Introduction to Operating Systems
#					Spring 2023
#
# Modified by: Sara Subedi (V00986656)
# Last Updated: February 11, 2023
###############################################################################################
COMPILER=gcc
OPTIONS=-g -Wall -Werror -Wunreachable-code -std=c11 -DDEBUG -D_GNU_SOURCE

COMPILE=$(COMPILER) $(OPTIONS)
BUILD=build


PROGRAM=PMan

all: program

program: PMan.c $(BUILD)/linked-list.o $(BUILD)/input-parse.o $(BUILD)/bg-cmds.o
	$(COMPILE) $< $(BUILD)/*.o -o $(PROGRAM) -lreadline

$(BUILD)/linked-list.o: linked-list.c  build
	$(COMPILE) -c $< -o $@

$(BUILD)/input-parse.o: input-parse.c linked-list.h build
	$(COMPILE) -c $< -o $@

$(BUILD)/bg-cmds.o: bg-cmds.c linked-list.h build
	$(COMPILE) -c $< -o $@

build:
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD) $(PROGRAM)

.PHONY: build clean
