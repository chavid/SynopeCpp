# [Pointeurs automatiques (std::auto\_ptr)](TheorieBonnesPratiquesBiblio "wikilink")

## Les problèmes des pointeurs ordinaires

Nous connaissons les problèmes posés par l'utilisation des pointeurs ordinaires (raw) et d'objets alloués sur le tas :

  - la déclaration d'un pointeur ne précise pas si il désigne un objet unique ou un tableau,
  - la déclaration d'un pointeur ne précise pas si il désigne un objet de la pile ou du tas,
  - on ne sait pas si le pointeur est "propriétaire" de l'objet pointé,

`  c'est à dire si il a en charge sa destruction,`

  - si il faut détruire l'objet pointé, on ne sait pas si il faut un

`  simple delete ou une procédure plus complexe, et si il s'agit`  
`  d'un objet unique ou d'un tableau,`

  - il est difficile de garantir la destruction des objets du tas en

`  présence d'exceptions,`

  - il est parfois difficile de garantir que la destruction n'est faite

`  qu'une seule fois, indépendamment du chemin emprunté par le programme`  
`  à l'exécution,`

  - il est impossible de tester si le pointeur référence un objet existant

`  ou déjà détruit.`

Comme toute ressource dynamique, la gestion saine de la zone mémoire appelée "tas" repose sur l'idiome RAII, c'est à dire sur la création de "gardes". Dans le cas particulier du tas, ces gardes sont des objets qui se comportent comme des pointeurs (support des opérateurs `*` et `->`) mais qui rendent des services supplémentaires, en particulier la destruction automatique des objets pointés. On parle alors de pointeurs améliorés (smart).

La pratique est tellement courante qu'elle a rapidement fait l'objet d'implémentations standardisées, livrées avec la bibliothèque standard.

## `std::auto_ptr`

Il s'agit de la première implémentation, en C++03, d'un pointeur qui "possède" l'objet pointé, c'est à dire que lorsque le pointeur est détruit, l'objet pointé est détruit également.

``` cpp
#include <string>
#include <iostream>
#include <memory>

void hello()
 {
  auto str = new std::string("hello") ;
  std::auto_ptr<std::string> ptr(str) ;
  std::cout << *ptr << std::endl ;
  *ptr += " world !" ;
  std::cout << *ptr << std::endl ;
 }

int main()
 {
  // ...
  hello() ;
  // ...
  hello() ;
  // ...
  return 0 ;
 }
```

## Limites

Ce pointeur souffre néanmoins d'un défaut majeur : pour éviter les doubles destructions, en cas de copie, le pointeur original est modifié pour être vide, ce qui n'est pas conforme à l'idée que l'on se fait d'une copie. Une des conséquences, c'est qu'on ne peut pas stocker des `auto_ptr` dans les conteneurs de la bibliothèque standard, car ces derniers requièrent des objets qui ont un comportement de copie normal (en C++03).

``` cpp
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

int main()
 {
  typedef std::auto_ptr<std::string> string_ptr ;
  typedef std::vector<string_ptr> string_vec ;

  string_vec mots ;
  mots.push_back(string_ptr(new std::string("world"))) ; // NE COMPILE PAS EN C++03 !
  mots.push_back(string_ptr(new std::string("hello"))) ; // NE COMPILE PAS EN C++03 !
  std::reverse(mots.begin(),mots.end()) ;

  string_vec::const_iterator mot ;
  for ( mot = mots.begin() ; mot != mots.end() ; ++mot )
   { std::cout<<(**mot)<<std::endl ; }
   
  return 0 ;
 }
```

Le contournement de ces problèmes a longuement agité la communauté C++, jusqu'à l'introduction d'un ajout déterminant à la syntaxe du langage, les "rvalue references", que nous appelerons "références temporaires" par abus de langage.

Cette nouveauté, malheureusement un peu complexe, a ouvert la voie à une nouvelle famille de pointeurs améliorés complètement compatibles avec les conteneurs : depuis C++11, on n'utilise plus les `auto_ptr`, déclaré "deprecated", même si ces derniers ont été améliorés pour ne plus souffrir des problèmes qu'ils avaient en C++03.

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
