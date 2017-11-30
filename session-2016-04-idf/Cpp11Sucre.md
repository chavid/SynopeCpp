# [C++11 : petites nouveautés pratiques](TheorieClassique.md)

## Le mot-clé `auto`

Fatigué de taper le type à rallonge des itérateurs, alors que ce type est "évident" ? Vous pouvez maintenant laisser le compilateur déduire le type d'une nouvelle variable à partir du type de sa valeur initiale. A la place de :

``` cpp
std::map<std::vector<int>,std::list<float>> m ;
std::map<std::vector<int>,std::list<float>>::iterator itr = m.begin() ;
```

On peut maintenant écrire :

``` cpp
std::map<std::vector<int>,std::list<float>> m ;
auto itr = m.begin() ;
```

## Instruction `for` généralisée

On dispose à présent d'une boucle `for` applicable directement à toute collection disposant d'un itérateur. A la place de :

``` cpp
int t[200] ;
for ( int i = 0 ; i<200 ; ++i )
 { // traite i }
```

On peut maintenant écrire :

``` cpp
int t[200] ;
for ( int i : t )
 { // traite i }
```

Avec la détermination automatique du type, à la place de :

``` cpp
std::vector<int> v ;
for ( std::vector<int>::iterator itr = v.begin() ; itr = v.end() ; ++itr )
 { // traite i }
```

On peut maintenant écrire :

``` cpp
std::vector<int> v ;
for ( auto itr : v )
 { // traite itr }
```

## Enumérations fortement typées

Les enumérations historiques de C++ permettent de définir des constantes qui sont traitées comme des entiers, ce qui induit de nombreux risques d'erreur :

``` cpp
enum color { red, green, blue } ;
enum shape { circle, square, triangle };
 
color c = red ;
bool flag = (c >= triangle); // erreur de type !
```

C++11 permet de déclarer une énumération sous forme de classe, constituant un type à part entière, indépendant des entiers.

``` cpp
enum class Couleur { jaune, rouge, bleu, vert } ;
couleur c ;
.....
c = Couleur::jaune             // OK
c = Couleur::jaune+3 ;         // Interdit (accepté avec un enum ordinaire)
if (c==Couleur::rouge) .....   // OK
if (c==6) .....                // Interdit (accepté avec un enum ordinaire)
if (c>Couleur::rouge) .....    // Interdit, on ne peut pas tout avoir ;)
```

## Nouvelles constantes chaînes

C++11 introduit de nouvelles constantes de type chaîne, basées sur des types caractères étendus. Il existe à présent :

  - `char` : doit maintenant supporter UTF-8 ;
  - `wchar_t` : caractère multi-octets (nombre d'octets variable), existant déjà en C++03 ;
  - `char16_t` : caractère unicode UTF-16 ;
  - `char32_t` : caractère unicode UTF-32.

Les différentes constantes se notent maintenant :

``` cpp
"bonjour"   // tableau constant de char, terminé par 0
L"bonjour"  // tableau constant de wchar_t, terminé par 0
u8"bonjour" // tableau constant de char, codés suivant UTF-8, terminé par 0
u"bonjour"  // tableau constant de char16_t, codés suivant UTF-16, terminé par 0
U"bonjour"  // tableau constant de char32_t, codés suivant UTF-32, terminé par 0
```

## Le pointeur nul

En C++03, la gestion du pointeur nul n'est pas entièrement satisfaisante, du fait qu'il est nécessaire de le représenter à l'aide d'un entier. Cela peut notamment conduire à une difficulté en cas de surdéfinition, comme dans ce cas de figure :

``` cpp
const int NULL = 0 ;
void f(char *) ;
void f(int) ;
//...
f(0) ;    // appel de f(int)
f(NULL) ; // idem
```

On pourrait penser s'en sortir en définissant NULL de cette façon :

``` cpp
const int NULL = (void *)0 ;
```

Mais la conversion de `void *` en `char *` n'est pas une conversion implicite légale. Pour régler le problème, il faudrait écrire :

``` cpp
f((char *)NULL) ; // appel de f(char *)
```

C++11 introduit une notation spécifique aux pointeurs, à savoir `nullptr` (en toute rigueur, il s'agit d'une instance du type `nullptr_t`). Cette valeur peut être convertie implicitement en un pointeur nul de n'importe quel type, ce qui règle le problème de surdéfinition évoqué ci-dessus. Elle peut-être comparée à un pointeur de n'importe quel type. Elle ne peut pas être convertie implicitement en entier ; par contre, elle peut l'être en booléen.

    f(nullptr) ; // appel de f(char *)

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

L'instruction `using` est appelée à remplacer `typedef` dans toutes les circonstances.

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

## Divers

Dans les définitions de patrons, il n'est plus nécessaire de prévoir un espace entre deux symboles "\<" ou "\>".

De façon compatible avec C99, le nouveau type entier `long long int` doit comprendre au moins 64 bits.

L'opérateur `sizeof` pourra s'appliquer directement à un membre d'une classe et non plus seulement à un membre d'un objet.

## Exercice

Fichiers à récupérer

  - <https://llr-training-git.in2p3.fr/chamont/CppNouvellesPratiques/raw/master/Exercices/coef0.cpp>
  - <https://llr-training-git.in2p3.fr/chamont/CppNouvellesPratiques/raw/master/Exercices/coef0.ref>
  - <https://llr-training-git.in2p3.fr/chamont/CppNouvellesPratiques/raw/master/Exercices/coef.sh>

Etapes de l'exercice :

  - Etudiez le fichier `coef_0.cpp`, et vérifiez que `coef_0.sh` vous permet d'enchainer compilation, exécution et comparaison avec la sortie de référence.
  - Essayez d'utiliser `auto` et la nouvelle forme de `for`. Deux suggestions :
      - Créez une classe `FromTo` pour représenter un séquence d'entiers, avec des méthodes `begin()` et `end()` et un itérateur minimaliste qui possède les opérateurs `!=`, `*` et `++`.
      - Transformez l'attribut `max_` de `Boucle` en paramètre template, ce qui permet de faire du tableau de testeurs un tableau statique dont la taille est connue à la compilation.
  - Ce qui pourrait être testé à l'aide d'un `static_assert` :
      - que `max` est un entier supérieur à 0,
      - que le type utilisé avec Coef est bien un type entier (voir std::is\_integral).

Solution

  - <https://llr-training-git.in2p3.fr/chamont/CppNouvellesPratiques/raw/master/Exercices/coef1.cpp>

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
