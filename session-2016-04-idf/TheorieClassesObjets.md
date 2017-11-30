# [Classes & objets](TheorieObjets "wikilink")

\_\_FORCETOC\_\_

## Motivation

Certaines variables ont vocation à fonctionner ensemble. On les initialise au même moment, et on les passent ensemble aux fonctions qui les manipulent.

``` cpp
#include <iostream>
#include <cmath>

double norme( double v_x, double v_y, double v_z )
 { return sqrt( v_x*v_x + v_y*v_y + v_z*v_z ) ; }

int main()
 {
  double v1_x, v1_y, v1_z ;
  v1_x = 1 ;
  v1_y = 5 ;
  v1_z = -2 ;

  std::cout << norme(v1_x,v1_y,v1_z) << std::endl ;          

  double v2_x, v2_y, v2_z ;
  v2_x = 2 ;
  v2_y = 2 ;
  v2_z = 4 ;

  std::cout << norme(v2_x,v2_y,v2_z) << std::endl ;          

  return 0 ;
 }
```

## Regroupement des variables

#### Création d'une classe

Un objet est un groupe de variables. On qualifie ces dernières de **variables membres**, ou **attributs** de l'objet. Le nom et le type des variables qui composent l'objet sont définis par sa classe.

``` cpp
#include <iostream>
#include <cmath>

double norme( double v_x, double v_y, double v_z )
 { return sqrt( v_x*v_x + v_y*v_y + v_z*v_z ) ; }

class Vecteur
 {
  ...
  double x ;
  double y ;
  double z ;
 } ;

int main()
 {
  Vecteur v1 ;
  v1.x = 1 ;
  v1.y = 5 ;
  v1.z = -2 ;
  std::cout << norme(v1.x,v1.y,v1.z) << std::endl ;          

  Vecteur v2 ;
  v2.x = 2 ;
  v2.y = 2 ;
  v2.z = 4 ;
  std::cout << norme(v2.x,v2.y,v2.z) << std::endl ;          

  return 0 ;
 }
```

#### Utilisation d'objets comme arguments de fonctions

Un objet peut être utilisé en tant qu'argument de fonction comme n'importe quelle variable.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  ...
  double x ;
  double y ;
  double z ;
 } ;

double norme( Vecteur v )
 { return sqrt(v.x*v.x+v.y*v.y+v.z*v.z) ; }

int main()
 {
  Vecteur v1 ;
  v1.x = 1 ;
  v1.y = 5 ;
  v1.z = -2 ;
  std::cout << norme(v1) << std::endl ;          

  Vecteur v2 ;
  v2.x = 2 ;
  v2.y = 2 ;
  v2.z = 4 ;
  std::cout << norme(v2) << std::endl ;          

  return 0 ;
 }
```

Un objet peut aussi être passé par référence...

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  ...
  double x ;
  double y ;
  double z ;
 } ;

void init( Vecteur & v, double x, double y, double z )
 { v.x = x ; v.y = y ; v.z = z ; }

double norme( Vecteur v )
 { return sqrt(v.x*v.x+v.y*v.y+v.z*v.z) ; }

int main()
 {
  Vecteur v1 ;
  init(v1,1,5,-2) ;
  std::cout << norme(v1) << std::endl ;          

  Vecteur v2 ;
  init(v2,2,2,4) ;
  std::cout << norme(v2) << std::endl ;          

  return 0 ;
 }
```

... ou par pointeur

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  ...
  double x ;
  double y ;
  double z ;
 } ;

void init( Vecteur * pv, double x, double y, double z )
 { (*pv).x = x ; (*pv).y = y ; (*pv).z = z ; }

double norme( Vecteur v )
 { return sqrt(v.x*v.x+v.y*v.y+v.z*v.z) ; }

int main()
 {
  Vecteur v1 ;
  init(&v1,1,5,-2) ;
  std::cout << norme(v1) << std::endl ;          

  Vecteur v2 ;
  init(&v2,2,2,4) ;
  std::cout << norme(v2) << std::endl ;          

  return 0 ;
 }
```

A noter : l'opérateur `->` est un raccourci pour appliquer en série `*` puis `.`, ce qui permet d'alléger un peu l'écriture dans un code riche en pointeurs. Par exemple, pour l'exemple ci-dessus :

``` cpp
...

void init( Vecteur * pv, double x, double y, double z )
 { pv->x = x ; pv->y = y ; pv->z = z ; }

...
```

On peut également le qualifier de constant lors d'un passage par référence ou pointeur, afin d'éviter la duplication des données tout en les protégeant d'une modification malencontreuse :

``` cpp
..

double norme( const Vecteur & v )
 { return sqrt(v.x*v.x+v.y*v.y+v.z*v.z) ; }

