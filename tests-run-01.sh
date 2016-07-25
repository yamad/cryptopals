#!/usr/bin/env bash

source "tests-util.sh"

run-tests "cc-01"
run-tests "cc-02"
run-tests "cc-03"
run-tests "cc-04"
run-tests "cc-05" "command-input"
