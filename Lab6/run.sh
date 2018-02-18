#!/bin/bash
if clang++ -std=c++11 CLI.cc
then
    ./a.out
	rm a.out
fi
