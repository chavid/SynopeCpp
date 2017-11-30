# [Spécialisations de templates](TheorieGenerique.md)

\_\_FORCETOC\_\_

La programmation générique de C++ offre une fonctionnalité dont les possibilités n'ont pas fini de nous étonner (et de nous plonger dans le désarroi) : après avoir défini une classe ou une fonction paramétrée, nous pouvons ajouter des **implémentations différentes** pour des valeurs particulieres de tout ou partie des paramètres : c'est la spécialisation de template.

``` cpp
#include <iostream>
#include <typeinfo>  // typeid()

template<typename TypeEntier>
void affiche( TypeEntier te )
 { std::cout<<typeid(TypeEntier).name()<<": "<<te<<std::endl ; }

template<>
void affiche( char te )
 { std::cout<<typeid(char).name()<<": "<<int(te)<<std::endl ; }

int main()
 {
  affiche(long(8)) ;
  affiche(int(8)) ;
  affiche(short(8)) ;
  affiche(char(8)) ;
 }
```

Dans le cas d'une classe, on peut même changer l'interface de cette dernière (pour le meilleur et pour le pire).

``` cpp
#include <iostream>
#include <typeinfo>  // typeid()

template<typename TypeEntier>
class Afficheur
 {
  public :
    void affiche( TypeEntier te )
     { std::cout<<typeid(TypeEntier).name()<<": "<<te<<std::endl ; }
 } ;

template<>
class Afficheur<char>
 {
  public :
    std::string texte( char te )
     {
      std::string texte { typeid(char).name() } ;
      texte += ": "+std::to_string(int(te)) ;
      return texte ;
     }
 } ;

int main()
 {
  Afficheur<long> afflong ;
  Afficheur<int> afflint ;
  Afficheur<short> affshort ;
  Afficheur<char> affchar ;
  afflong.affiche(8) ;
  afflint.affiche(8) ;
  affshort.affiche(8) ;
  std::cout<<affchar.texte(8) ;
 }
```

## Autres cas de spécialisation totale

Imaginons une fonction template permettant à 2 exécutables d'échanger des messages suivant un protocole particulier, dont voici la fonction générique de construction de message :

``` cpp
#include <iostream>
#include <string>
#include <typeinfo>  // Pour typeid()
#include <cmath>     // M_PI

template <typename Type>
std::string message( const Type & valeur )
 {
  std::string str_valeur = std::to_string(valeur) ;
  std::string res = typeid(valeur).name()
   + "|" + std::to_string(str_valeur.size())
   + "|" + str_valeur ;
  return res ;
 }

int main()
 {
  std::cout << message(unsigned(42)) << std::endl ;
  std::cout << message(double(M_PI)) << std::endl ;
  return 0 ;
 }
```

    #!txt
    j|2|42
    d|8|3.141593

Nous voyons que notre fonction générique se trouve vite limitée par l'utilisation de la méthode [std::to\_string](http://fr.cppreference.com/w/cpp/string/basic_string/to_string) sur la valeur passée en paramètre (suivre le lien pour voir les valeurs supportées).

Imaginons que nous voulions maintenant échanger des messages de type `std::string` suivant le même procédé.

``` cpp
  std::string question = "Quelle est la réponse à la grande question sur la vie, l'univers et le reste ?";
  std::cout << message(question) << std::endl ;
```

Nous allons devoir le spécialiser :

``` cpp
template <>
std::string message( std::string valeur )
 {
  std::string res = "str" + "|" + std::to_string(valeur.size()) + "|" + valeur ;
  return res ;
 }
```

## Complication avec les types imbriqués

La possibilité de spécialiser un patron offre un moyen exceptionnellement souple de traiter les cas particuliers, sans obscurcir le cas général. On peut tout faire dans une spécialisation, y compris revoir totalement l'interface d'un patron de classe. Les variantes spécialisées n'ont pas besoin d'être regroupées avec le cas général, et peuvent surgir "par surprise" au détour d'un `#include`.

En conséquence, il y a des situations où le compilateur ne veut pas se risquer à faire des hypothèses sur l'interface d'un patron qui n'est pas encore entièrement connu, et il a besoin d'être "accompagné" par le programmeur.

Prenons cet exemple :

``` cpp
template<typename Conteneur>
void inspecte( Conteneur & conteneur )
 {
  Conteneur::Iterateur * x ; // NE COMPILE PAS !
  ...
 }
```

Il peut s'agir d'une déclaration de variable locale `x` de type `C::iterator *`. Mais, si le type `C` dispose d'une variable statique publique nommée `iterator`, il peut aussi s'agir d'une multiplication de `iterator` par `x`... le compilateur n'a aucun moyen de le savoir, tant que `C` n'est pas connu. Pour résoudre cette ambiguité, le compilateur considère par défaut qu'il s'agit d'une variable, à moins que le programmeur lui dise explicitement qu'il s'agit d'un type, à l'aide du mot-clé `typename` :

