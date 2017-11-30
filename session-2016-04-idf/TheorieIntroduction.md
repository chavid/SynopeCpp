# [Introduction](README.md)

## Auteurs

Cours préparé par David Chamont (LAL) et Vincent Rouvreau (INRIA), membres du réseau régional [LoOPS](http://reseau-loops.github.io/), dans le cadre d'une série de formations Python & C++ soutenue par le réseau national [Devlog](http://devlog.cnrs.fr/).

  - [Why learn C++ in 2016 ?](http://itscompiling.eu/2016/03/10/why-learn-cpp-2016/)

## Plan par demi-journées

1.  Programmation procédurale

2\. Programmation orientée objet 3. Exceptions & opérateurs 4. Programmation générique 5. Bonnes pratiques & bibliothèque standard 6. Programmation fonctionnelle & concurrente

## C++98/03 ou C++11/14 ?

A partie de 2011, le langage C++ a reçu de nombreuses améliorations et extensions, qui en feraient presque un nouveau langage. Ces nouveautés sont chaque jour mieux supportées par les compilateurs. Pour un débutant, il est donc impératif de partir sur de bonnes bases en apprenant ce C++ "moderne".

Pour autant, les nouveautés ne sont pas toutes matures ni unanimement adoptées, et il y a une palanquée de lignes de code déjà écrites, qui ne changeront pas en un jour. Vous devez donc aussi connaître le C++ "historique", parce que vous le croiserez partout, et parce qu'il aide à comprendre la genèse et le fonctionnement du C++ "moderne".

## Quel environnement de développement ?

En ce début d'année 2016, nous vous recommandons de vous intéresser à [Visual Studio Code](https://code.visualstudio.com/docs/languages/cpp). Contrairement à ce que pourrait laisser entendre les mots "Studio" et "Microsoft", il s'agit d'un produit qui se veut ouvert, portable et léger. Et il est assez rassurant de voir qu'il est contruit autour de technologies de dernière génération. Le support du debugging est encore limité à trop peu de plateformes, mais la communauté semble très active.

## Support du langage C++11/14 par les compilateurs (sans parler de la bibliothèque standard)

#### Clang

Support complet de la syntaxe :

  - **C++11** (ou 0x) : Clang \>= **3.3** ; utilisez l'option **`-std=c++11`**.
  - **C++14** (ou 1y) : Clang \>= **3.4** ; utilisez l'option **`-std=c++14`** pour Clang \> 3.4, `-std=c++1y` pour Clang \<= 3.4.

Détails des fonctionnalités :

  - [C++ Support in Clang](http://clang.llvm.org/cxx_status.html)

#### GCC

Il faut utiliser des options de compilation spécifiques :

  - à partir de la version 4.3, utilisez l'option `-std=c++0x`.
  - à partir de la version 4.7, utilisez l'option **`-std=c++11`**.
  - à partir de la version 4.8, utilisez l'option **`-std=c++14`**.

Support complet de la syntaxe :

  - **C++11** (ou 0x) : Gcc **4.81** ?
  - **C++14** (ou 1y) : Gcc **5** ?

Détails des fonctionnalités :

  - [Support de C++11 par GCC](https://gcc.gnu.org/projects/cxx0x.html)
  - [Support de C++14 par GCC](https://gcc.gnu.org/projects/cxx1y.html)

#### Visual C++

Détails des fonctionnalités :

  - [Support de C++11/14/17 par Visual C++](https://msdn.microsoft.com/fr-fr/library/hh567368.aspxv)

#### iCC

Détails des fonctionnalités :

  - [Support de C++11 par iCC](https://software.intel.com/en-us/articles/c0x-features-supported-by-intel-c-compiler)
  - [Support de C++14 par iCC](https://software.intel.com/en-us/articles/c14-features-supported-by-intel-c-compiler)

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
