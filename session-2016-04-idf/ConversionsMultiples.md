# [Constructeurs, assignateurs et conversions multiples](TheorieGenerique "wikilink")

## Constructeurs et assignateurs généralisés

#### Motivation

Les pointeurs améliorés se comportent comme des pointeurs ordinaires, mais ajoutent des fonctionnalités supplémentaires, comme de rendre la mémoire allouée sur le tas. La plupart des itérateurs de la bibliothèque standard sont des pointeurs améliorés qui rendent des services précieux, comme passer d'un élément au suivant par un simple appel à l'opérateur `++`, ce que ne pourrait faire un pointeur ordinaire.

Une des choses que les pointeurs ordinaires font très bien : les conversions implicites. Un pointeur de classe dérivée se converti sans broncher en pointeur de classe de base, un pointeur d'objet non constant en pointeur d'objet constant, etc. Prenons cette hiérarchie à trois étages :

``` cpp
class Top { ... } ;
class Middle : public Top { ... } ;
class Bottom : public Middle { ... } ;
Top * pt1 = new Middle ;                   // convert Middle * to Top *
Top * pt2 = new Bottom ;                   // convert Bottom * to Top *
const Top * pct2 = pt1 ;                   // convert Top *    to const Top*
```

Reproduire de telles conversions avec des classes maison est une affaire délicate. Nous aurions besoin de faire en sorte que le code suivant compile :

``` cpp
template <typename T>
class SmartPtr
 {
  public :                             // smart pointers are typically
    explicit SmartPtr( T * realPtr ) ; // initialized by built-in pointers
    .....
 } ;

SmartPtr<Top> pt1 =                 // convert SmartPtr<Middle>  
  SmartPtr<Middle>( new Middle ) ;  // to SmartPtr<Top>

SmartPtr<Top> pt2 =                 // convert SmartPtr<Bottom>  
  SmartPtr<Bottom>( new Bottom ) ;  // to SmartPtr<Top>

SmartPtr<const Top> pct2 = pt1 ;    // convert SmartPtr<Top>  
                                    // to SmartPtr<const Top>
```

Les différentes instances d'un même patron n'ont pas de relation au sens des types, si bien que du point de vue du compilateur, `SmartPtr`<Middle> et `SmartPtr`<Top> n'ont pas plus de rapport que, disons, `vector`<float> et `Widget`. Pour obtenir les conversions que nous souhaitons entre les instances de `SmartPtr`, nous devons les programmer explicitement.

#### Constructeurs généralisés

Commencons par les constructeurs. Un point clé : il est impossible d'écrire tous les constructeurs dont nous pourrions avoir besoin. Dans la hiérarchie ci-dessus, nous pouvons coder comment construire un `SmartPtr`<Top> à partir d'un `SmartPtr`<Middle> ou d'un `SmartPtr`<Bottom>, mais une nouvelle classe dérivée est ajoutée plus tard ?

``` cpp
class BelowBottom : public Bottom { ... } ;
```

Nous avons à présent besoin d'un constructeur de `SmartPtr`<Top> à partir d'un `SmartPtr`<BelowBottom>, mais il est inimaginable de devoir modifier le patron `SmartPtr` à chaque création d'une nouvelle classe. Ce qu'il nous faut, c'est un constructeur paramétré, pouvant être instancié automatiquement pour chaque nouveau type d'argument. Un tel constructeur est une forme de "fonction membre paramétrée", ou "patron de fonction membre" :

``` cpp
template<typename T>
class SmartPtr
 {
  public :
    template<typename U>                       // member template
    SmartPtr( const SmartPtr<U> & other) ;     // for a "generalized
    .....                                      // copy constructor"
 } ;
```

Pour n'importe quel type `T` et `U`, le compilateur sait maintenant instancier un constructeur de `SmartPtr`<T> à partir d'un `SmartPtr`<U> : on parle de "constructeur généralisé".

Nous n'avons pas déclaré ce constructeur `explicit`, volontairement, car les conversions entre pointeurs ordinaires sont elles aussi implicites.

Nous disposons maintenant d'un constructeur qui en fait trop, car il sait aussi transformer un `SmartPtr`<Top> en `SmartPtr`<Bottom>, ce qui est contraire au sens de l'héritage. De même, nous ne voulons pas d'une transformation de `SmartPtr`<int> en `SmartPtr`<double>, alors qu'il n'existe pas de conversion implicite entre `int*` en `double*`. Comment limité notre constructeur aux seuls cas souhaitables ? En imaginant disposer, comme dans `unique_ptr` et `shared_ptr`, d'une méthode `get` qui donne accès au pointeur ordinaire qui est caché dans un `SmartPtr`, on peut obtenir l'effet voulu à l'aide de cette implémentation :

