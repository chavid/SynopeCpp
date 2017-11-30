#!/usr/bin/env sh
rm -f tp6_static_const.exe \
&& g++ -Wall -Wextra -Wshadow tp6_static_const.cpp -o tp6_static_const.exe \
&& ./tp6_static_const.exe | tee tp6_static_const.log \
&& diff tp5_pointeurs.log tp6_static_const.log