...
```

## Rattachement des fonctions

#### Intégration des fonctions à la classe

On peut également attacher des fonctions à une classe. On qualifie ces dernières de **fonctions membres**, ou **méthodes**. Ces fonctions sont définies par la classe. On les invoquent sur un objet, et elles voient les variables membres de l'objet en question, à travers le pointeur this.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  ...
  double x ;
  double y ;
  double z ;

  void init( double xx, double yy, double zz )
   { (*this).x = xx ; (*this).y = yy ; (*this).z = zz ; }

  double norme()
   { return sqrt( (*this).x * (*this).x + (*this).y * (*this).y + (*this).z * (*this).z ) ; }
 } ;

int main()
 {
  Vecteur v1 ;
  v1.init(1,5,-2) ;
  std::cout << v1.norme() << std::endl ;          

  Vecteur v2 ;
  v2.init(2,2,4) ;
  std::cout << v2.norme() << std::endl ;          

  return 0 ;
 }
```

#### Séparation de la déclaration et de la définition

Par défaut, il vaut mieux décrire le corps des fonctions séparement de la définition de la classe.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  ...
  double x ;
  double y ;
  double z ;

  void init( double xx, double yy, double zz ) ;
  double norme() ;
 } ;

void Vecteur::init( double xx, double yy, double zz )
 { (*this).x = xx ; (*this).y = yy ; (*this).z = zz ; }
 
double Vecteur::norme()
 { return sqrt( (*this).x * (*this).x + (*this).y * (*this).y + (*this).z * (*this).z ) ; }

int main()
 {
  Vecteur v1 ;
  v1.init(1,5,-2) ;
  std::cout << v1.norme() << std::endl ;          

  Vecteur v2 ;
  v2.init(2,2,4) ;
  std::cout << v2.norme() << std::endl ;          

  return 0 ;
 }
```

#### Appels mutuels de fonctions membres

Une fonction membre, invoquée sur un certain objet, peut appeler une autre fonction membre de ce même objet, à travers le pointeur this.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  ...
  double x ;
  double y ;
  double z ;

  void init( double xx, double yy, double zz ) ;
  double norme() ;
  void affiche_norme() ;
 } ;

void Vecteur::init( double xx, double yy, double zz )
 { (*this).x = xx ; (*this).y = yy ; (*this).z = zz ; }
 
double Vecteur::norme()
 { return sqrt( (*this).x * (*this).x + (*this).y * (*this).y + (*this).z * (*this).z ) ; }

void Vecteur::affiche_norme()
 { std::cout << (*this).norme() << std::endl ; }

int main()
 {
  Vecteur v1 ;
  v1.init(1,5,-2) ;
  v1.affiche_norme() ;


  Vecteur v2 ;
  v2.init(2,2,4) ;
  v2.affiche_norme() ;

  return 0 ;
 }
```

#### This implicite

Le pointeur this peut être omis par le développeur, et l'est presque tout le temps. Lorsque le compilateur rencontre un nom de variable qui n'est pas défini dans la fonction membre courante, il vérifie si il existe une variable membre avec ce nom. Pour mieux distinguer les variables membres au sein du code, les programmeurs utilisent généralement une convention de nommage, par exemple l'ajout d'un '\_' à la fin du nom de variable.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  ...
  double x_ ;
  double y_ ;
  double z_ ;

  void init( double x, double y, double z ) ;
  double norme() ;
  void affiche_norme() ;
 } ;

void Vecteur::init( double x, double y, double z )
 { x_ = x ; y_ = y ; z_ = z ; }
 
double Vecteur::norme()
 { return sqrt(x_*x_+y_*y_+z_*z_) ; }

void Vecteur::affiche_norme()
 { std::cout << norme() << std::endl ; }

int main()
 {
  Vecteur v1 ;
  v1.init(1,5,-2) ;
  v1.affiche_norme() ;


  Vecteur v2 ;
  v2.init(2,2,4) ;
  v2.affiche_norme() ;

  return 0 ;
 }
```

#### Méthodes constantes

Tout comme n'importe quelle variable, un objet peut-être déclaré constant lors de sa création, ou bien dans certains contexte ou il a été échangé par référence ou pointeur constant.

Dans ces circonstances, on ne peut plus invoquer que des méthodes qui ne modifient pas le contenu de l'objet, ce que le compilateur ne peut pas toujours vérifier automatiquement. Le programmeur doit aider le compilateur en précisant, méthode par méthode, si elle est susceptible ou pas de modifier l'état de l'objet auquel elle est appliquée.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  public :
    void init( double x, double y, double z )
     { x_ = x ; y_ = y ; z_ = z ; }
    double norme() const
     { return sqrt(x_*x_+y_*y_+z_*z_ ) ; }
  private :
    double x_ ;
    double y_ ;
    double z_ ;
 } ;

void affiche_norme( const Vecteur & v )
 {
  std::cout << v.norme() << std::endl ;
  // v.init(0,0,0) ; ERREUR DE COMPILATION
 }

int main()
 {
  Vecteur v ;
  v.init(1,2,3) ;
  affiche_norme(v) ;                  
  return 0 ;
 }
```

