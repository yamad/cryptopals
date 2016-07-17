#!/usr/bin/env bash

_ntests=0

# simple shell testing from https://github.com/ctz/cryptopals
ensure()
{
    if [ "$1" != "$2" ]; then
        echo "test failure:"
        echo "   '$1'"
        echo "is not equal to"
        echo "   '$2'"
        exit 1
    fi

    _ntests=$(($_ntests + 1))
    echo "  test $_ntests: OK"
}

# pass name of binary to test. must have a file called
# tests/<name>-tests.txt in which test inputs and expected outputs are
# listed on subsequent lines
run-tests()
{
    echo "Run tests for ${1}:"
    cat "tests/${1}-tests.txt" |
        while { read -r input; read -r expected; }
    do
        actual=`bin/${1} ${input}`
        ensure ${actual} ${expected}
    done
}
