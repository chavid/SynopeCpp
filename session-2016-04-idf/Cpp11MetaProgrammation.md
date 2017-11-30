# [Métaprogrammation en C++11/14](TheorieGenerique "wikilink")

## Calcul de valeurs

Le calcul des valeurs à la compilation en C++03 est basé uniquement sur le comportement des patrons qui, en dehors des types, peuvent aussi être paramétrés avec des valeurs entières. Par exemple :

``` cpp
 
template<int i, int j>
struct Multiply
 {
  static const int value = i * j ;
 }
...
int array[ Multiply<3,4>::value ] ;
```

La structure Multiply nous montre un exemple simple de métafonction : on donne deux valeurs (expressions constantes) de type int et nous pouvons en trouver une troisième pendant la compilation, qui peut servir à dimensionner un tableau statique.

La formule `Multiply<3,4>::value` ressemble difficilement à un appel de fonction, mais c'en est pourtant un : vous entrez des valeurs et vous obtenez un résultat. Et c'est ainsi que sont implémentés les métafonctions en C++03.

Passons à l'exemple de la factorielle, cas d'école habituel pour illustrer les concepts de base de la programmation fonctionnelle : la récursivité, les conditions d'arrêt de récursivité et la gestion des erreurs.

``` cpp
 
template<int i>
struct Factorial
 {
  static const int value = i * Factorial<i-1>::value ;
 } ;

template<>
struct Factorial<0>
 {
  static const int value = 1 ;
 } ;
```

La première définition introduit la récursion. La valeur value de l'instanciation courante est déterminée par la valeur correspondante d'une autre instanciation. La seconde définition correspond à la condition finale de récursivité, pour i == 0.

Si l'on veut vérifier une précondition, par exemple que i est bien positif, on peut s'appuyer sur un patron intermédiaire qui possède une spécialisation non implémentée dans le cas interdit, déclenchant une erreur de compilation :

``` cpp
 
template<int i, bool c>
struct NegativeArgument
 {
  static const int value = Factorial<i>::value ;
 } ;

template<int i>
struct NegativeArgument<i, false> ; //indéfini

template<int i>
struct SafeFactorial
 {
  static const int value = NegativeArgument<i, (i >= 0)>::value ;
 } ;
```

La définition de métafonctions comme celles-là peut vite devenir compliquée. Comment C++11 va-t-il nous aider ? En étendant le concept des expressions constantes, qui peut complètement se substituer à la gymnastique ci-dessus :

Pour faire court, en C++11, nos deux versions (sécurisée et non sécurisée) du calcul de factorielle durant le temps de compilation peuvent être définies ainsi :

``` cpp
 
constexpr int factorial(int i)
 { return (i==0) ? 1 : i*factorial(i-1) ; }

constexpr int safe_factorial(int i)
 { return (i<0) ? throw exception() : factorial(i) ; }
```

## Calcul de types

Un autre type de métafonction est celui où nous allons passer un type comme argument et en avoir un autre en retour. Par exemple, cett métafonction supprime le pointeur de plus haut niveau là où il est possible de l'enlever, via une spécialisation partielle :

``` cpp
 
template<typename U>    // cas général 
struct remove_pointer
 {
  typedef U type ;
 } ;

template<typename T>    // pour U = T*
struct remove_pointer<T*>
 {
  typedef T type ;
 } ;
```

Cela ne semble pas si mal, mais dans l'état actuel des choses, pour utiliser notre métafonction dans un patron de fonction, vous devrez utiliser typename :

``` cpp
 
template<typename T>
typename remove_pointer<T>::type fun(T val) ;
```

Le C++11 offre un nouveau confort avec les alias de patrons. Nous pouvons ajouter au code précédent un alias qui facilite son usage :

``` cpp
 
template<typename T>
using remove_pointer_t = typename remove_pointer<T>::type ;
...
template<typename T>
remove_pointer_t<T> fun(T val) ;
```

Cette même convention, consistant à faire une variante "\_t" de toutes les métafonctions qui font du calcul de types, est appliquée partout dans la bibliothèque standard, par exemple pour `enable_if`<T> :

``` cpp
template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;
...
template <typename T>
void do_stuff(std::enable_if_t<std::is_integral<T>::value, T> &t) {}
...
template <typename T>
void do_stuff(std::enable_if_t<std::is_class<T>::value, T> &t) {}
```

## Nouvelle bibliothèque standard

C++11/14 introduit une volée de nouveaux traits, dont `is_fundamental`<T> (est-ce que T est un type prédéfini ?), `is_array`<T> (est-ce que T est un tableau ?), et `is_base_of<T1, T2>` (est-ce que T2 est une classe de base de T1, ou T1 lui-même ?). On notera que ces nouveaux traits sont tous implémentés individuellement pour favoriser leur composition, plutôt que regroupés à la façon de la bibliothèque C++03. La nouvelle bibliothèque fournit également un grand nombre de métafonctions de manipulation de types telles que `remove_pointer`<T>.

