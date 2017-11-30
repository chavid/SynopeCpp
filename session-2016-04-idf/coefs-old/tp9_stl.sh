#!/usr/bin/env sh
rm -f tp9_stl.exe \
&& g++ -Wall -Wextra -Wshadow tp9_stl.cpp -o tp9_stl.exe \
&& ./tp9_stl.exe | tee tp9_stl.log \
&& diff tp8_io.log tp9_stl.log