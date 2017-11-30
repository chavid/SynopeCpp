# [Diverses nouveautés de la bibliothèque standard](TheorieBonnesPratiquesBiblio.md)

Bon nombre de classes existantes ont été réécrites pour bénéficier de la nouvelle sémantique de déplacement, des nouvelles possibilités d'initialisation, ou des nouvelles expressions constantes. Ci-dessous, une sélection de nouveautés qui n'ont pas déjà été traitées ailleurs.

## Le type `array`

Les tableaux natifs du C++ posent des problèmes liés à l'équivalence entre nom de tableau et pointeur, combinée à l'arithmétique des pointeurs : à partir d'un nom de tableau, on peut pratiquement accéder à n'importe quel emplacement mémoire \! C++11 introduit un nouveau conteneur `array` dont l'objectif est de remplacer ces tbleaux natifs, en offrant plus de sécurité, sans perdre en efficacité.

Un objet de type `array` possède une taille fixe, définie à sa construction, et il est peut être initialisé par une liste d'initialisation :

``` cpp
std::array<int,4> t1 ;             // 4 éléments de type int, initialisés à 0
std::array<int,6> t2 { 2, 4, 6 } ; // 6 éléments de type int, initialisés à 2, 4, 6, 0, 0, 0
```

Quelques méthodes :

  - `begin`, `end`, `size` : itérateurs bidirectionnels usuels.
  - `operator[]`, `at` : la seconde méthode vérifie la validité de l'indice.
  - `data` : retourne un pointeur permettant de traiter le tableau comme un tableau natif.

## Les tuples

Il s'agit d'une généralisation des paires à un nombre quelconque d'éléments.

``` cpp
tuple<int,double,std::string> t1 { 5, 6.8, "chose" }
.....
int n = get<0>(t1) ;  // n vaut 5
get<1>(t1) = 2.3 ;    // remplace le deuxième élément 6.8 par 2.3
auto s = get<2>(t1) ; // s sera de type string et contiendra "chose"
```

Un tuple peut évidemment être affecté à un autre tuple de même type, mais aussi en cas de types différents, si il existe une conversion implicite légale pour chacun des éléments.

## Les tables de hachage

Les conteneurs associatifs de C++03 reposent sur un arbre binaire pour ordonner leurs éléments, ce qui conduit à une efficacité en O(Log N). C++11 introduit de nouveaux conteneurs basés sur une technique de "hachage", qui offre une efficacité en O(1) :

  - `unordered_set` ;
  - `unordered_multiset` ;
  - `unordered_map` ;
  - `unordered_multimap`.

Comme toujours, il n'y a pas de conteneur universellement meilleur, et le bon choix dépend de l'utilisation qui en est faite.

## Les enveloppes de référence

Lors de l'utilisation des patrons, le compilateur n'est pas toujours en mesure de deviner notre intention :

``` cpp
void f( int & n ) { n = 5 ; }

template < typename F, typename T >
void g( F f, T t ) { f(t) ; }

int p = 3 ;
g(f,p) ; // p vaut toujours 3 après l'appel !
```

La fonction `g` étant de la forme `g< void (int &), int >`, son deuxième argument lui sera donc transmis par valeur. La valeur de `p` restera donc inchangée. Avec C++03, on pourrait régler localement le problème en restreignant les possibilités de `g` :

``` cpp
template < typename F, typename T >
void g( F f, T & t ) { f(t) ; }
```

En C++11, on peut forcer `g` à utiliser une transmission par référence :

``` cpp
#include <utility>

void f( int & n ) { n = 5 ; }

template < typename F, typename T >
void g( F f, T t ) { f(t) ; }

int p = 3 ;
g(f,std::ref(p)) ; // p vaut bien 5 après l'appel
```

## Les enveloppes polymorphes de fonctions

Les nouveaux objets de type `function` permettent de représenter n'importe quelle fonction (indépendante, membre ou objet fonction), définie alors uniquement par la nature de ses arguments et de sa valeur de retour :

``` cpp
std::function< int ( double, char ) > fp ;

int f1( double x, char c ) { /* ..... */ }
fp = &f1 ;
int n1 = fp(4.5,'aa) ;

class A
 {
  public :
    int operator()( double x, char c ) { /* ..... */ }
 } ;
A a ;
fp = std::ref(a) ;
int n2 = fp(3.8,'z') ;
```

## Autres nouvelles possibilités de la bibliothèque standard

  - Gestion des "expressions régulières".
  - `forward_list` : liste simplement chaînée dotée d'un itérateur unidirectionnel.
  - Au lieu de la seule fonction aléatoire `rand`, C++11 offre maintenant trois moteurs de génération (`linear_congruential`, `substract_with_carry`, `mersenne_twister`) ainsi que de nombreuses distributions : uniforme, bernouilli, géométrique, poisson, binomiale, uniforme, exponentielle, normale et gamma.
  - De nouvelles possibilités de métaprogrammation sont apportées par le type `traits`, qui permet de retrouver certaines caractéristiques des types passés en paramètres, de tester la présence de certaines fonctions membres stratégiques (constructeur de recopie, de déplacement...), et d'instancier de façon conditionnelle les patrons.

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
