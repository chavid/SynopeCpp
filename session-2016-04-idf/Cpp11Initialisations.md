# [C++11 : initialisation uniforme ?](TheorieClassique.md)

## Motivation : multiplicité des syntaxes d'initialisation

Il y a mille et une façons d'initialiser les objets, souvent liées à l'historique du langage et à la volonté de rester compatible avec les versions précédentes du langage et avec le C. Cela perturbe le programmeur, nuit à la lisibilité du code, et surtout posent des problèmes dans l'usage des patrons qui ne peuvent pas traiter de façon équivalente les différents types qu'on leur donne en paramètres.

D'une part, entre types prédéfinis et classes, on entretient une ambiguité entre l'usage de parenthèses, façon constructeurs, et l'usage du signe égal, ce qui entretient également l'ambiguité entre initialisation et affectation.

``` cpp
int n = 5     ;
int n(5)      ; // introduit par C++98 pour uniformiser classes et types prédéfinis
class A {...} ;
A a = 5       ;  équivalent à "A a(5)"... mais pas autorisé pour une classe non copiable.
```

D'autre part, pour les types agrégés (tableaux, unions et structures simples), mais seulement pour eux, on peut utiliser les accolades.

``` cpp
struct st { int x, double z } ;
st s1 = { 4, 5.3 } ;
int t[] = { 3, 5, 9 } ;
```

## Nouveauté : initialisation par accolades

C++11 définit une nouvelle forme d'initialisation, à base d'accolades, qu'il s'efforce de rendre utilisable dans d'autres situations : appels de fonction, valeur de retour. Ceci pour tous les types de variables ou d'objets. L'objectif est de disposer d'une **initialisation uniforme**.

Quelques exemples avec des valeurs scalaires :

``` cpp
int n = { 5 } ; // notation dite "initialisation copie"
int n { 5 }   ; // notation dite "initialisation directe"
int f( double x )
 {
  //...
  return { 9 } ;
 }
f({ 5.25 }) ;
```

#### Améliorations

L'usage d'une liste vide, pour une variable scalaire, entraine l'initialisation à la valeur par défaut du type.

``` cpp
int n {} ; // équivaut à int n(0) ;
```

Par ailleurs, l'usage d'accolades interdit le raccourcissement automatique et implicite des valeurs.

``` cpp
double x, y, z ;
.....
int sum1 {x+y+z} ; // error ! sum of doubles may not be expressible as int
```

#### Nouveaux usages pour les aggrégats/tableaux/collections

Pour les aggrégats, l'ancienne notation utilisée pour les initialisations convient. Elle se généralise aux appels de fonction, de définition d'une valeur de retour, d’initialisation de membre d'une classe, ainsi qu'aux tableaux dynamiques.

``` cpp
voif f( const int t[3] ) { ..... }
f( { 3, 5, 7 } ) ;

class C
 {
  int tab[3] ;
  public :
    C() : tab { 3, 6, 9 } { ..... }
 } ;

int * tab = new int[5] { 1, 2, 3, 4, 5 } ;
```

Les objets peuvent eux aussi être construits à l'aide des nouvelles listes d'initialisation :

``` cpp
class Point
 {
  public :
    Point( float x, float y ) : x_ {x}, y_ {y} {}
  private :
    float x_, y_ ;
 } ;

Point p1 = { 1.1, 2.2 } ; // notation dite "initialisation copie"
Point p2 { 3.3, 4.4 }   ; // notation dite "initialisation directe"
```

## Nouveauté : le type `std::initializer_list`<T>

Ce type permet de manipuler des listes d'initialisations formées d'un nombre quelconque d'expressions constantes d'un type implicitement compatible avec un type donné. Il peut être employé comme argument de fonction, et ses différents éléments sont accessibles à l'aide de méthodes classiques telles que `begin()`, `end()`, `size()`.

``` cpp
class A
 {
   A( std::initializer_list<double> l ) { ..... }
 } ;

A a1 { 2.5, 3.4 } ;  // OK : 2 valeurs de type double
A a2 { 1, 3, 8.5 } ; // OK : 3 valeurs d'un type compatible avec double
```

## Résolution de la surdéfinition

Les constructeurs avec liste d'initialisation, quand ils sont définis, sont prioritaires sur les autres. Dans l'exemple artificiel ci-dessous, le deuxième constructeur n'est jamais utilisé.

``` cpp
class A
 {
   A( std::initializer_list<double> l ) { ..... } // constructeur 1
   A( int n, double y ) { ..... }                 // constructeur 2
   A( std::string s ) { ..... }                   // constructeur 3
 } ;

A a1 { 1, 5.6, 2, 12, 99 } ;  // utilise constructeur 1
A a2 { 6, 12 } ;              // utilise constructeur 1
A a3 { "bonjour" } ;          // utilise constructeur 3
```

## Piège

Tous les conteneurs de la bibliothèque standard disposent dorénavant d'un constructeur à base de `std::initializer_list`<T>.

``` cpp
std::vector<int> v1 { 1, 4, 9, 3, 7 } ;
std::map<int, std::string> m1 { { 1, "bonjour" }, { 2, "salut" }, { 3, "hello" } } ;
```

De ce fait, pour disposer de certains constructeurs en esquivant le constructeur prioritaire, il faudra parfois recourir à l'ancienne notation :

``` cpp
std::vector<int> v1 (100, 1) ; // vecteur de 100 éléments initialisés à 1
std::vector<int> v1 {100, 1} ; // vecteur de 2 éléments initialisés à 100 et 1
```

## Débat

On voit ci-dessus que l'initialisation à base d'accolades n'élimine pas totalement le besoin des autres formes historiques. Et toutes les formes possèdent des nuances subtiles, comme l'initialisation ou non à 0 d'un type numérique prédéfini. En conséquence, on peut se demander si l'objectif initial de simplification a bien été atteint...

Cependant, on ne peut que se réjouir de certaines nouvelles possibilités, telle que l'initialisation directe d'un vecteur avec ses valeurs listées entre accolades.

## Exercice

Fichiers à récupérer

  - <https://llr-training-git.in2p3.fr/chamont/CppNouvellesPratiques/raw/master/Exercices/coef1.cpp>

Essayez d'utiliser l'initialisation par accolades dans le programme `coef...cpp`. Elle peut notamment s'utiliser pour l'initialisation des attributs des obejts dans la zone d'initialisation des constructeurs. Cela devrait vous amener à détecter quelques incohérences de types dans le code.

Solution

  - <https://llr-training-git.in2p3.fr/chamont/CppNouvellesPratiques/raw/master/Exercices/coef2.cpp>

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
