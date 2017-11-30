# [Opérateurs](TheorieExceptionsOperateurs "wikilink")

\_\_FORCETOC\_\_

## Motivation

Lorsque l'on définit des classes représentant des grandeurs mathématiques, on aimerait pouvoir utiliser les opérateurs mathématiques habituels plutôt que des méthodes.

``` cpp
#include <iostream>

class Vecteur
 {
  public :

    init( double x, double y, double z )
     { x_ = x ; y_ = y ; z_ = z ; } 

    Vecteur ajoute( Vecteur v )
     {
      Vecteur res ;
      res.init(x_+v.x_,y_+v.y_,z_+v.z_) ;
      return res ;
     } 

  private :
    double x_, y_, z_ ; 
 } ; 

int main ()
 { 
  Vecteur v1, v2, v3 ; 
  v1.init(1,0,2) ;
  v2.init(-2,5,3) ;
  v3 = v1.ajoute(v2) ;
  return 0 ; 
 }
```

## Définition d'un opérateur

On peut étendre l'usage des opérateurs du C++ aux classes, définissant une méthode dont le nom est une concaténation entre le mot "operator" et l'opérateur proprement dit.

``` cpp
#include <iostream>

class Vecteur
 {
  public :

    Vecteur( double x, double y, double z )
     : x_{x}, y_{y}, z_{z}
     {} 

    Vecteur operator+( Vecteur v )
     {
      Vecteur res ;
      res.init(x_+v.x_,y_+v.y_,z_+v.z_) ;
      return res ;
     } 

  private :
    double x_, y_, z_ ; 
 } ; 

int main ()
 { 
  Vecteur v1(1,0,2), v2(-2,5,3), v3 ; 
  v3 = v1 + v2 ;
  return 0 ; 
 }
```

## Limites

On ne peut pas changer le nombre d'argument des opérateurs ou les règles de précédence. On ne peut pas "inventer" de nouveaux opérateurs, mais seulement redéfinir classes la plupart de ceux qui existent déjà pour les types prédéfinis.

Les opérateurs suivants existent par défaut pour toute nouvelle classe :

``` cpp
= == !=
. -> .* ->*
new delete sizeof
```

Les opérateurs suivants peuvent être redéfinis au sein d'une classe :

``` cpp
+ - * / % ^ & | ~ !
= < > += -= *= /= %= ^= &=
|= << >> >>= <<= == != <= >= &&
|| ++ -- ->* , -> [] () new delete
```

Les opérateurs suivants ne peuvent jamais être redéfinis :

``` cpp
:: . .* ?: sizeof
```

## Opérateurs chevrons

On peut étendre les entrées/sorties de la bibliothèque standard aux nouvelles classes en surchargeant les opérateurs `<<` et `>>`.

``` cpp
#include <iostream>

class complex
 {
  public : complex( double r, double i ) : r_(r), i_(i) {}
  private : double r_, i_ ;
  friend ostream & operator<<( std::ostream &, complex const & ) ;
 } ;   

std::ostream & operator<<( std::ostream & os, complex const & c )
 {
  os << '(' << c.r_ << '|' << c.i_ << ')' ;       
  return os ;
 }

int main( int argc, char * * argv )
 {
  complex c(1.2,-.5) ;
  std::cout << c << std::endl ;          
  return 0 ;
 }
```

\== Opérateur d'affectation (`operator=`)

Pour toute classe, le compilateur fournit un opérateur d'affectation par défaut. Ce dernier recopie les valeurs des variables membres, une à une, mais en présence de pointeur il se contente de copier le pointeur lui-même, sans dupliquer la valeur pointée. Cette dernière se retrouve donc partagée entre l'ancien et le nouvel objet. On parle de "copie superficielle". En particulier, le code ci-dessous est verolé. Quel est le problème ?

``` cpp
#include <iostream>
#include <cstring>

class Texte
 {
  public :

    Texte( const char * str )
     : taille_ { longueur(str) } ,
       donnees_ { new char [taille_] }
     { copie(str,donnees) ; }

    ~Texte()
     { delete [] donnees_ ; }

  private :

    int longueur( const char * itr )
     {
      int l {} ;
      while ((*itr)!='\0')
       { l++ ; itr++ ; }
      return l+1 ;
     }

    void copie( const char * itr1, char * itr2 )
     {
      while ((*itr1)!='\0')
       { *itr2_ = *itr1_ ; itr2_++ ; itr1_++ }
      *itr2 = '\0' ;
     }

    unsigned int taille_{0} ;
    char * donnees_{nullptr} ;

 } ;

int main()
 {
  Texte t1("hello world") ;
  Texte t2 ;
  t2 = t1 ;
  return 0 ;
 }
```