``` cpp
template<typename T>
class SmartPtr
 {
  public :
  template<typename U>
    SmartPtr( const SmartPtr<U> & other )         // initialize this held ptr
     : heldPtr(other.get()) { ..... }             // with other's held ptr
    T * get() const { return heldPtr ; }
    .....
  private :                                       // built-in pointer held
    T * heldPtr ;                                 // by the SmartPtr
 } ;
```

Toute l'astuce est dans l'instruction `heldPtr(other.get())` : si et seulement si les types `T` et `U` permettent une transformation de `U*` en `T*`, l'instruction compilera et nous disposerons alors d'une conversion de `SmartPtr`<U> en `SmartPtr`<T>. Exactement l'effet recherché.

#### Assignateurs généralisés

Ce qui vaut pour les contructeurs vaut aussi pour les opérateurs d'assignation. Le type `shared_ptr` accepte la construction, l'assignation et le déplacement depuis tous les types compatibles `shared_ptr`, `unique_ptr` et `auto_ptr` :

``` cpp
template<class T> class shared_ptr
 {
  public :
    ...
    template< class Y > explicit shared_ptr( Y * ptr ) ;
    template< class Y > shared_ptr( const shared_ptr<Y> & r ) ;
    template< class Y > shared_ptr( shared_ptr<Y> && r ) ;
    template< class Y > explicit shared_ptr( const std::weak_ptr<Y> & r ) ;
    template< class Y > shared_ptr( std::auto_ptr<Y> && r ) ;
    ...
    template< class Y > shared_ptr & operator=( const shared_ptr<Y> & r ) ;
    template< class Y > shared_ptr & operator=( shared_ptr<Y> && r ) ;
    template< class Y > shared_ptr & operator=( std::auto_ptr<Y> && r ) ;
    ...
 } ;
```

#### Constructeur et assignateur par défaut

Attention : le compilateur **n'assimile** pas ces constructeurs et assignateurs généralisés aux constructeurs et assignateurs par défaut, même si, à l'évidence, une des instances possibles de ces fonctions membres généralisées sont justement les constructeur de copie et assignateur par défaut. En conséquence, le compilateur va ajouter ces fonctions, à moins que vous ne le fassiez vous-même. C'est pour cette raison que dans `shared_ptr`, on peut trouver la déclaration d'un constructeur et d'un assignateur par copie, en plus des versions paramétrées :

``` cpp
template<class T> class shared_ptr
 {
  public :
    ...
    shared_ptr(shared_ptr const& r);                 // copy constructor

    template<class Y>                                // generalized
    shared_ptr(shared_ptr<Y> const& r);              // copy constructor

    shared_ptr& operator=(shared_ptr const& r);      // copy assignment

    template<class Y>                                // generalized
    shared_ptr& operator=(shared_ptr<Y> const& r);   // copy assignment
    ...
 } ;
```

## Réintégrer les fonctions non-membres dans les patrons quand on souhaite faire des conversions de type

