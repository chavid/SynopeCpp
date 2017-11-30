#!/usr/bin/env sh
rm -f tp2_classes_objets.exe \
&& g++ -Wall -Wextra -Wshadow tp2_classes_objets.cpp -o tp2_classes_objets.exe \
&& ./tp2_classes_objets.exe | tee tp2_classes_objets.log
