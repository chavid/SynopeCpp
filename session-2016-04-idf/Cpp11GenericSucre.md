# [Petites nouveautés diverses](TheorieGenerique "wikilink")

## \<\< et \>\>

Dans les définitions de patrons, il n'est plus nécessaire de prévoir un espace entre deux symboles "\<" ou "\>".

## Assertions statiques

La nouvelle directive `static_assert` permet de verifier une condition **lors de la compilation** et d'afficher le message de son choix en cas d'échec. Elle est typiquement utilisée pour tester un type utilisé dans un patron :

``` cpp
template <typename T> .....
static_assert(sizeof(T)>=8,"Type de taille insuffisante\n") ;
```

Un autre exemple, indépendant des patrons :

``` cpp
const int n = 5 ;
.....
static_assert(n>0,"Dimension incorrecte pour le tableau t\n") ;
double t[n] ;
```

## Synonymes de patrons

Il devient possible de définir un synonyme pour une spécialisation partielle d'un patron de classes, en utilisant une syntaxe basée sur `using` :

``` cpp
template <typename T, typename U> class Paire { public : T x ; U y ; } ;
template <typename U> using PaireInt = Paire<int,U> ;
.....
PaireInt<double> pid ; // équivaut à Paire<int,double>
```

Un autre exemple utilisant la bibliothèque standard :

``` cpp
template <typename T> using MapInt = std::map<int,T> ;
.....
MapInt<double> md ; // équivaut à std::map<int,double>
```

L'instruction `using` est également capable de remplacer les anciens usages de `typedef` appelé à disparaitree :

``` cpp
using Integer = int;
// même chose que : typedef int Integer;

using FunPtr = int* (*)(int*) ;
// même chose que : typedef int*(*FunPtr) (int*);
```

## Patrons de variables

C++14 introduit la possibilité de faire des patrons de variables. L'exemple habituellement avancé est celui d'une variable constante pi que l'on pourrait décliner avec des précision différentes pour tous les types prédéfinis.

``` cpp
#include <iostream>
#include <iomanip>

template<class T>
const T pi = T(3.1415926535897932385) ;
 
template<class T>
T circular_area(T r)
 {
  return pi<T> * r * r ;
 }

int main()
 {
  std::cout<<"double : "<<std::setprecision(15)<<circular_area<double>(1)<<std::endl ;
  std::cout<<"float  : "<<std::setprecision(15)<<circular_area<float>(1)<<std::endl ;
  std::cout<<"int    : "<<std::setprecision(15)<<circular_area<int>(1)<<std::endl ;
 }
```

En C++17, dans la bibliothèque standard, on utilisera les patrons de variables pour faire des raccourcis "\_v" pour les patrons qui servent à faire des tests sur les types. Par exemple :

``` cpp
template< class T, class U >
constexpr bool is_same_v = is_same<T, U>::value ;
```

Références :

  - <http://en.cppreference.com/w/cpp/language/variable_template>

## Aide à l'instanciation de patrons

L'usage intensif de patrons conduit à l'effet appelé "code bloat" : l'explosion invisible du nombre de types instanciés, sans que le développeur en soit vraiment conscient, et la duplication du code de ces types dans tous les fichiers compilés séparemment, conduisant à des executables de très grande taille. L'élimination de ces doublons dans les éxécutables n'ayant pas vraiment été réglée, C++11 permet au développeur d'aider le compilateur en interdisant ou forcant explicitement l'instanciation d'un patron. Pour l'interdire, on utilisera la directive `extern` :

``` cpp
extern template class Point<int> ; // Point<int> ne sera pas instancié dans le fichier courant
```

Pour obtenir l'instanciation d'un patron, une simple déclaration suffit :

``` cpp
template class Point<int> ; // Point<int> sera instancié dans le fichier courant
```

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
