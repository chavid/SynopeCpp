# [C++11 : amélioration des fonctionnalités des objets](TheorieObjets.md)

## Amélioration de la construction des objets

### Constructeurs délégués

On peut maintenant demander qu'un constructeur en utilise un autre, en employant la même syntaxe que pour l'appel d'un constructeur d'une classe ascendante :

``` cpp
class A
 {
  public :
    A() : A(0,0)        { indic_ = false ; }  // utilise A(int,int)
    A( int x ) : A(x,0) { indic_ = false ; }  // utilise A(int,int)
    A( int x, int y )   { x_ = x ; y_ = y ; indic_ = true ; }
  private :
    int x_, y_ ;
    bool indic_ ;
 } ;
```

### Initialisation de membres données

C++03 ne permet pas d'initialiser des membres données lors de leur déclaration, à l'exception des membres statiques constants. Cela devient possible pour les membres non statiques avec C++11 :

``` cpp
class A
 {
  .....
  private : 
    int n = 5 ; // interdit en C++03, OK en C++11
 } ;
```

Il s'agit d'une initialisation par défaut qui pourra être éventuellement modifiée par un constructeur.

Seule la notation "nue" ou par accolades est autorisée dans ce contexte : pas d'initialisation par parenthèses.

Les membres statiques non constants ne peuvent toujours pas être initialisés de cette manière.

### Héritage du constructeur

Les règles relatives à la redéfinition des fonctions membres font que, par défaut, une fonction membre redéfinie dans une classe dérivée masque les fonctions de même nom des classes ascendantes. Il reste toutefois possible de réintroduire ces fonctions à l'aide de l'instruction `using`. Cependant, rien de comparable n'était prévu pour les constructeurs. Cela devient possible en C++11 :

``` cpp
class A
 {
  public :
    A( int ) { ..... }
    A( int, int ) { ..... }
 } ;
class B : public A
 {
  using A::A ; // on peut utiliser A::A( int) et A::A( int, int )
               // comme si ils étaient déclarés B::B( int) et B::B( int, int )
 } ;
```

## Amélioration de l'utilisation des fonctions membres

### Fonctions interdites

Il y a toute une catégorie d'objets pour lesquels autoriser les copies n'a pas de sens (mutex, verrous, pointeurs propriétaires, ...). Pour empêcher ces copies en C++03, on a pris l'habitude de déclarer privés le constructeur par copie et l'affectation par copie, et de ne pas fournir d'implémentation. Ainsi, toute tentative de copie depuis l'extérieur de la classe déclenche une erreur de compilation, et toute tentative depuis l'intérieur de la classe va déclencher une erreur lors de l'édition de liens.

``` cpp
class no_copies
{
public:
  no_copies(){}
private:
  no_copies(no_copies const&);             // (1) No implementation
  no_copies& operator=(no_copies const&);
};
no_copies a;
no_copies b(a); // (2) Won’t compile
```

En C++11, le comité a voulu remplacer cette astuce par un mécanisme plus clair et plus général : la possibilité d'interdire une fonction en la déclarant `= delete`. On peut ainsi réécrire le classe `no_copies` :

``` cpp
class no_copies
{
public:
  no_copies(){}
  no_copies(no_copies const&) = delete;
  no_copies& operator=(no_copies const&) = delete;
};
```

L'intention est mieux exprimée, permet d'obtenir des messages plus clair du compilateur, et une utilisation interdite depuis l'intérieur de la classe est détectée dès la compilation.

Par ailleurs, les fonctions interdites prennent part à la résolution des surdéfinitions, ce qui permet d'interdire certaines variantes, et certaines conversions implicites non souhaitées. Par exemple, si on définit une fonction qui réclame une valeur `short`, on peut facilement empêcher le compilateur d'accepter (et raccourcir) une valeur `int` :

``` cpp
void foo(short);
void foo(int) = delete;
.....
foo(42);        // Error, int overload declared deleted
foo((short)42); // OK
```

### Fonctions par défaut

A l'opposé de ce qui précède, le mot-clé `default` permet d'exiger explicitement du compilateur qu'il fournisse son implémentation par défaut pour les fonctions qu'il sait générer, telles que les constructeurs vides, par copie et déplacement, les destructeurs et les opérateurs d'affectation par copie et déplacement.

Dans quel but ?

  - Pour les faire apparaitre clairement dans l'interface, à des fins de documentation.
  - Pour les placer en accès protégé ou privé, alors qu'il est public par défaut.
  - Pour faire en sorte que le constructeur prenne un original no constant.
  - Pour rendre un destructeur virtuel, totu en conservant son implementation par defaut.
  - Pour forcer le compilateur à générer la fonction, dans un contexte ou il ne le fait

`  pas habituellement. Par exemple pour conserver le constructeur par defaut, alors`  
`  qu'on fournit par ailleurs un autre constructeur.`

  - Pour préserver les propriétés particulières liées aux implémentations par défaut.

<!-- end list -->

``` cpp
class Y
{
private:
  Y() = default;                    // Change access
public:
  Y(Y&) = default;                  // Take a non-const reference
  T& operator=(const Y&) = default; // Declare as defaulted for documentation
protected:
  virtual ~Y() = default;           // Change access and add virtual
};
```