``` cpp
 
namespace std
 {
  // helper class:
  template <class T, T v> struct integral_constant ;
  typedef integral_constant<bool, true> true_type ;
  typedef integral_constant<bool, false> false_type ;
  
  // primary type categories:
  template <class T> struct is_void;
  template <class T> struct is_null_pointer;
  template <class T> struct is_integral;
  template <class T> struct is_floating_point;
  template <class T> struct is_array;
  template <class T> struct is_pointer;
  template <class T> struct is_lvalue_reference;
  template <class T> struct is_rvalue_reference;
  template <class T> struct is_member_object_pointer;
  template <class T> struct is_member_function_pointer;
  template <class T> struct is_enum;
  template <class T> struct is_union;
  template <class T> struct is_class;
  template <class T> struct is_function;
  
  // composite type categories:
  template <class T> struct is_reference;
  template <class T> struct is_arithmetic;
  template <class T> struct is_fundamental;
  template <class T> struct is_object;
  template <class T> struct is_scalar;
  template <class T> struct is_compound;
  template <class T> struct is_member_pointer;
  
  // type properties:
  template <class T> struct is_const;
  template <class T> struct is_volatile;
  template <class T> struct is_trivial;
  template <class T> struct is_trivially_copyable;
  template <class T> struct is_standard_layout;
  template <class T> struct is_pod;
  template <class T> struct is_literal_type;
  template <class T> struct is_empty;
  template <class T> struct is_polymorphic;
  template <class T> struct is_abstract;
  template <class T> struct is_signed;
  template <class T> struct is_unsigned;
  template <class T, class... Args> struct is_constructible;
  template <class T> struct is_default_constructible;
  template <class T> struct is_copy_constructible;
  template <class T> struct is_move_constructible;
  template <class T, class U> struct is_assignable;
  template <class T> struct is_copy_assignable;
  template <class T> struct is_move_assignable;
  template <class T> struct is_destructible;
  template <class T, class... Args> struct is_trivially_constructible;
  template <class T> struct is_trivially_default_constructible;
  template <class T> struct is_trivially_copy_constructible;
  template <class T> struct is_trivially_move_constructible;
  template <class T, class U> struct is_trivially_assignable;
  template <class T> struct is_trivially_copy_assignable;
  template <class T> struct is_trivially_move_assignable;
  template <class T> struct is_trivially_destructible;
  template <class T, class... Args> struct is_nothrow_constructible;
  template <class T> struct is_nothrow_default_constructible;
  template <class T> struct is_nothrow_copy_constructible;
  template <class T> struct is_nothrow_move_constructible;
  template <class T, class U> struct is_nothrow_assignable;
  template <class T> struct is_nothrow_copy_assignable;
  template <class T> struct is_nothrow_move_assignable;
  template <class T> struct is_nothrow_destructible;
  template <class T> struct has_virtual_destructor;
  
  // type property queries:
  template <class T> struct alignment_of;
  template <class T> struct rank;
  template <class T, unsigned I = 0> struct extent;
  
  // type relations:
  template <class T, class U> struct is_same;
  template <class Base, class Derived> struct is_base_of;
  template <class From, class To> struct is_convertible;
  
  // const-volatile modifications:
  template <class T> struct remove_const;
  template <class T> struct remove_volatile;
  template <class T> struct remove_cv;
  template <class T> struct add_const;
  template <class T> struct add_volatile;
  template <class T> struct add_cv;
  template <class T> using remove_const_t = typename remove_const<T>::type;
  template <class T> using remove_volatile_t = typename remove_volatile<T>::type;
  template <class T> using remove_cv_t = typename remove_cv<T>::type;
  template <class T> using add_const_t = typename add_const<T>::type;
  template <class T> using add_volatile_t = typename add_volatile<T>::type;
  template <class T> using add_cv_t = typename add_cv<T>::type;
  
  // reference modifications:
  template <class T> struct remove_reference;
  template <class T> struct add_lvalue_reference;
  template <class T> struct add_rvalue_reference;
  template <class T> using remove_reference_t = typename remove_reference<T>::type;
  template <class T> using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
  template <class T> using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;
  
  // sign modifications:
  template <class T> struct make_signed;
  template <class T> struct make_unsigned;
  template <class T> using make_signed_t = typename make_signed<T>::type;
  template <class T> using make_unsigned_t = typename make_unsigned<T>::type;
  
  // array modifications:
  template <class T> struct remove_extent;
  template <class T> struct remove_all_extents;
  template <class T> using remove_extent_t = typename remove_extent<T>::type;
  template <class T> using remove_all_extents_t = typename remove_all_extents<T>::type;
  
  // pointer modifications:
  template <class T> struct remove_pointer;
  template <class T> struct add_pointer;
  template <class T> using remove_pointer_t = typename remove_pointer<T>::type;
  template <class T> using add_pointer_t = typename add_pointer<T>::type;
  
  // other transformations:
  template <std::size_t Len, std::size_t Align = /*default-alignment*/> struct aligned_storage;
  template <class T> struct decay;
  template <bool, class T = void> struct enable_if;
  template <bool, class T, class F> struct conditional;
  template <class... T> struct common_type;
  template <class T> struct underlying_type;
  template <class> class result_of; // not defined
  template <class F, class... ArgTypes> class result_of<F(ArgTypes...)>;
  template <std::size_t Len, std::size_t Align = /*default-alignment*/> using aligned_storage_t = typename aligned_storage<Len,Align>::type;
  template <std::size_t Len, class... Types> using aligned_union_t = typename aligned_union<Len,Types...>::type;
  template <class T> using decay_t = typename decay<T>::type;
  template <bool b, class T = void> using enable_if_t = typename enable_if<b,T>::type;
  template <bool b, class T, class F> using conditional_t = typename conditional<b,T,F>::type;
  template <class... T> using common_type_t = typename common_type<T...>::type;
  template <class T> using underlying_type_t = typename underlying_type<T>::type;
  template <class T> using result_of_t = typename result_of<T>::type;
 }
```
