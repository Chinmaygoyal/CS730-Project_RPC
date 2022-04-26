CC = gcc
CFLAGS = -g

LIB=lib
SRC=src
TEST_SRC=tests
BENCH_SRC=benchmark-tests
INC=include
LIB_OBJ=lib
OBJ=obj
BIN=bin

HDRS=$(shell ls $(INC)/*.h)
TEST_FILES=$(wildcard $(TEST_SRC)/*.c)
TEST_BINS=$(TEST_FILES:$(TEST_SRC)/%.c=$(BIN)/%)
BENCH_FILES=$(wildcard $(BENCH_SRC)/*.c)
BENCH_BINS=$(BENCH_FILES:$(BENCH_SRC)/%.c=$(BIN)/%)

all: library benchmarks tests

library: $(LIB)/libsfork.so

$(LIB)/libsfork.so: $(LIB_OBJ)/sfork_lib.o
	gcc -shared -o $@ $^

$(LIB_OBJ)/%.o: $(SRC)/%.c $(HDRS)
	mkdir -p lib
	$(CC) -fPIC -c -I$(INC) $< -o $@

tests: $(TEST_BINS)
benchmarks: $(BENCH_BINS)

$(BIN)/%: $(BENCH_SRC)/%.c $(HDRS)
	mkdir -p $(BIN)
	$(CC) -I$(INC) $< -o $@ -L${LIB} -lsfork

$(BIN)/%: $(TEST_SRC)/%.c $(HDRS)
	mkdir -p $(BIN)
	$(CC) -I$(INC) $< -o $@ -L${LIB} -lsfork


.PHONY: clean
clean:
	rm -rf $(OBJ) $(LIB) $(LIB_OBJ) $(BIN)
