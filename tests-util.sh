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
    echo "  OK"
}

# pass name of binary to test. must have a file called
# tests/<name>-tests.txt in which test inputs and expected outputs are
# listed on subsequent lines. if the second argument is
# "command-input", then the test input line is a shell command
run-tests()
{
    echo "Run tests for ${1}:"
    cat "test/${1}-tests.txt" |
        while { read -r input; read -r expected; }
        do
            if [ "$2" == "command-input" ]
            then
                actual=$(eval ${input})
            else
                actual=`build/bin/${1} ${input}`
            fi
            ensure "${actual}" "${expected}"
        done
}
