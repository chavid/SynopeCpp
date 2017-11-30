# [Expressions à base de patrons](TheorieGenerique "wikilink")

## Motivation

Pour implémenter des opérateurs mathématiques tels que la multiplication (`*`), tout en respectant les notations et les attendus des mathématiciens, il est recommandé que ces opérateurs construisent et retournent une nouvelle valeur, sans toucher aux opérandes. Si on multiplie d'énormes matrices, les conséquences sur le temps de calcul peuvent être dramatiques, en particulier à cause de toutes les valeurs intermédiaires qui sont produites avant d'arriver au résultat final :

``` cpp
typedef SquareMatrix<double, 10000> BigMatrix ;
BigMatrix m1, m2, m3, m4, m5 ;               // create matrices and
...                                          // give them values
BigMatrix result = m1 * m2 * m3 * m4 * m5 ;  // compute their product
```

Les techniques de programmation classique entraineront la création de quatre matrices temporaires (une pour chaque appel à `*`), et le parcours d'une séquence de quatre boucles à travers tous les éléments de matrice. Avec la technique dite des "expressions à base de patrons", il est possible d'éliminer les matrices temporaires et de fusionner les boucles, le tout sans changer la syntaxe du code client ci-dessus. Le code résultant utilise moins de mémoire et s'exécute incroyablement plus vite.

## Exemple

Pour les besoins de la démonstration, nous allons utiliser des vecteurs de 3 valeurs (mais qui pourraient tout aussi en contenir 1000), et nous concentrer sur une expression simple à base de + et - :

``` cpp
#include <iostream>

...

int main()
 {
  Vector V1{1,1,1}, V2{2,2,2}, V3{3,3,3}, V4{-1,-1,-1} ;
  V4 = V1+(V2-V3) ;
  std::cout<<V4<<std::endl ;
  return 0 ;
 }
```

L'objectif est donc d'éviter une implémentation naive des opérateurs + et - entre instances de Vector, qui conduirait à boucler trois fois sur la liste des valeurs et à construire deux objets temporaires. A la place, on souhaite que l'expression `V1+(V2-V3)` renvoie une structure la plus légère possible, sans calcul et sans boucle, ces derniers étant réalisé en une seule fois au dernier moment, lors de l'affectaton à `V4`.

## Marqueurs pour les opération supportées

Afin de permettre l'utilisation de techniques de traits, et de faciliter l'écriture du code, on va lister l'ensemble des opérations supportées. Nous avons vu, dans le cas des itérateurs, que l'on utilisait des classes vides pour incarner les différentes catégories d'itérateurs, avec des relations d'héritage. Dans le cas présent, on se contentera d'un simple enum :

``` cpp

enum Operations
 {
  Add,   // addition
  Sub,   // soustraction
 } ;
```

## Eléments de la structure intermédiaire

Notre structure intermédiaire sera construite comme un arbre d'objets représentant les operations et les opérandes constituant l'expression. Chaque élément de la structure doit pouvoir se comporte en itérateur : savoir renvoyer sa valeur courante, et savoir s'incrémenter. L'incrémentation peut être implémentée dans une classe de base, le calcul de la valeur courante doit être spécialisé pour chaque opération :

``` cpp

// Modèle de classe pour nos opérateurs binaires
template <class T, class U, int Op>
struct BinaryOp {} ;

// Classe de base pour nos opérateurs binaires
template <class T, class U>
struct BinaryOpBase
 {
  BinaryOpBase(T i1, U i2) : itr1(i1), itr2(i2) {}
  inline void operator ++() {++itr1; ++itr2;}

  T itr1 ;
  U itr2 ;
 } ;

// Spécialisation pour l'addition
template <class T, class U>
struct BinaryOp<T, U, Add> : public BinaryOpBase<T, U>
 {
  BinaryOp(T i1, U i2) : BinaryOpBase<T, U>(i1, i2) {}
  inline double operator *() { return *(this->itr1) + *(this->itr2) ; }
 } ;

// Spécialisation pour la soustraction
template <class T, class U>
struct BinaryOp<T, U, Sub> : public BinaryOpBase<T, U>
 {
  BinaryOp(T i1, U i2) : BinaryOpBase<T, U>(i1, i2) {}
  inline double operator *() { return *(this->itr1) - *(this->itr2) ; }
 } ;
```

## Construction de la structure intermédiaire

Fournissons maintenant l'implémentation des opérateurs du C++ pour la classe `Vector`, qui ne va rien calculer, mais à la place construire des éléments de la structure ci-dessus. L'astuce consistant à qtocker dans notre arbre, partout ou devrait apparaitre un vecteur, un itérateur vers son premier élément :

``` cpp

// Utilitaires

template <class T, class U>
inline auto MakeAdd(const T & t, const U & u)
 { return BinaryOp<T, U, Add>(t, u); }

template <class T, class U>
inline auto MakeSub(const T & t, const U & u)
 { return BinaryOp<T, U, Sub>(t, u); }
 
// Opérateur +

template <class Expr1, class Expr2>
auto operator +(const Expr1 & v1, const Expr2 & v2)
 { return MakeAdd(v1, v2); }

template <class Expr>
auto operator +(const Vector & v1, const Expr & v2)
 { return MakeAdd(v1.begin(), v2); }

template <class Expr>
auto operator +(const Expr & v1, const Vector & v2)
 { return MakeAdd(v1, v2.begin());}

auto operator +(const Vector & v1, const Vector & v2)
 { return MakeAdd(v1.begin(), v2.begin()); }

// Opérateur -

template <class Expr1, class Expr2>
auto operator -(const Expr1 & v1, const Expr2 & v2)
 { return MakeSub(v1, v2); }

template <class Expr>
auto operator -(const Vector & v1, const Expr & v2)
 { return MakeSub(v1.begin(), v2); }

template <class Expr>
auto operator -(const Expr & v1, const Vector & v2)
 { return MakeSub(v1, v2.begin());}

auto operator -(const Vector & v1, const Vector & v2)
 { return MakeSub(v1.begin(), v2.begin()); }
```

## Classe Vector

Enfin, il nous reste à doter notre classe `Vector` des itérateurs nécessaires, et d'un operateur d'affectation capable de boucler sur notre structure intermédiaire :

``` cpp
struct Vector
 {
  // Itérateurs
  typedef double * iterator ;
  typedef const double * const_iterator ;
  iterator begin() { return elems ; }
  iterator end()   { return elems + 3 ; }
  const_iterator begin() const { return elems ; }
  const_iterator end()   const { return elems + 3 ; }

  // déclenche le déroulement de l'expression
  template <class T>
  const Vector & operator=(Expr v)
   {
    for (iterator i = begin(); i != end(); ++i, ++v)
      *i = *v ;
    return *this ;
   }

  // Données
  double elems[3] ;
 } ;
```
