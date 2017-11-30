# [Instructions courantes](TheorieClassique.md)

\_\_FORCETOC\_\_

## Définition et format d'une instruction

Un programme est une séquence d'instructions. Chaque instruction se termine par le symbole `";`". Les fins de lignes sont de simples séparateurs, au même titre que les espaces et les tabulations.

``` cpp
  #   include <string>
int nombre ; nombre       = 1
;         std::string           nom;
nom=
"truc" ;
nombre      =          2
;
```

## \!Entrées/Sorties

Les entrées/sorties standard de C++ ne font pas partie du langage lui-même, mais de la bibliothèque standard : il faut inclure le fichier `iostream`. Ensuite on manipule les canaux prédéfinis (`std::cout`, `std::cerr` et `std::cin`) à l'aide des opérateurs d'insertion (`<<`) et d'extraction (`>>`).

``` cpp
#include <iostream>
#include <string>
std::string reponse ;
std::cout << "bonjour" << std::endl ;
std::cout << "ca va ? " ;
std::cin >> reponse ;
```

## Conditions & boucles : if (...) {...} else {...}

L'instruction `if` permet d'éxécuter un bloc d'instructions (entre accolades) lorsqu'une condition (entre parenthèses) est vraie. La partie `else` est optionnelle.

``` cpp
#include <iostream>
#include <string>
std::cout << "ca va ? " ;
std::string rep ;
std::cin >> rep ;
if (rep=="oui")
 { std::cout << "tant mieux" << std::endl ; }
else
 {
  std::cout << "c'est pas grave" << std::endl ;
  std::cout << "ca ira mieux demain" << std::endl ;
 }
```

## Conditions & boucles : while (...) {...}

L'instruction `while` permet d'éxécuter un bloc d'instructions en boucle tant qu'une condition est vraie. La condition est vérifiée **avant** chaque itération.

``` cpp
#include <iostream>
#include <string>
std::string rep ;
rep = "oui" ;
while (rep!="non")
 {
  std::cout << "on continue ? " ;
  std::cin >> rep ;
 }
```

## Conditions & boucles : do {...} while (...)

L'instruction `do` permet d'éxécuter un bloc d'instructions en boucle tant qu'une condition est vraie. La condition est vérifiée **après** chaque itération.

``` cpp
#include <iostream>
#include <string>
std::string rep ;
do
 {
  std::cout << "on continue ? " ;
  std::cin >> rep ;
 } while (rep!="non") ;
```

## Conditions & boucles : for (...;...;...) {...}

L'instruction `for` permet d'éxécuter un bloc d'instructions tant qu'une condition est vraie. Une instruction supplémentaire est éxécutée avant la première itération, la condition est vérifiée avant chaque itération, et la troisième instruction est appliquée après chaque itération.

``` cpp
for ( int indice = 0 ; indice < 10 ; indice = indice+1 )
 { std::cout << indice << std::endl ; }
```

## Variables locales

On peut définir une variable locale au sein de l'un des blocs {} définis pour les besoins d'une instruction. La variable est créée en début de bloc, détruite en fin de bloc, et n'est visible et utilisable qu'au sein du bloc.

``` cpp
for ( int i = 0 ; i < 10 ; i = i+1 )
 {
  int j = 10*i ;
  std::cout << j << std::endl ;
 }
```

C'est très pratique et plus lisible de déclarer une variable seulement dans le bloc où elle est utilisée. Cependant, si les performances sont importantes, la variable de grande taille et la boucle exécutée très souvent, on préférera déclarer la variable au niveau supérieur, à l'extérieur du bloc, pour qu'elle ne soit pas sans cesse re-créée et re-détruite.

``` cpp
int j ;
for ( int i = 0 ; i < 10 ; i = i+1 )
 {
  j = 10*i ;
  std::cout << j << std::endl ;
 }
```

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
