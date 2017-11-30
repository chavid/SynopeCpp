#!/usr/bin/env sh
  rm -f tp7_exceptions.exe \
&& g++ -Wall -Wextra -Wshadow tp7_exceptions.cpp -o tp7_exceptions.exe \
&& ./tp7_exceptions.exe | tee tp7_exceptions.log \
&& diff tp6_static_const.log tp7_exceptions.log