``` cpp
template<typename Conteneur>
void inspecte( Conteneur & conteneur )
 {
  typename Conteneur::Iterateur * x ;
  ...
 }
```

Cette règle s'applique à tous les noms qui sont imbriqués et qui dépendent d'un paramètre, même indirectement. Prenons un autre exemple :

``` cpp
template<typename Val>
class MonConteneur
 {
  public : 
    class Iterateur
     { ...
 } ;

template<typename Val>
void inspecte( MonConteneur<Val> & conteneur )
 {
  MonConteneur::Iterateur * x ; // NE COMPILE PAS !
  ...
 }
```

Même si `MonConteneur` semble connue, on n'est pas à l'abri d'une spécialisation "sale" qui change la nature de `Iterateur`, et qui s'interposerait avant que la fonction `inspecte()` soit effectivement appelée :

``` cpp
...

template<>
class MonConteneur<int>
 {
  public : 
    static int Iterateur ;
    ...
 } ;

MonConteneur<int> conteneur ;

inspecte(conteneur) ;

...
```

Bref, le mot-clé `typename` doit précéder tous les noms imbriqués et dépendants. Mais seulement ceux-là :

``` cpp
template<typename C>                  // typename autorisé (ou class)
void f( const C & container,          // typename interdit (C n'est pas un nom imbriqué)
        typename C::iterator iter) ;  // typename requis   (iterator est imbriqué et dépendant)
```

La règle comprend deux exceptions. Parce qu'il n'y a pas d'ambiguité possible, il y a deux emplacements où les noms imbriqués et dépendants **doivent** être utilisés **sans** `typename` :

  - la liste des classes de base d'une classe
  - la liste d'initialisation des membres d'un constructeur

<!-- end list -->

``` cpp
template<typename T>
class Derived: public Base<T>::Nested // typename interdit
 {                       
  public:                             
   Derived(int x)
    : Base<T>::Nested(x)              // typename interdit
     {                                
      typename Base<T>::Nested temp ; // typename requis
      .....
 } ;
```

L'utilisation de `std::iterator_traits`<IterT>`::value_type` est la façon standard de récupérer le type des valeurs pointées par l'itérateur, en utilisant une classe de traits. L'instruction ci-dessus permet de créer de créer un objet temporaire et de lui assigner la valeur pointée par l'itérateur. Si `IterT` est `std::list`<std::string>`::iterator`, alors `temp` sera de type `std::string`. Parce que le nom `std::iterator_traits`<IterT>`::value_type` est à la fois imbriqué dans `std::iterator_traits`<IterT> et dépendant de `IterT`, il faut le faire précéder de `typename`.

Comme il est long de taper un tel nom, une pratique commune consiste à créer un alias local de même nom :

    #!cpp
    template<typename IterT>
    void workWithIterator( IterT iter )
     {
      typedef typename std::iterator_traits<IterT>::value_type value_type ;
      value_type temp(*iter) ;
      ...
     }
    </source>

## Complication avec l'héritage

Lorsqu'on hérite d'une classe paramétrée, du fait que cette dernière peut-être spécialisée avec un changement d'interface, le compilateur refuse de tenir pour acquis qu'il disposera toujours de tout ce qui est défini dans la classe de base générale. Reprenons un exemple :

``` cpp
template<typename Val>
class MonConteneur
 {
  public : 
    int taille() { ...
 } ;

template<typename Val>
class MonConteneurEtendu : public MonConteneur<Val>
 {
  public : 
    int taille_etendue()
     { return taille()+ ...  // NE COMPILE PAS !
 } ;
```

Trois moyens sont utilisables pour "rassurer" le compilateur et obtenir l'héritage habituel :

1.  rendre l'attribut visible à l'aide d'une instruction using : `... using MonConteneur`<Val>`::taille ; ...`.
2.  appeler l'attribut à travers le pointeur this : `...return this->taille()...`.
3.  appeler l'attribut avec le nom de classe en préfixe : `...return MonConteneur`<Val>`::taille()...`.

Ce dernier choix n'est pas le meilleur, parce que si la méthode appelée est virtuelle, l'utilisation de cet opérateur inhibe la liaison dynamique. Dans les trois approches, il s'agit de rassurer le compilateur en lui promettant que toutes les éventuelles spécialisations de `MonConteneur` comprendront une méthode `taille()`. Si la promesse n'est pas tenue, une erreur de compilation viendra le sanctionner plus tard.

  
  
© *INRIA & CNRS 2016*  
*Assemblé et rédigé par Vincent Rouvreau et David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
