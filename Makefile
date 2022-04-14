CC = gcc
CFLAGS = -g

LIB=lib
SRC=src
TEST_SRC=tests
INC=include
LIB_OBJ=lib
OBJ=obj
BIN=bin
# BENCH_SRC=benchmark-test
# BENCH_OBJ=bech-obj


HDRS=$(shell ls $(INC)/*.h)
TEST_FILES=$(wildcard $(TEST_SRC)/*.c)
TEST_BINS=$(TEST_FILES:$(TEST_SRC)/%.c=$(BIN)/%)

all: library tests

library: $(LIB)/libsfork.so

$(LIB)/libsfork.so: $(LIB_OBJ)/sfork_lib.o
	gcc -shared -o $@ $^

$(LIB_OBJ)/%.o: $(SRC)/%.c $(HDRS)
	mkdir -p lib
	$(CC) -fPIC -c -I$(INC) $< -o $@

tests: $(TEST_BINS)

$(BIN)/%: $(TEST_SRC)/%.c $(HDRS)
	mkdir -p $(BIN)
	$(CC) -I$(INC) $< -o $@ -L${LIB} -lsfork

# $(OBJ)/%.o: $(TEST_SRC)/%.c $(HDRS)
# 	mkdir -p obj
# 	$(CC) -c -I$(INC) $< -o $@


.PHONY: clean
clean:
	rm -rf $(OBJ) $(LIB) $(LIB_OBJ) $(BIN)
