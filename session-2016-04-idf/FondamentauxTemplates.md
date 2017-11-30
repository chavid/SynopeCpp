# [Patrons (templates)](TheorieGenerique.md)

\_\_FORCETOC\_\_

## Motivation

Certaines fonctions très utilisées doivent être réécrites pour tous les types possibles, même si les instructions sont exactement les mêmes.

``` cpp
int min( int arg1, int arg2 )
 {
  if (arg1<arg2) return arg1 ;
  else return arg2 ;
 }

double min( double arg1, double arg2 )
 {
  if (arg1<arg2) return arg1 ;
  else return arg2 ;
 }

int main()
 {
  std::cout << min(4,3) << std::endl ;          
  std::cout << min(5.6,2) << std::endl ;          
  return 0 ;
 }
```

## Fonctions Paramétrées

Une fonction est **paramétrée** si certains types auxquels elle a recours ne sont pas connus lors de sa définition. En règle générale, ces types doivent être précisés à chaque utilisation (mais le compilateur peut souvent les "deviner" si on les omet). Ces fonctions paramétrées ont aussi appelées **patron de fonction**.

``` cpp
#include <iostream>

template <typename T>
T min( T arg1, T arg2 )
 {
  if (arg1<arg2) return arg1 ;
  else return arg2 ;
 }

int main()
 {
  std::cout << min<int>(4,3) << std::endl ;          
  std::cout << min<double>(5.6,2) << std::endl ;          
  return 0 ;
 }
```

## Classes Paramétrées

On peut également paramétrer les classes. De même que pour les fonctions, on précise les types manquants à chaque utilisation de la classe (et cette fois le compilateur ne peut pas deviner). Ces classes paramétrées ont aussi appelées **patron de classe**.

``` cpp
#include <iostream>
#include <iomanip>
#include <cmath>

template <typename T>
class Vecteur
 {
  public :
    void init( T x, T y, T z )
     { x_ = x ; y_ = y ; z_ = z ; } 
    T norme()
     { return sqrt(x_*x_+y_*y_+z_*z_) ; }
  private :
    T x_, y_, z_ ; 
 } ; 

int main ()
 {
  double n = 1/3. ; 
  Vecteur<double> vd ;
  vd.init(n/sqrt(3.),n/sqrt(3.),n/sqrt(3.)) ;
  Vecteur<float> vf ;
  vf.init(n/sqrt(3.),n/sqrt(3.),n/sqrt(3.)) ;
  std::cout.precision(18) ;
  std::cout.setf(std::ios::fixed,std::ios::floatfield) ;
  std::cout
   << "n = " << std::setw(20) << n << '\n'          // 0.333333333333333315
   << "  ~ " << std::setw(20) << vd.norme() << '\n' // 0.333333333333333370
   << "  ~ " << std::setw(20) << vf.norme() << '\n' // 0.333333343267440796
   << std::flush ;
  return 0 ; 
 }
```

## Faire suivre les informations de types

Lorsqu'on utilise tout un enchainement de fonctions et de classes paramétrées, il arrive qu'on perde l'accès à l'information des paramètres initiaux. Il est toujours utile de renommer un paramètre type avec un nom prédéfini fixe, permettant de le récupérer si besoin.

``` cpp
#include <iostream>
#include <iomanip>
#include <cmath>
#include <typeinfo>

template <typename T>
class Vecteur
 {
  public :
    typedef T value_type ; 
    void init( T x, T y, T z )
     { x_ = x ; y_ = y ; z_ = z ; } 
    T norme()
     { return sqrt(x_*x_+y_*y_+z_*z_) ; }
  private :
    T x_, y_, z_ ; 
 } ; 

template <typename V>
void process( V & v )
 {
  double n = 1/3. ; 
  v.init(n/sqrt(3.),n/sqrt(3.),n/sqrt(3.)) ;
  std::cout.precision(18) ;
  std::cout.setf(std::ios::fixed,std::ios::floatfield) ;
  std::cout << typeid(typename V::value_type).name() << " : " << std::setw(20) << v.norme() << std::endl ;
 }

int main ()
 {
  Vecteur<double> vd ;
  process(vd) ;
  Vecteur<float> vf ;
  process(vf) ;
  return 0 ; 
 }
```

## Limites

On ne peut pas déclarer des contraintes sur les valeurs possibles pour les paramètres. A chaque utilisation, le compilateur substitue le type demandé au paramètre, et réussit à compiler ou pas, selon que le type convient ou pas. On dit que l'interface requise pour les paramètres est **implicitement** constituée par l'ensemble des opérations qui sont appliquées au paramètre au sein de la définition de la fonction ou de la classe. Le comité de normalisation du langage réfléchit depuis longtemps à un moyen de spécifier des contraintes sur les paramètres : c'est l'idée de \*\*concept\*\*, qui peine à se concrétiser. En attendant, si vous créez des fonctions et des classes paramètres dont les paramètres doivent respecter des conditions complexes, vous en êtes réduit à documenter ces conditions. Par ailleurs, n'hésitez pas à utiliser des **noms parlants** pour vos paramètres, plutôt que `A` ou `T`...

Ensuite, la facilité avec laquelle on peut multiplier les types utilisés comme paramètres peut conduire à une démultiplication exponentielle du code source à compiler, ce qui peut faire exploser le temps de compilation et la taille du fichier éxécutable final : on parle de **code bloat**.

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