Déjà pour une classe ordinaire, lorsqu'elle incarne une entité numérique dotée d'opérateurs mathématiques, on recommande de privilégier des fonctions non membres, pour bénéficier des conversions implicites de façon uniforme sur tous les termes d'une opération. Reprenons l'exemple de la classe {{{Rational}} et de l'opérateur de multiplication, en paramétrant innocemment le tout :

``` cpp
template<typename T>
class Rational
 {
  public :
    Rational( const T & numerator = 0,     
              const T & denominator = 1) ;
    const T numerator() const ;
    const T denominator() const ;
    .....
 } ;

template<typename T>
const Rational<T> operator*( const Rational<T> & lhs,
                             const Rational<T> & rhs )
 { ...... }
```

On aimerait pouvoir écrire et compiler du code de ce genre :

``` cpp
Rational<int> oneHalf(1, 2) ;
Rational<int> result = oneHalf * 2;   // error! won't compile
```

Pourquoi l'ajout de patrons brise-t-il cette implémentation ? En l'absence de patron, le compilateur sait exactement quelle est la fonction que nous voulons exécuter : l'opérateur `*` prenant deux `Rational` en argument. A présent, il dispose d'une infinité d'opérateurs `*` possibles et ne parvient pas à trouver la bonne valeur de `T`, pour nous évidente.

Le problème est spécifique aux fonctions paramétrées : on instancie que les fonctions nécessaires, cette étape est réalisée avant la résolution de la surcharge, et les possibilités de conversions implicites ne sont pas prises en compte lors de cette première étape. Dans l'exemple qui nous occupe, l'expression `oneHalf * 2` nécessite un operateur `*` prenant en argument un `Rational`<int> et un `int`, cela ne colle pas avec la signature de l'opérateur paramétré, donc l'instance avec `T` égal à `int` n'est pas crée, et finalement elle manque lors de l'étape ultérieure de résolution de la surcharge.

On peut aider le compilateur à être conscient de l'existence de la fonction `operator*( const Rational`<int>` & , const Rational`<int>` & )` en ajoutant une déclaration `friend` au sein de la classe. Ainsi, lorsque la variable `oneHalf` est crée, le compilateur crée l'instance `Rational`<int>, la fonction `operator*( const Rational`<int>` & , const Rational`<int>` & )` se trouve déclarée, elle est donc connue au moment de la résolution de la surcharge de `oneHalf*2`, et notre code compile \! (notons qu'à l'intérieur de la définition du patron de classe `Rational`, on peut utiliser `Rational` comme raccourci de `Rational`<T>)

``` cpp
template<typename T>
class Rational
 {
  public :
    .....
    friend                                              // declare operator*
      const Rational operator*(const Rational& lhs,     // function (see
                               const Rational& rhs);    // below for details)
 } ;

template<typename T>                                // define operator*
const Rational<T> operator*(const Rational<T>& lhs, // functions
                            const Rational<T>& rhs)
 { ..... }
```

Cela compile... mais échoue au moment de l'édition de liens :(

Avec notre déclaration `friend`, nous avons fait en sorte que l'instance de l'opérateur `*` avec la valeur `int` soit déclarée, mais pas que sa définition soit générée. On peut s'en sortir en polluant la définition de la classe avec le corps de la fonction amie :

``` cpp
template<typename T>
class Rational
 {
  public :
    .....
    friend const Rational operator*( const Rational & lhs, const Rational & rhs )
     {
      return Rational(lhs.numerator() * rhs.numerator(),  
        lhs.denominator() * rhs.denominator()) ;
     }                                         
 } ;
```

Un tel usage de `friend` n'a rien à voir avec l'accès aux membres privés, mais il résoud notre problème. Pour limiter la pollution de la définition de classe, notamment pour des fonctions de taille plus conséquente, on fera en sorte que cette fonction amie délègue l'essentiel de ses instructions à une fonction d'aide extérieure :

``` cpp
template<typename T> class Rational ;                // declare
                                                     // Rational
                                                     // template
template<typename T>                                    // declare
const Rational<T> doMultiply(const Rational<T>& lhs,    // helper
                             const Rational<T>& rhs);   // template
template<typename T>
class Rational
 {
  public :
   .....
  friend
    const Rational<T> operator*(const Rational<T>& lhs,
                                const Rational<T>& rhs)   // Have friend
   { return doMultiply(lhs, rhs); }                      // call helper
  .....
 } ;

template<typename T>                                      // define
const Rational<T> doMultiply(const Rational<T>& lhs,      // helper
                             const Rational<T>& rhs)      // template in
{                                                         // header file,
  return Rational<T>(lhs.numerator() * rhs.numerator(),   // if necessary
                     lhs.denominator() * rhs.denominator());
}
```

## A retenir

  - Utiliser une fonction membre paramétrée pour générer les fonctions qui acceptent tous les types compatibles.
  - Si vous déclarez une fonction membre paramétrée pour obtenir un constructeur de copie généralisé ou un opérateur d'assignation généralisé, il reste nécessaire de déclarer aussi le constructeur de copie et l'opérateur d'assignation normaux.
  - Si vous écrivez un patron de classe qui offre des fonctions supportant des conversions de type implicites pour tous les paramètres, définissez ces fonctions en tant qu'amies au sein du patron de classe.

## Exercice

#### Fichiers à récupérer

  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef10.cpp>

#### Enoncé

Faites en sorte que Coef sache s'initialiser ou se convertir aussi bien en float qu'en double. Commencez par transformer en patrons les méthodes de conversion qui ont été factorisées dans \!CoefImpl, puis l'assignateur et le convertisseur de Coef, puis modifiez les instances de Testeur pour utiliser des float au lieu des double et transformez en patron la méthode erreur de Testeur.

#### Solution

  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef11.cpp>
  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef11.ref>
