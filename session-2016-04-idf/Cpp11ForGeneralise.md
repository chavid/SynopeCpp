# [For généralisé](TheorieFonctionnelleConcurrente.md)

La plupart des boucles d'un programme parcourent tous les éléments d'une collection. Pour simplifier ce cas d'utilisation, C++ 11 introduit une nouvelle syntaxe qui fait office de raccourcit sur la manière basique de parcourir un tableau ordinaire ou une collection de la bibliothèque standard à l'aide d'appels à `begin()` et à `end()`.

Au lieu de :

``` cpp
#include <iostream>
#include <list>

int main()
 {
  int tab[3] { 1, 2, 3 } ;
  std::list<int> liste { 1, 2, 3 } ;
  
  for ( int i=0 ; i<3 ; ++i )
   { std::cout << tab[i] ; }
  std::cout << std::endl ;
  
  for ( auto itr=liste.begin() ; itr!=liste.end() ; ++itr )
   { std::cout << *itr ; }
  std::cout << std::endl ;
  
  return 0 ;
 }
```

on pourra écrire :

``` cpp
#include <iostream>
#include <list>

int main()
 {
  int tab[3] { 1, 2, 3 } ;
  std::list<int> liste { 1, 2, 3 } ;
  
  for ( auto val : tab )
   { std::cout << val ; }
  std::cout << std::endl ;
  
  for ( auto val : liste )
   { std::cout << val ; }
  std::cout << std::endl ;
  
  return 0 ;
 }
```

Cette syntaxe fonctionnera également avec vos propres classes, si elles répondent à l'interface des collections standards : présence d'une méthode `begin()` qui renvoit un itérateur unidirectionnel pointant en début de collection, et présence d'une méthode `end()` qui renvoit un itérateur pointant après le dernier élément.

Si on cherche à modifier les éléments de la collection, on veillera à déclarer la valeur à gauche du `:` en tant que référence :

``` cpp
#include <iostream>
#include <list>

int main()
 {
  std::list<int> liste { 1, 2, 3 } ;
  
  for ( auto &  val : liste )
   { val++ ; }
  
  for ( auto val : liste )
   { std::cout << val ; }
  std::cout << std::endl ;
  
  return 0 ;
 }
```

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