Pour que le code précédent fonctionne, on doit ajouter un opérateur d'affectation qui duplique la ressource pointée. Par ailleurs, pour avoir une classe "propre", il faut aussi fournir un contructeur de copie du même acabit.

``` cpp
#include <iostream>
#include <cstring>

class Texte
 {
  public :
    ...

    Texte( const Texte & t )
     : taille_ { longueur(t.taille_) } ,
       donnees_ { new char [taille_] }
     { copie(t.donnees_,donnees) ; }

    Texte & operator=( const Texte & t )
     {
      delete [] donnees_ ;
      taille_ = t.taille_ ;
      donnees_ = new char [t.taille_] ;
      copie(t.donnees_,donnees) ;
      return *this ;
     }

  ...
 } ;

int main()
 {
  Texte t1("hello world") ;
  Texte t2(t1) ;
  texte t3 ;
  t3 = t2 ;
  return 0 ;
 }
```

Que se passe-t-il avec le code suivant ?

``` cpp
...

int main()
 {
  Texte t1("hello world") ;
  t1 = t1 ;
  return 0 ;
 }
```

Pour répondre à ce problème, dans toutes les implémentations de l'opérateur d'affectation, on protège l'objet contre une affectation à lui-même :

``` cpp
class Texte
 {
  public :
    ...
    Texte & operator=( const Texte & t )
     {
      if (this == &t) return *this ;
      ...
     }
  ...
 } ;
```

## Opérateurs de conversion

Un opérateur de conversion est un méthode permettant de transformer un objet vers un type donné. Lorsque le compilateur a besoin de forcer le type d'un objet, de façon implicite ou explicit, c'est cet operateur qui sera appelé. Il faut un opérateur de conversion pour chaque type envisagé.

``` cpp
#include <iostream>

class Rationnel
 {
  public :

    Rationnel( int numerateur, int denominateur )
     : numerateur_{numerateur}, denominateur_{denominateur}
     {}

    operator int()
     { return numerateur_ / denominateur ; }

    operator double()
     { return double(numerateur_) / denominateur ; }

  private :

    int numerateur_ ;
    int denominateur_ ;

 } ;

int main()
 {
  Rationnel val_r(15,7) ;
  double val_d {val_r} ;
  int val_i {val_r} ;
  std::cout << "val_r: " << val_r << std::endl ;
  std::cout << "val_d: " << val_d << std::endl ;
  std::cout << "val_i: " << val_i << std::endl ;
  return 0 ;
 }
```

Depuis C++11, le mot-clé `explicit` n'est plus réservé aux constructeurs, mais peut maintenant être appliqué aux opérateurs de conversion. Dans l'exemple ci-dessous, on utilise la transformation en double pour extraire la norme d'un vecteur, mais on ne veut pas que le compilateur utilise sans prévenir nos vecteurs dans des fonctions qui attendent un scalaire.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {

  public :

    Vecteur( double x, double y, double z )
     : x_{x}, y_{y}, z_{z}
     {} 

    explicit operator double()
     { return sqrt(x_*x_+y_*y_+z_*z_) ; }

  private :

    double x_, y_, z_ ; 

 } ; 

int main ()
 { 
  Vecteur v(-2,5,3) ;
  std::cout << double(v) << std::endl ; // OK
  std::cout << cos(v) << std::endl ;    // PAS OK
  return 0 ; 
 }
```

## Opérateurs d'exécution et objets-fonctions

On peut doter une classe d'un opérateur `operator()` qui permet de donner à ses objets un comportement de fonction. Dit autrement : si vous écrivez une expression dans laquelle un objet est utilisé qui si il s'agissait d'une fonction, c'est son opérateur d'exécution qui est invoqué. On peut aussi voir ces "objets-fonctions", ou "foncteurs", comme des fonctions auxquelles on aurait ajouter des paramètres perssitants d'un appel à l'autre.

``` cpp
#include <iostream>

class FonctionLineaire
 {

  public :

    FonctionLineaire( double constante )
     : constante_{constante}
     {}

    double operator()( double valeur )
     { return constante_*valeur ; }

  private :

    double constante_ ;    

 } ;

int main()
 {
  double valeurs[10] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } ;
  FonctionLineaire fois2(2) ;
  for ( double valeur : valeurs )
    std::cout<<fois2(valeur)<<std::endl ;
 }
```

  
  
© *CNRS / INRIA 2016*  
*Assemblé et rédigé par David Chamont et Vincent Rouvreau, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
