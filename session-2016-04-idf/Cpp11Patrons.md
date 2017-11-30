# [Patrons à nombre de paramètres variables (patrons variadiques)](TheorieGenerique "wikilink")

## Definition d'un patron de classe variadique

C++11 introduit la possibilité de définir des patrons avec un nombre variable de paramètres. Cette fonctionnalité (patrons variadiques) est très différente de la notion de fonction à nombre d'arguments variables (ellipse), ne serait-ce que parce que cette dernière s'exprime au moment de l'éxécution, tandis que la variabilité des patrons s'exprime au moment de la compilation elle-même.

Du côté de la notation, de même qu'une fonction variadique est déclarée avec une ellipse (...) dans ses arguments, les patrons variadiques contiennent un bloc variable de paramètres, déclaré via "typename...". Pour définir pleinement un patron variadique, on s'appuie sur une spécialisation récursive. Cela nécessite une déclaration générale, une spécialisation récursive et une spécialisation terminale :

``` cpp
template <typename... Types> struct Tuple ;

template <typename T, typename... Types>
struct Tuple<T, Types...>
 {
  T data_ ;
  Tuple<Types...> others_ ;
  Tuple() : data_(), others_() {}
 } ;

template <>
struct Tuple<>
 { Tuple() {} } ;
```

On peut relever plusieurs choses :

  - le patron général non spécialisé n'a pas besoin d'être défini, il ne sert que de support à la spécialization,
  - on peut mélanger dans une même déclaration des paramètres normaux, et un bloc variable de paramètres (parameter pack),
  - pour réutiliser les paramètres d'un bloc, on écrit son nom suivi d'une ellipse (pack expansion).

L'exemple ci-dessus repose sur une imbrication récursive de données. On peut aussi faire appel à l'héritage pour échapper en partie à cette imbrication :

``` cpp
template <typename... Types> class A ;

template <typename T, typename... Types>
class A <T, Types...> : A <Types...>
 {
  T data_ ;
 } ;

template <>
class A <>
 { A() {} } ;
```

## Expansion du bloc de paramètres

La puissance des patrons variadiques est liée à tout ce que l'on peut faire au moment de l'expansion. L'usage le plus basique est de dérouler la liste des paramètres :

``` cpp
template<typename ... Params>
struct dummy
 {
  std::tuple<Params...> data;
 } ;
```

La seule donnée membre est ici une instance de "std::tuple\<\>" comprenant tous les types spécifiés, ainsi la structure "dummy\<int,double,char\>" comprend une variable membre de type "std::tuple\<int,double,char\>". On peut combiner les blocs de paramètres avec des paramètres ordinaires :

``` cpp
template<typename ... Params>
struct dummy2
{
  std::tuple<std::string,Params...> data ;
};
```

Encore plus intéressant, on peut effectuer des transformations sur chacun des paramètres d'un bloc, en décalant simplement l'ellipse à la fin du motif de transformations. Ci-dessous, au lieu d'un tuple des types originaux, on insère un pointeur pour chaque paramètre :

``` cpp
template<typename ... Params>
struct dummy3
 {
  std::tuple<Params* ...> pointers ;
  std::tuple<std::unique_ptr<Params> ...> unique_pointers ;
 } ;
```

Le motif de transformation peut être aussi complexe que souhaité, et s'inscrire au sein d'une expression plus complexe, l'art consistant à bien placer son ellipse. Par exemple si votre bloc contient "int,int,char", alors "std::tuple\<std::pair\<std::unique\_ptr <Params>,double\> ... \>" deviendra

``` cpp
std::tuple<
  std::pair<std::unique_ptr<int>,double>,
  std::pair<std::unique_ptr<int>,double>,
  std::pair<std::unique_ptr<char>,double>
>
```

Si le bloc de paramètres est utilisé dans la liste de paramètre d'un autre patron, il n'est pas nécessaire que ce dernier soit variadique, mais le nombre de paramètres après expansion doit être exactement le bon.