## Masquage

La partie **privée** d'une classe n'est accessible qu'aux membres de la classe. On place généralement les variables membres dans la partie privée, et les fonctions membres dans la partie **publique**.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  public :

    void init( double x, double y, double z )
     { x_ = x ; y_ = y ; z_ = z ; }

    double norme()
     { return sqrt(x_*x_+y_*y_+z_*z_ ) ; }

  private :

    double x_ ;
    double y_ ;
    double z_ ;
 } ;

int main()
 {
  Vecteur v ;
  v.init(1,5,-2) ;
  std::cout << v.norme() ;           
  // v.x_ = 0 ;
  return 0 ;
 }
```

## Fonctions et classes amies

Exceptionnellement, il peut-être utile de déclarer comme "amies" certaines classes et fonctions extérieures, leur donnant ainsi accès aux membre privés. Cela se déclare à l'aide du mot-clé `friend`.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  public :

    void init( double x, double y, double z )
     { x_ = x ; y_ = y ; z_ = z ; }

    double norme()
     { return sqrt(x_*x_+y_*y_+z_*z_ ) ; }

  private :

    friend void affiche( std::ostream &, const Vecteur & ) ;

    double x_ ;
    double y_ ;
    double z_ ;
 } ;

void affiche( std::ostream & os, const Vecteur & vec )
 { os<<"("<<vec.x_<<"|"<<vec.y_<<"|"<<vec.z_<<")"<<std::endl ; }

int main()
 {
  Vecteur v ;
  v.init(1,5,-2) ;
  affiche(std::cout,v) ;
  std::cout << v.norme() ;           
  // v.x_ = 0 ;
  return 0 ;
 }
```

## Aggrégation des objets

Une classe peut posséder des attributs dont le type est une autre classe : un objet peut contenir d'autres objets.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  public :

    void init( double x, double y, double z )
     { x_ = x ; y_ = y ; z_ = z ; }

    double norme()
     { return sqrt(x_*x_+y_*y_+z_*z_ ) ; }

  private :

    double x_ ;
    double y_ ;
    double z_ ;
 } ;


class Impulsion
 {
  public :

    void init( double x, double y, double z, double m )
     {
      vitesse_.init(x,y,z) ;
      masse_ = m ;
     }

    double norme()
     { return (vitesse_.norme()*masse_) ; }

  private :

    Vecteur vitesse_ ;
    double masse_ ;

 } ;

int main()
 {
  Impulsion imp ;
  imp.init(1,5,-2,2) ;
  std::cout << imp.norme() << std::endl ;          
  return 0 ;
 }
```

## Classes imbriquées

Si une classe n'a d'utilité qu'au sein d'une autre classe, on peut "imbriquer" sa définition à l'intérieur.

``` cpp
#include <iostream>
#include <cmath>

class Impulsion
 {
  public :

    void init( double x, double y, double z, double m )
     {
      vitesse_.init(x,y,z) ;
      masse_ = m ;
     }

    double norme()
     { return (vitesse_.norme()*masse_) ; }

  private :

    class Vecteur
     {
      public :
        void init( double x, double y, double z )
         { x_ = x ; y_ = y ; z_ = z ; }
        double norme()
         { return sqrt(x_*x_+y_*y_+z_*z_ ) ; }
      private :
        double x_ ;
        double y_ ;
        double z_ ;
     } ;

   Vecteur vitesse_ ;
    double masse_ ;

 } ;

int main()
 {
  Impulsion imp ;
  imp.init(1,5,-2,2) ;
  std::cout << imp.norme() << std::endl ;          
  return 0 ;
 }
```

Pour autant, si on place la définition imbriquée dans la partie publique, on peut utiliser cette classe de l'extérieur :

``` cpp
#include <iostream>
#include <cmath>

class Impulsion
 {
  public :

    class Vecteur
     {
      public :
        void init( double x, double y, double z )
         { x_ = x ; y_ = y ; z_ = z ; }
        double norme()
         { return sqrt(x_*x_+y_*y_+z_*z_ ) ; }
      private :
        double x_ ;
        double y_ ;
        double z_ ;
     } ;

    void init( double x, double y, double z, double m )
     {
      vitesse_.init(x,y,z) ;
      masse_ = m ;
     }

    double norme()
     { return (vitesse_.norme()*masse_) ; }

  private :

   Vecteur vitesse_ ;
    double masse_ ;

 } ;

int main()
 {
  Impulsion::Vecteur vec ;
  vec.init(1,5,-2) ;
  std::cout << vec.norme() << std::endl ;    
  
  Impulsion imp ;
  imp.init(1,5,-2,2) ;
  std::cout << imp.norme() << std::endl ;
  
  return 0 ;
 }
```

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
