#!/usr/bin/env sh
rm -f tp5_pointeurs.exe \
&& g++ -Wall -Wextra -Wshadow tp5_pointeurs.cpp -o tp5_pointeurs.exe \
&& ./tp5_pointeurs.exe | tee tp5_pointeurs.log \
&& diff tp4_operateurs_templates.log tp5_pointeurs.log
