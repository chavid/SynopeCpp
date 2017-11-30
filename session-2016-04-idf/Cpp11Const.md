# [Extension des expressions constantes](TheorieGenerique "wikilink")

## Les expressions constantes de C++03

Les littéraux entiers tels que 42 sont des expressions constantes. Le sont aussi des des expressions arithmétiques simples telles que 23\*2-4. On peut également y inclure des variables constantes entières, si elles sont elle-même initialisées avec une expression constante.

``` cpp
const int i=23;
const int two_i=i*2;
const int four=4;
const int forty_two=two_i-four;
```

Certaines situations du C++ requièrent une expression constante :

  - la taille d'un tableau statique,

<!-- end list -->

``` cpp
int bounds=99;
int array[bounds];    // Error bounds is not a constant expression 
const int bounds2=99;
int array2[bounds2];  // OK, bounds2 is a constant expression
```

  - initialiser un paramètre de patron qui n'est pas un type,

<!-- end list -->

``` cpp
template<unsigned size>
struct test
{};
test<bounds> ia;   // Error bounds is not a constant expression 
test<bounds2> ia2; // OK, bounds2 is a constant expression
```

  - initialiser un membre constant statique au sein d'une classe,

<!-- end list -->

``` cpp
class X
{
  static const int the_answer=forty_two;
};
```

  - initialiser statiquement une variable globale de type prédéfini ou aggrégat.

<!-- end list -->

``` cpp
struct my_aggregate
{
  int a;
  int b;
};
static my_aggregate ma1={forty_two,123}; // Static initialization
int dummy=257;
static my_aggregate ma2={dummy,dummy};   // Dynamic initialization
```

Ces initialisations étant réalisées à la compilation, elles permettent d'échapper au problème de l'ordre d'initialisation des variables globales en début d'exécution de programme.

## Motivation : précalculer tout ce qui peut l'être à la compilation

Le C++ moderne a largement étendu ce qui constitue des expressions constantes, avec l'introduction du mot clé **`constexpr`**, qui signifie grosso modo "constante précalculée à la compilation". On peut par exemple à présent utiliser les types prédéfinis flottants :

``` cpp
constexpr double pi = 3.1415926535897 ;

class X
 {
  static constexpr double deux_pi__ = 2*pi ;
 } ; 
```

## Fonctions pouvant intervenir dans le précalcul

Si les arguments et le résultat d'une fonction remplissent certaines conditions, et que le corps de la fonction est suffisamment simple, elle peut être déclarée `constexpr` et servir à précalculer des expressions constantes.

``` cpp
constexpr int square(int x)
{
  return x*x;
}

int array[square(5)];
```

Le fait qu'une qu'une fonction est déclarée `constexpr` ne garantis pas qu'elle produit des expressions constantes. Elle peut également être appelée avec des arguments qui ne sont pas des expressions constantes, auquel cas elle renvoie un résultat ordinaire. Ci-dessous, l'appel à `square` est parfaitement valide, mais le résultat ne peut pas servir comme taille d'un tableau.

``` cpp
int dummy=4;
int array[square(dummy)]; // (1) Error, dummy is not a constant expression
```

## Classes pouvant intervenir dans le précalcul

Non seulement nous pouvons à présent utiliser des types flottants, mais en fait nous pouvons utiliser tous les types dits "littéraux", c'est à dire à peu près tous les types prédéfinis, ainsi que toute classe qui répond aux exigences suivantes :

  - avoir un destructeur trivial, c'est à dire non virtuel et qui utilise l'implémentation par défaut,
  - avoir un constructeur par defaut trivial, ou au moins un constructeur `constexpr` qui ne soit pas un constructeur par copie ou déplacement,
  - les types de toutes les variables membres non statiques et toutes les classes de base doivent être triviaux.

Concentrons nous d'abord sur la classe CX ci-dessous, dotée d'un constructeur par défaut trivial :

``` cpp
class CX
 {
  public :
    CX() = default ;
    CX(int a, int b) : a_(a),b_(b) {}
    int get_a() const { return a_ ; }
    int get_b() const { return b_ ; }
    int foo() const { return a_+b_ ; }
  private :
    int a_ ;
    int b_ ;
 } ;
```

Notez que le constructeur par defaut a été explicitement déclaré et défini par défaut, afin qu'il soit présent malgré le second constructeur, et afin de préserver sa nature triviale. Ainsi, CX répond aux exigences d'un type littéral et peut servir dans une expression ou une fonction constante :

``` cpp
constexpr CX create_cx()
 {
  return CX() ;
 }
```

C'est à peu prêt tout ce que l'on peut faire, tant que l'on n'a pas également explicité le caractère `constexpr` des autres méthodes :

``` cpp
class CX
 {
  public :
    CX() = default ;
    constexpr CX(int a, int b) : a_(a),b_(b) {}
    constexpr int get_a() const { return a_ ; }
    constexpr int get_b() const { return b_ ; }
    constexpr int foo() const { return a_+b_ ; }
  private :
    int a_ ;
    int b_ ;
 } ;
```

Nous pouvons maintenant écrire des fonctions constantes plus élaborées :

