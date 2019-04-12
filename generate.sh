#!/bin/bash

TEST_PATH="$(pwd)/tests"
IN_PATH="$TEST_PATH/in"
OUT_PATH="$TEST_PATH/out"
GEN_PATH="$TEST_PATH/gens"
SEED_PATH="$TEST_PATH/seed"

make

files=('1' '2' '3' '4' '5' '6' '7' '8' '9' '10')

for ((i=0; i<${#files[@]}; i++)) do
    file=${files[${i}]}

    #./generator.out -f $SEED_PATH/$file.seed < $GEN_PATH/$file.gen 1> $IN_PATH/$file.in 2> $OUT_PATH/$file.out
    ipython calcLimit.py $IN_PATH/$file.in >> $OUT_PATH/$file.out
    ipython check.py $IN_PATH/$file.in $OUT_PATH/$file.out

done
