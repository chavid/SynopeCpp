#!/usr/bin/env sh
rm -f tp3_heritage_polymorphisme.exe \
&& g++ -Wall -Wextra -Wshadow tp3_heritage_polymorphisme.cpp -o tp3_heritage_polymorphisme.exe \
&& ./tp3_heritage_polymorphisme.exe | tee tp3_heritage_polymorphisme.log
