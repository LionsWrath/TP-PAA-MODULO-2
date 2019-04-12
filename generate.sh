#!/bin/bash

TEST_PATH="$(pwd)/tests"
IN_PATH="$TEST_PATH/in"
OUT_PATH="$TEST_PATH/out"
GEN_PATH="$TEST_PATH/gens"
SEED_PATH="$TEST_PATH/seed"

make
./generator.out -f $SEED_PATH/$1.seed < $GEN_PATH/$1.gen 1> $IN_PATH/$1.in 2> $OUT_PATH/$1.out
ipython calcLimit.py $IN_PATH/$1.in >> $OUT_PATH/$1.out
