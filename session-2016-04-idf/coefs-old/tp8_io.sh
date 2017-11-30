#!/usr/bin/env sh
rm -f tp8_io.exe \
&& g++ -Wall -Wextra -Wshadow tp8_io.cpp -o tp8_io.exe \
&& ./tp8_io.exe | tee tp8_io.log \
&& diff tp7_exceptions.log tp8_io.log