``` cpp
constexpr CX make_cx( int a ) { return CX(a,1) ; }
constexpr CX half_double( CX old ) { return CX(old.get_a()/2,old.get_b()*2) ; }
constexpr int foo_squared( CX val ) { return square(val.foo()) ; }
int array[foo_squared(half_double(make_cx(10)))] ; // 49 elements
```

Cela peut sembler beaucoup d'efforts pour définir une taille de tableau ou une valeur initiale constante. Un des autres bénéfices qui peut justifier cet effort, c'est le fait que le calcul est réalisé à la compilation, ainsi que l'initialisation de la variable qui recoit cette valeur, et on échappe donc aux problème de l'ordre d'initialisation des variables globales au lancement d'un programme.

``` cpp
CX si = half_double(CX(42,19)) ; // Statically initialized
```

Dans un contexte multi-thread, ces initialisations précalculées échappent également aux aléas d'exécution des threads, ce qui est particulièrement important pour des classes telles que std::mutex ou std::atomic\<\>.

## Variables `constexpr`

Vous noterez ci-dessus que la variable `si` n'est pas une variable constante. C'est le caractère `constexpr` de la valeur initiale qui suffit à permettre une initialisation à la compilation.

Cependant, le qualificateur `constexpr` peut également être appliqué à la variable elle-même, ce qui interdit de l'initialiser avec autre chose qu'une expression constante, et accessoirement rend la variable `const`.

``` cpp
constexpr int i=45 ;               // OK 
constexpr std::string s(“hello”) ; // Error, std::string isn’t a literal type
int foo() ;
constexpr int j=foo() ; // Error, foo() isn’t declared constexpr
```

Pour qu'une variable puisse être `constexpr`, elle doit répondre aux exigences suivantes :

  - son type doit être littéral,
  - elle doit être immédiatement construite ou initialisée,
  - les arguments du constructeur ou la valeur initiale ne peut contenir que des valeurs littérales, des variables ou des fonctions `constexpr`,
  - le constructeur utilisé (explicitement ou implicitement) doit répondre aux exigences d'un constructeur `constexpr`, et dans le cas d'un constructeur explicite il doit être explicitement `constexpr`.

## Retour aux exigences sur les fonctions `constexpr`

La description formelle du contenu autorisé d'une fonction `constexpr` est assez effrayante, mais c'est moins compliqué que cela en à l'air. Il s'agit d'exclurer tout ce qui pourrait empêcher son évaluation à la compilation. Tentons une description informelle qui couvre la majorité des situations :

  - tous les types impliqués doivent être littéraux,
  - on peut déclarer des variables locales, de type littéral, en les initialisant,
  - on s'interdit tout accès à une variable extérieure, globale, statique,...
  - on s'interdit de traiter les exceptions,
  - bien sur, on appelle que des fonctions `constexpr`.

Les fonctions constantes sont de pures fonctions sans effet de bord. Pour les méthodes de classes, quelques exigences supplémentaires :

  - elles ne peuvent pas être virtuelles,
  - la classe doit être un type littéral, ainsi que toutes ses classes

`  de base et ses variables membres.`

  - un constructeur `constexpr` doit initialiser toutes ses

`  variables membre et ses classes de base, à l'aide d'expression constantes.`

## Patrons de fonctions et de fonctions membres

Quand il est appliqué à des patrons, le qualificateur `constexpr` est ignoré si l'un des paramètres utilisé pour les arguments ou le retour n'est pas un type littéral. Vous pouvez ainsi écrire des fonctions qui peuvent être `constexpr` ou non, selon les paramètres utilisés.

``` cpp
template<typename T>
constexpr T sum(T a,T b)
{
  return a+b;
}

constexpr int i=sum(3,42);    // OK, sum<int> is constexpr

std::string s=
  sum(std::string("hello"),
      std::string(" world")); // OK, but sum<std::string> isn’t constexpr
```

En dehors de la nature des types donnés en paramètres, tous les autres critères doivent vérifiés, sous peine d'erreur de compilation.

## Les limites de C++11

En C++11, les fonctions `constexpr` ne pouvaient avoir qu'une unique instruction `return` et le corps des constructeurs `constexpr` devait être vide. C'est pour cette raison que vous croiserez des fonctions faisant usage de l'opérateur `?:` pour émuler un `if`, ou s'appelant récursivement afin d'émuler une boucle `for`. Cette gymnastique n'est plus nécessaire à partir de C++14.

ATTENTION : à propos des fonctions membres d'une classe, en C++11 le mot clef `constexpr` impliquait un `const` implicite ; ce n'est plus le cas en C++14.

## Exercice

#### Fichiers à récupérer

  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef11.cpp>
  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef11.ref>

#### Enoncé

  - Précalculez à la compilation le \!FromTo utilisé dans boucle() :

<!-- end list -->

``` cpp
      ...
      constexpr FromTo range(deb,fin+1) ;
      for ( auto bits : range )
       { t->execute(bits) ; }
      ...
```

#### Solution

  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef12.cpp>

## Références

Références :

  - <http://en.cppreference.com/w/cpp/language/constant_expression>
  - <http://en.cppreference.com/w/cpp/concept/LiteralType>
  - <http://www.cprogramming.com/c++11/c++11-compile-time-processing-with-constexpr.html>

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
