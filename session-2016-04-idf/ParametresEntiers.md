# [Paramètres entiers](TheorieGenerique.md)

\_\_FORCETOC\_\_

## Mécanique de base

Parmi les paramètres d'un patron, on peut non seulement introduire des types, à l'aide du mot clé `typename` (ou anciennement `class`) mais également des valeurs entières, à condition qu'elles puissent être évaluée dès le stade de compilation.

Une utilisation typique est de se servir de cette valeur pour définir un tableau de taille fixe :

``` cpp
#include <iostream>
#include <cmath>

template <typename Type, int SIZE>
class Vecteur
 {
  public :
    Vecteur( Type norme )
     {
      double v = sqrt(norme*norme/SIZE) ;
      for ( int i=0 ; i<SIZE ; ++i ) v_[i] = v ; 
     }
    Type norme()
     {
      double res {} ;
      for ( int i=0 ; i<SIZE ; ++i ) res += v_*v_ ; 
      return sqrt(res) ;
     }
  private :
    Type v_[SIZE] ; 
 } ; 

int main ()
 {
  Vecteur<float,3> v {5} ;
  std::cout<<"norme : "<<v.norme()<<std::endl ;
  return 0 ; 
 }
```

## Métaprogrammation

En combinant l'usage des paramètres entiers, la récursivité et la spécialisation, on peut utiliser le compilateur pour précalculer certaines valeurs dès le stade de la compilation. Ci-dessous, l'exemple classique du calcul de factorielles. Les possibilités de cette **métaprogrammation** sont étonnantes, mais demandent une expertise importante.

``` cpp
#include<iostream>

template<int n>
struct Factorial
 {
  static const int value = n * Factorial<n-1>::value ;
 } ;

template<>
struct Factorial<0>
 {
  static const int value = 1 ;
 } ;

int main()
 {
  std::cout << Factorial<5>::value << std::endl ;  // prints 120
  std::cout << Factorial<10>::value << std::endl ; // prints 3628800
  return 0 ;
 }
```

## Assertions statiques

Depuis C++11, la nouvelle directive `static_assert` permet de verifier une condition **lors de la compilation** et d'afficher le message de son choix en cas d'échec. Elle est typiquement utilisée pour tester un type ou un entier utilisé dans un patron :

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

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
