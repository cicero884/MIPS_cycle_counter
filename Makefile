SHELL := /bin/bash

SRC_DIR := src
OUT_DIR := output

FORMAT := clang-format
CC := gcc
CFLAG := -g -Wall -I

SRC := $(wildcard ./$(SRC_DIR)/*.c)
SRC2 := $(notdir $(SRC))
OBJ := $(SRC2:%.c=%.o)

all: $(OBJ)
	$(CC) -o $(OUT_DIR)/cycle_calculator $(OUT_DIR)/$(OBJ)

%.o: $(SRC_DIR)/%.c
	$(CC) -o $(OUT_DIR)/$@ -c $(CFLAGS) $<

clean:
	rm $(OUT_DIR)/*.o

format:
	@echo "format all files"
	@find . -name '*.h' -or -name '*.cpp' -or -name '*.c' | xargs clang-format -i -style=file $1