Parmi les propriétés des implémentations par défaut, le fait qu'elles puissent être "triviales" peut avoir des effets :

  - Un objet dont le constructeur par copie, l'affectation par copie et le destructeur

`  sont triviaux, peut être copié avec memcpy ou memmove (très rapide).`

  - Pour qu'un type littéral puisse être utilisé dans une fonction constante (constexpr),

`  il faut que son constructeur vide, son constructeur par copie et son destructeur`  
`  soient triviaux.`

  - Une classe possédant un constructeur vide trivial, ainsi qu'un constructeur par copie,

`  un opérateur d'affectation par copie et un destructeur triviaux, peut être utilisé`  
`  dans une union aux constructeur et destructeur personnalisés.`

  - Une classe possédant un opérateur d'affectation par copie peut être utilisé

`  avec le patron de classe std::atomic.`

Une implémentation n'est pas triviale simplement parce qu'elle est fournit par le compilateur. Il y a d'autres conditions. Mais si elle est fournie par le développeur, elle ne peut pas être triviale.

Par ailleurs, une classe dont aucun constructeur n'est implémenté par le développeur peut être un "aggrégat" et être initialisée à l'aide d'un initialisateur d'aggrégat :

``` cpp
struct aggregate
{
  aggregate() = default;
  aggregate(aggregate const&) = default;
  int a;
  double b;
};
aggregate x={42,3.141}; // x.a <= 42, x.b <= 3.141
```

If you create an instance of class X without an initializer, the contained int (a) is default initialized. If the object has static storage duration, it’s initialized to zero; otherwise, it has an indeterminate value that can potentially cause undefined behavior if it’s accessed before being assigned a new value:

    #!cpp
    X x1; // x1.a has an indeterminate value
    </source>
    
    If, on the other hand, you
    initialize your instance of X by explicitly invoking the default constructor,
    then a is initialized to zero:
    
    <source lang="cpp">
    X x2=X(); // x2.a==0
    </source>
    
    This bizarre property also extends to base classes and members. If your class has a
    compiler-generated default constructor and any of your data members and base
    classes also have a compiler-generated default constructor, data members of
    those bases and members that are built-in types are also either left with an
    indeterminate value or initialized to zero, depending on whether or not the
    outer class has its default constructor explicitly invoked.
    
    Although this rule
    is confusing and potentially error prone, it does have its uses, and if you
    write the default constructor yourself, you lose this property; either data
    members like a are always initialized (because you specify a value or
    explicitly default construct) or always uninitialized (because you don’t):
    
    <source lang="cpp">
    X::X():a(){}   // a==0 always
    X::X():a(42){} // a==42 always
    X::X(){}       // (1)
    </source>
    
    If you omit the initialization of a from the constructor of X as in the third example
    (1), then a is left uninitialized for nonstatic instances of X and initialized
    to zero for instances of X with static storage duration.
    
    Under normal
    circumstances, if you write any other constructor manually, the compiler will
    no longer generate the default constructor for you, so if you want one you
    have to write it, which means you lose this bizarre initialization property.
    However, by explicitly declaring the constructor as defaulted, you can force
    the compiler to gen- erate the default constructor for you, and this property
    is retained:
    
    <source lang="cpp">
    X::X() = default; // Default initialization rules for a apply
    </source>
    
    This property is used for the atomic types (see section 5.2), which have their
    default constructor explicitly defaulted. Their initial value is always
    undefined unless either (a) they have static storage duration (and thus are
    statically initialized to zero), or (b) you explicitly invoke the default
    constructor to request zero initialization, or (c) you explicitly specify a
    value. Note that in the case of the atomic types, the constructor for
    initialization with a value is declared constexpr (see section A.4) in order
    to allow static initialization. ￼￼

### Test des noms de membres d'une classe dérivée

En C++03, on peut redéfinir involontairement une fonction virtuelle. On peut aussi, suite à une faute de frappe dans une redéfinition de fonction virtuelle, aboutir à la définition d'une nouvelle fonction.

C++11 permet de se protéger de ces erreurs : si vous qualifiez une méthode virtuelle à l'aide de `override`, le compilateur vérifiera que la méthode existe belle et bien dans la classe de base.

Par ailleurs, si vous voulez interdire qu'une méthode virtuelle , bien que virtuelle, soit redéfinie dans une classe dérivée, utilisez le qualificateur `final`. Ce même qualificateur peut aussi servir à dire qu'une classe ne peut plus être dérivée.

``` cpp
class A
 {
  public :
    virtual void fct1() =0 ;
    virtual void fct2( int ) =0 ;
    virtual void fct3( bool ) =0 ;
 } ;

class B : public A
 {
  public :
     virtual void fct3( bool ) final ;
 } ;

class C final : public B
 {
  public :
    void fct1 () override ;       // OK
    void ft2 ( int ) override ;   // erreur : A::ft2 n'existe pas
    void fct2 ( bool ) override ; // erreur : pas les bons types
    void fct3 ( bool ) override ; // erreur : B::fct3 est finale
 } ;

class D : public C                // erreur : C est final
 {} ;                             
```

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
