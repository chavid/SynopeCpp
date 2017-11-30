# [A la croisée de la surcharge et des patrons](TheorieGenerique "wikilink")

## Motivation

Quand patrons et fonctions surchargées sont en compétition, les patrons sont souvent invasifs et le résultat surprenant :

``` cpp
void foo( unsigned i )
 { std::cout << "unsigned " << i << "\n" ; }
...
template <typename T>
void foo( const T & t )
 { std::cout << "template " << t << "\n"; }
...
f(42)
```

Que pensez vous qu'il s'affiche ? "template 42", parce "42" est par défaut de type "int", et correspondra parfaitement au patron de fonction, alors que la première fonction demande une conversion.

Quand le compilateur résoud la surcharge et examine une fonction candidate qui est un patron, il doit substituer les types spécifiés explicitement ou les déduire des arguments d'appel de la fonction. Prenons ce nouvel exemple :

``` cpp
int negate( int i )
 { return -i ; }

template <typename T>
typename T::value_type negate( const T & t )
 { return -t() ; }

negate(42) ;
```

Cette fois, c'est bien la première variante qui est sélectionnée. Mais tous les candidats ont été envisagés, et le patron a été interprété sous cette forme :

``` cpp
int::value_type negate( const int & t) ;
```

Une forme invalide, puisque le type prédéfini `int` n'a pas de membre `value_type`. On pourrait imaginer que le compilateur émette une erreur dans ce cas de figure, mais cela rendrait l'écriture de code générique extrêmement difficile. Le standard préconise donc donc que la déduction de type a échouée, et que le candidat doit être ignoré, mais qu'il ne s'agit pas d'une erreur de compilation.

## SFINAE

Dans le jargon C++, cette règle est appelée SFINAE (Substitution Failure Is Not An Error). Le standard précise que la règle s'applique au "contexte immédiat" du patron de fonction, c'est à dire tout ce qui intervient dans la déclaration de la fonction, et non dans sa définition, i.e. le corps de la fonction. Modifions un peu notre exemple précédent :

``` cpp
template <typename T>
void negate( const T & t)
 { typename T::value_type n = -t() ; }
```

Cette fois, si jamais cette fonction est sélectionnée pour résoudre un appel de fonction, avec un type prédéfini pour T, nous aurons bel et bien une erreur de compilation.

La morale, c'est que lorsque vous écrivez un patron de fonction qui n'a de sens qu'avec certains types, essayez de formuler une déclaration qui fera échouer la déduction de type, afin que le type inapproprié ne passe pas le cap de la résolution de la surcharge.

## `enable_if`

Très tôt dans l'histoire du C++, SFINAE est devenu un mécanisme qui a été utilisé à dessein, lorsqu'on s'est rendu compte qu'il permettait d'écrire des patrons ne se déclenchant qu'avec des paramètres appropriés, au lieu d'attraper tout ce qui passe. La technique la plus adoptée est la suivante :

``` cpp
template <bool, typename T = void>
struct enable_if
 {} ;

template <typename T>
struct enable_if<true, T>
 { typedef T type ; } ;
```

En faisant "transiter" un type T à travers un enable\_if, on peut faire en sorte qu'une condition, si elle n'est pas vérifiée, fasse échouer la substitution, et donc écarte un patron de fonction sans déclencher d'erreur de compilation.

``` cpp
template <typename T>
void do_stuff(typename enable_if<std::is_integral<T>::value, T>::type & t )
 {
  // an implementation for integral types (int, char, unsigned, etc.)
 }

template <typename T>
void do_stuff(typename enable_if<std::is_class<T>::value, T>::type & t )
 {
  // an implementation for class types
 }
```

## Exercice

#### Fichiers à récupérer

  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef15.cpp>

#### Enoncé

Faites en sorte que les fonctions `fois_puissance_de_deux()` et `entier_max()` puisse s'appliquer à n'importe quel type entier, et rien d'autre, en vous appuyant sur `std::enable_if<>` et `std::is_integral<>`. De même, permettez la multiplication d'un `Coef`<T> par n'importe quel type entier et non signé.

#### Solution

  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef16.cpp>
