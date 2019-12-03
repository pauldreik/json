#!/bin/sh

#
# builds the fuzzers

set -eu

fuzzdir=$(dirname $0)

cd $fuzzdir

CLANG=clang++-8
srcfile=fuzzer.cpp
fuzzer=./fuzzer

if [ ! -e $fuzzer -o $srcfile -nt $fuzzer ] ; then

$CLANG \
-std=c++17 \
-fsanitize=fuzzer,address,undefined \
-fno-sanitize-recover=undefined \
-DBOOST_JSON_STANDALONE \
-DBOOST_JSON_HEADER_ONLY \
-I../include \
-o $fuzzer \
$srcfile

fi

# make sure the old crashes pass without problems
find old_crashes -type f -print0 |xargs -0 $fuzzer

# make an initial corpus from the test data already in the repo
seedcorpus=seedcorpus
if [ ! -d $seedcorpus ] ; then
 mkdir $seedcorpus
 find ../test -name "*.json" -type f -print0 |xargs -0 cp -f -t $seedcorpus/
fi

# run the fuzzer for a short while
if [ ! -d out ] ; then
mkdir -p out
$fuzzer out/ $seedcorpus/ -max_total_time=30
fi

# minimize the corpus
if [ ! -d cmin ] ; then
mkdir -p cmin
$fuzzer cmin/ out/ $seedcorpus/ -merge=1
fi

