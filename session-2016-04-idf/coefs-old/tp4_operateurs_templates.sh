#!/usr/bin/env sh
rm -f tp4_operateurs_templates.exe \
&& g++ -Wall -Wextra -Wshadow tp4_operateurs_templates.cpp -o tp4_operateurs_templates.exe \
&& ./tp4_operateurs_templates.exe | tee tp4_operateurs_templates.log
