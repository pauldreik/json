#!/bin/sh

#
# builds the fuzzers, runs old crashes etc
#
# Optional: set environment variable CLANG, otherwise clang is auto detected.
#
# By Paul Dreik 2019 for the boost json project
# License: Boost 1.0

set -ex

fuzzdir=$(dirname $0)
me=$(basename $0)

cd $fuzzdir

if [ -z $CLANG ] ; then
    #see if we can find clang
    for clangver in -10 -9 -8 -7 -6 -6.0 "" ;   do
	CLANG=clang++$clangver
	if which $CLANG >/dev/null; then
	    break
	fi
    done
fi

if ! which $CLANG >/dev/null; then
    if ! -x $CLANG; then
	echo $me: sorry, could not find clang $CLANG
	exit 1
    fi
fi
echo $me: will use clang $CLANG

srcfile=fuzzer.cpp
fuzzer=./fuzzer

if [ ! -e $fuzzer -o $srcfile -nt $fuzzer ] ; then
    
    $CLANG \
	-std=c++17 \
	-O3 \
	-g \
	-fsanitize=fuzzer,address,undefined \
	-fno-sanitize-recover=undefined \
	-DBOOST_JSON_STANDALONE \
	-DBOOST_JSON_HEADER_ONLY \
	-I../include \
	-o $fuzzer \
	$srcfile
fi

# make sure ubsan stops in case anything is found
export UBSAN_OPTIONS="halt_on_error=1"

# make sure the old crashes pass without problems
find old_crashes -type f -print0 |xargs -0 $fuzzer

# make an initial corpus from the test data already in the repo
seedcorpus=seedcorpus
if [ ! -d $seedcorpus ] ; then
    mkdir $seedcorpus
    find ../test -name "*.json" -type f -print0 |xargs -0 cp -f -t $seedcorpus/
fi

# if an old corpus exists, use it
# get it with curl -O --location -J https://bintray.com/pauldreik/boost.json/download_file?file_path=corpus%2Fcorpus.tar
if [ -e corpus.tar ] ; then
  mkdir -p oldcorpus
  tar xf corpus.tar -C oldcorpus
  OLDCORPUS=oldcorpus/cmin
else
  OLDCORPUS=
fi


# run the fuzzer for a short while
mkdir -p out
$fuzzer out/ $OLDCORPUS $seedcorpus/ -max_total_time=30

# minimize the corpus
mkdir -p cmin
$fuzzer cmin/ $OLDCORPUS out/ $seedcorpus/ -merge=1

