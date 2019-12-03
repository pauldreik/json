# Fuzzing

Boost json has support for fuzzing support. Clang/libFuzzer is used.

## Building the fuzzers
Execute the fuzzing/fuzzer.sh script. You need clang++ installed. The fuzzer script will start fuzzing for a limited time, interrupt it if you wish.

## Running fuzzing
Either modify the fuzzer.sh script, or run it to build the fuzzer and then issue:
```sh
mkdir -p out
./fuzzer out
```

## Rerunning old crashes
Given a test case testcase.json, build the fuzzer and execute it:
```sh
./fuzzer testcase.json
```
