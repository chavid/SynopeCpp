# [Fonctions](TheorieClassique.md)

\_\_FORCETOC\_\_

## Définition de fonction

Pour être utilisable dans une instruction C++, une fonction doit être **préalablement définie**. Cette définition comprend le nom et le type des arguments, le type de la valeur de retour et le bloc d'instructions qui composent la fonction.

``` cpp
#include <iostream>

bool affiche_division( int arg1, int arg2 )
 {
  if (arg2==0)
   { return false ; }
  else
   {
    int division ;
    division = arg1/arg2 ;
    std::cout
     << arg1 << " / " << arg2
     << " => " << division
     << std::endl ;
    return true ;
   }
 }

...
if (affiche_division(12,3)==false)
 { std::cerr << "échec" << std::endl ; }
if (affiche_division(6,0)==false)
 { std::cerr << "échec" << std::endl ; }
if (affiche_division(15,6)==false)
 { std::cerr << "échec" << std::endl ; }
...
```

## Fonction main

Tout programme C++ doit comprendre une et une seule fonction `main`. Son type de retour est `int` et correspond à la valeur retournée au système d'exploitation lorsque l'éxécution du programme se termine.

``` cpp
#include <iostream>

bool affiche_division( int arg1, int arg2 )
 {
  if (arg2==0)
   {
    std::cout << arg1 << " / " << arg2 << " => division par 0 !" << std::endl ;
    return false ;
   }
  else
   {
    std::cout << arg1 << " / " << arg2 << " => " << (arg1/arg2) << std::endl ;
    return true ;
   }
 }

int main()
 {
  if (affiche_division(12,3)==false) { return 1 ; }
  if (affiche_division(6,0)==false) { return 2 ; }
  if (affiche_division(15,6)==false) { return 3 ; }
  return 0 ;
 }
```

Une deuxième façon d'écrire la fonction `main(...)` permet de récupérer ce que l'utilisateur a tapé sur sa ligne de commande en lançant l'application :

  - `argc` désigne le nombre de mots tapés par l'utilisateur.
  - `argv` contient ces mots, sous la forme d'un tableau de chaîne de caractères.

<!-- end list -->

``` cpp
#include <iostream>

int main (int argc, char *argv[])
 {
  int i ;
  for ( i=0 ; i<argc ; i++ )
   { std::cout << argv[i] << std::endl ; }
 }
```

Si on sait que l'un des mots est supposé être un nombre entier ou flottant, on peut utiliser les fonctions C `atoi()` ou `atof()` pour transformer le mot en nombre.

``` cpp
#include <iostream>
#include <cstdio> 

int main (int argc, char *argv[])
 {
  if (argc!=3)
   {
    std::cout << "Mauvais nombre d'arguments : "<< argc <<std::endl ;
    return 1 ;
   }
  else
   {
    double d1 = atof(argv[1]) ;
    double d2 = atof(argv[2]) ;
    std::cout << "Somme   : " << (d1+d2) << std::endl ;
    std::cout << "Produit : " << (d1*d2) << std::endl ;
    return 0 ;
   }
 }
```

On notera en passant que pour toutes les fonctions issues du langage C, il faut une forme modifiée de fichier d'entête quand on est dans un programme C++. Par exemples, pour utiliser `atof()`, on inclut le fichier `<stdio.h>` en C, et <cstdio> en C++.

## Type de retour

Il n'y a pas de différence syntaxique entre fonction et procédure. Les fonctions qui n'ont pas vocation à retourner un résultat sont définies avec le type de retour `void`. On peut aussi appeler n'importe quelle fonction sans se soucier de son résultat.

``` cpp
#include <iostream>

void affiche_multiplication( int arg1, int arg2 )
 {
  std::cout
    << arg1 << " * " << arg2 
    << " => " << (arg1*arg2) 
    << std::endl ;
 }

bool affiche_division( int arg1, int arg2 )
 {
  if (arg2==0)
   {
    std::cerr
      << arg1 << " / " << arg2 
      << " => division par 0 !" 
      << std::endl ;
    return false ;
   }
  else
   {
    std::cout << arg1 << " / " << arg2 << " => " << (arg1/arg2) << std::endl ;
    return true ;
   }
 }

int main()
 {
  affiche_multiplication(12,3) ;
  affiche_multiplication(6,0) ;
  affiche_multiplication(15,6) ;

  affiche_division(12,3) ;
  affiche_division(6,0) ;
  affiche_division(15,6) ;

  return 0 ;
 }
```

## Passage d'arguments par valeur

Lors de l'éxécution d'un programme, chaque appel de fonction donne lieu à la création de variables temporaires auxquelles on affecte les valeurs données en arguments.

``` cpp
void incremente( int i )
 { i = i+1 ; }

int main()
 {                 
  int a ;
  a = 0 ;
  
  incremente(a) ;
  // int i = a ;
  // i = i+1 ;

  std::cout << a << std::endl ;
  return 0 ;
 }
```

## Passage d'arguments par référence

Lorsque l'argument d'une fonction est une référence, chaque appel de fonction donne lieu à la création d'une référence temporaire qui désigne la variable donnée en argument.

``` cpp
void incremente( int & i )
 { i = i+1 ; }

int main()
 {
  int a ;
  a = 0 ;

  incremente(a) ;
  // int & i = a ;
  // i = i + 1 ;

  std::cout << a << std::endl ;
  return 0 ;
 }
```