``` cpp
template<typename ... Types>
struct dummy4
{
  std::pair<Types...> data;
};
dummy4<int,char> a;    // OK, data is std::pair<int,char>
dummy4<int> b;         // Error, no second type
dummy4<int,int,int> c; // Error, too many types
```

## Patron de fonction variadique

On peut appliquer les mêmes techniques à une fonction libre ou membre d'une classe, et là encore, l'accès aux différents éléments de la liste passe par une technique de récursivité :

``` cpp
#include <iostream>

template<typename ... Args>
void show_args() ;

template<>
void show_args() {}

template<typename Arg, typename ... Args>
void show_args(Arg arg, Args ... args)
 {
  std::cout<<arg<<std::endl ;
  show_args<Args...>(args...) ;
 }

int main()
 {
  show_args("hello world !",42,3.1416) ;
  return 0 ;
 }
```

On peut aussi ne pas vouloir boucler dans les arguments, mais les transmettre directement à un autre appel de fonction, en réalisant quelques actions auxiliaires avant et après :

    template<typename ... Args>
    void call_show_args(Args ... args)
     {
      std::cout<<"About to show args"<<std::endl ;
      show_args(args...) ;
      std::cout<<"All args now shown"<<std::endl ;
     }

En utilisant des références universelles (T&&) pour les arguments de la fonction appellante, et en passant par "std::forward()", on aboutit au graal du transfert parfait (perfect forwarding) de ces arguments :

``` cpp
#include <memory>

template<typename T,typename ... Args>
std::shared_ptr<T> my_make_shared(Args&& ... args)
 {
  return std::share_ptr<T>(new T(std::forward<Args>(args)...)) ;
 }
```

On notera que dans ce cas, l'expansion dans l'appel à "new T" s'applique simultanément à "Args" et à "args".

Une dernière chose que l'on peut faire avec un bloc de paramètres : évaluer sa taille à l'aide de l'opérateur "sizeof..." . C'est probablement la seule utilisation possible de votre bloc sans le faire suivre de l'ellipse, vu qu'elle est déjà incluse dans le nom de l'opérateur.

``` cpp
template<typename ... Args>
unsigned count_args(Args ... args)
{
  return sizeof... (Args);
}
```

Le résultat de sizeof... est bien entendu une expression constante.

## Références

  - [Stage SM14, Supélec](http://ims.metz.supelec.fr/depot/CoursC++/Support/Cours/patrons/patrons-avances.html)
  - [Ellipses du C](https://www.ljll.math.upmc.fr/courscpp/Sections/Sect05-B7.html)

## Exercice

#### Fichiers à récupérer

  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef12.cpp>

#### Enoncé

1.  Pour préparer le terrain, dans un premier temps, allégeons les constructeurs de nos testeurs en transformant leur titre en variable constexpr de classe, qui est ensuite repassée au constructeur de Testeur :

<!-- end list -->

``` cpp
...
template< typename U >
class TesteurCoef : public Testeur
 {
  public :
  
    static constexpr const char * titre__ {"Test d'un coef"} ;
    TesteurCoef() : Testeur{titre__} {}
...
```

1.  Remplacer la classe Testeurs par un patron variadique \!ListeTesteurs qui contient ses testeurs par valeur, et faites également de boucle() un patron de boucle variadique capable de parcourir cette liste. Côté main(), cela donne :

<!-- end list -->

``` cpp
...
  try
   {
    ListeTesteurs<EchecDepuisStable,EchecDepuisTemporaire> tt1{} ;
    boucle<1,1000>(tt1) ;

    ListeTesteurs<TesteurCoef<unsigned char>,TesteurSomme<short>> tt2{} ;
    boucle<1,8>(tt2) ;

    std::cout<<std::endl ;
    return 0 ;
   }
...
```

1.  Au prix de quelques aménagements, éliminez les classes Testeurs et Testeur : créer une méthode titre() dans vos testeurs, faites de erreur() une fonction libre...
2.  Transformez les tt1 et tt2 de main() en constexpr.

#### Solution

  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef13.cpp>

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
