#!/usr/bin/env sh
rm -f tp4bis_mi_parcours.exe \
&& g++ -Wall -Wextra -Wshadow -c utile.cpp -o utile.o \
&& g++ -Wall -Wextra -Wshadow -c framework.cpp -o framework.o \
&& g++ -Wall -Wextra -Wshadow main.cpp utile.o framework.o -o tp4bis_mi_parcours.exe \
&& ./tp4bis_mi_parcours.exe | tee tp4bis_mi_parcours.log \
&& diff tp4_operateurs_templates.log tp4bis_mi_parcours.log