## Passage d'arguments par pointeur

Lorsque l'argument d'une fonction est un pointeur, chaque appel de fonction donne lieu à la création d'un pointeur temporaire auquel on affecte l'adresse donnée en argument. En utilisant ensuite l'opérateur \`\*\`, on accède ainsi à la variable d'origine, et non à une copie.

``` cpp
void incremente( int * pi )
 { *pi = *pi+1 ; }

int main()
 {
  int a ;
  a = 0 ;

  incremente(&a) ;
  // int * pi = &a ;
  // *pi = *pi + 1 ;

  std::cout << a << std::endl ;
  return 0 ;
 }
```

## Passage d'arguments constants

Un argument de fonction peut être constant. Dans le cas d'un passage d'argument par valeur, cela n'a guère d'intérêt, puisque la valeur est dupliquée, à part de protéger le programmeur d'une modification fortuite, s'il sait que les instructions de la fonction ne sont pas supposés modifier cette valeur.

Un usage plus courant consiste à protéger un argument passé par référence ou par pointeur. On fait ainsi l'économie d'une copie tout en se protégeant d'une modification accidentelle, ce qui peut être intéressant si la valeur échangée est de très grande taille.

``` cpp
#include <iostream>

void affiche_tableau( int taille, int const * tableau )
 {
  int i ;
  for ( i=0 ; i<taille ; i++ )
   { std::cout << tableau[i] << std::endl ; }           
 }

int main()
 {
  int t[4] = { 1, 2, 3, 4 } ;
  affiche_tableau(4,t) ;
  return 0 ;
 }
```

## Surcharge de fonctions

Plusieurs fonctions peuvent avoir le même nom, à condition d'avoir une signature différente : le nombre d'arguments doit être différent, ou bien le type d'au moins un argument doit être différent. Pour chaque appel de fonction, c'est le compilateur qui choisit la fonction la plus appropriée. Attention : le type de retour ne compte pas.

``` cpp
#include <iostream>
#include <climits>
#include <cfloat>

int division( int arg1, int arg2 )
 {
  if (arg2!=0) { return (arg1/arg2) ; }
  else { return INT_MAX ; }
 }

double division( double arg1, double arg2 )
 {
  if (arg2!=0) { return (arg1/arg2) ; }
  else { return DBL_MAX ; }
 }

int main()
 {
  std::cout << 12 << " / " << 3 << " => " << division(12,3) << std::endl ;
  std::cout << 6 << " / " << 0 << " => " << division(6,0) << std::endl ;
  std::cout << 15 << " / " << 6 << " => " << division(15,6) << std::endl ;
  double res = division(15,6) ;
  std::cout << 15 << " / " << 6 << " => " << res << std::endl ;
  std::cout << 15. << " / " << 6. << " => " << division(15.,6.) << std::endl ;
  return 0 ;
 }

// Affichage :
// 12 / 3 => 4
// 6 / 0 => 2147483647
// 15 / 6 => 2
// 15 / 6 => 2
// 15 / 6 => 2.5
```

## Variables locales

On peut définir une variable locale au sein d'une fonction. Comme pour les arguments de la fonction, la variable est créée au début de chaque appel de la fonction, et détruite lorsqu'on en sort.

``` cpp
void fois_dix_et_affiche( int i )
 {                 // creation et initialisation de i
  int j { 10*i } ; // creation et initialisation de j
  std::cout << j << std::endl ;
 }                 // destruction de i et j
```

La variable n'est visible et utilisable qu'au sein de la fonction qui la définit.

``` cpp
void fois_dix_et_affiche( int i )
 {
  int j { 10*i } ;
  affiche() ;
 }      

void affiche()
 {
  std::cout << j << std::endl ; // erreur de compilation
 }

int main()
 {
  int a { 3 } ;
  fois_dix_et_affiche(a) ;
 }
```

On peut réutiliser le même nom de variable un peu partout, le compilateur utilisera la variable la plus "locale". Attention : on peut aller jusqu'à masquer un argument avec une variable locale, en général par erreur.

``` cpp

int j = 0 ;

void foix_dix( int & i )
 {
  int i { i*10 } ;
 }

void affiche()
 {
  std::cout << j << std::endl ;
 }

void fois_dix_et_affiche( int i )
 {
  foix_dix(i) ;
  int j = i ;
  affiche() ;
 }      

int main()
 {
  int i = 3 ;
  fois_dix_et_affiche(i) ;
  fois_dix_et_affiche(i) ;
 }
```

## Pointeurs de fonction

Les pointeurs peuvent également référencer des fonctions.

Pour déclarer un pointeur de ce type, faites comme si vous déclariez une fonction, faites précéder le nom de fonction par un caractère "\*" et entourez ce nom "étoilé" par des parenthèses.

On initialise ou l'on affecte un tel pointeur en extrayant l'adresse d'une fonction de même signature grâce à l'opérateur "&".

Une fois qu'il a reçu une valeur, le pointeur peut s'utiliser comme une fonction.

``` cpp
#include <iostream>
#include <string>

void f1( void )
 { std::cout << "1" << std::endl ; }

void f2( void )
 { std::cout << "2" << std::endl ; }

int main()
 {
  void (*f)( void ) ;
  f = &f1 ;
  f() ;  
  f = &f2 ;
  f() ;  
  return 0 ;
 }
```

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
