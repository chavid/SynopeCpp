# [Construction & destruction](TheorieObjets "wikilink")

\_\_FORCETOC\_\_

## Initialisation des variables de type prédéfini

Lors de sa déclaration, on peut affecter une valeur initiale à une variable. Il s'agit d'une **initialisation**, à différencier des **affectations**. Une variable non initialisée contient une valeur indéterminée jusqu'à sa première affectation. Il est recommandé de toujours initialisé toutes les variables, afin d'éviter d'avoir par erreur une variable de valeur indéterminée, ce qui est particulièrement difficile à déboguer.

``` cpp
int i ;       // variable de valeur indéterminée
i = 1 ;       // affectation de 1 à i
int x = 0 ;   // variable initialisée à 0
int y(0) ;    // variable initialisée à 0
int z{} ;     // variable initialisée à 0
```

## Constructeur

De même que pour les variables de type prédéfini, on veut pouvoir attribuer un état initial à tous les objets qui sont déclarés, mais cela demande généralement plusieurs valeurs, et ne peux donc s'écrire avec une simple signe "=".

Un constructeur est une fonction membre particulière, sans type de retour et portant le même nom que la classe. Il est automatiquement éxécutée à chaque création d'objet, et sert principalement à initialiser les variables membres de l'objet.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  public :

    Vecteur( double x, double y, double z ) // double x_, y_, z_ ;
     { x_ = x ; y_ = y ; z_ = z ; }         // x_ = x ;
                                            // y_ = y ;
    double norme()                          // z_ = z ;
     { return sqrt(x_*x_+y_*y_+z_*z_) ; }

  private :

    double x_ ;
    double y_ ;
    double z_ ;

 } ;

int main()
 {
  Vecteur v1 {1,5,-2} ;
  std::cout << v1.norme() << std::endl ;          

  Vecteur v2 (2,2,4) ;
  std::cout << v2.norme() << std::endl ;          

  return 0 ;
 }
```

## Destructeur

De même qu'un constructeur est éxécuté à chaque création d'objet (sur la pile ou sur le tas), on peut définir un destructeur à éxécuter à chaque destruction d'objet. Le destructeur est une fonction membre particulière, sans type de retour et portant le même nom que la classe, précédé d'un signe "~".

``` cpp
#include <iostream>
#include <cmath>

int nb_vecteurs {} ;

class Vecteur
 {
  public :

    Vecteur( double x, double y, double z )
     {
      x_ = x ; y_ = y ; z_ = z ;
      nb_vecteurs++ ;
     }

    ~Vecteur()
     { nb_vecteurs-- ; }

    double norme()
     { return sqrt(x_*x_+y_*y_+z_*z_) ; }

  private :

    double x_ ;
    double y_ ;
    double z_ ;

 } ;
 
void f()
 {
  Vecteur v2 {2,2,4} ;
  std::cout << nb_vecteurs << std::endl ;    
 }

int main()
 {
  Vecteur v1 {1,5,-2} ;
  std::cout << nb_vecteurs << std::endl ;  
  f() ;
  std::cout << nb_vecteurs << std::endl ;    
  return 0 ;
 }
```

## Zone d'initialisation

Le corps d'un constructeur peut contenir n'importe quelles instructions. Chaque fois que la valeur d'une variable est modifiée, il ne s'agit pas d'une initialisation mais d'une affectation. Si l'on veut agir directement sur la valeur initiale donnée à une variable membre, il faut utiliser la zone d'initialisation optionnelle, qui se place juste avant le corps du constructeur.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  public :

    Vecteur( double x, double y, double z ) // double x_(x) ;
     : x_{x}, y_{y}, z_{z}                  // double y_(y) ;
     {}                                     // double z_(z) ;

    double norme()
     { return sqrt(x_*x_+y_*y_+z_*z_) ; }

  private :

    double x_ ;
    double y_ ;
    double z_ ;

 } ;

int main()
 {
  Vecteur v1 {1,5,-2} ;
  std::cout << v1.norme() << std::endl ;          

  Vecteur v2 {2,2,4} ;
  std::cout << v2.norme() << std::endl ;          

  return 0 ;
 }
```

## Aggrégation

Si l'une des variables membres d'une classe est un objet, le constructeur de cet objet est toujours éxécuté avant que ne soit éxécuté le constructeur de la classe englobante.

Si l'on veut passer des valeurs au constructeur d'une variable membre, il faut le faire dans la **zone d'initialisation** du constructeur de la classe englobante, exactement comme pour les variables ordinaires.

Inversement aux constructeurs, les destructeurs des variables membres sont éxécutés après le destructeur de la classe englobante.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  public :
    Vecteur( double x, double y, double z )
     : x_{x}, y_{y}, z_{z}
     {}
    void change( double x, double y, double z )
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
    Impulsion( double x, double y, double z, double m )
     : vitesse_{x,y,z}, masse_{m}
     {}
    void change_vitesse( double x, double y, double z )
     { vitesse_.change(x,y,z) ; }
    double norme()
     { return (vitesse_.norme()*masse_) ; }
  private :
    Vecteur vitesse_ ;
    const double masse_ ;
 } ;

int main()
 {
  Impulsion i {1,2,3,4} ;
  std::cout << i.norme() << std::endl ;                      
  return 0 ;
 }
```

## Variables membres constantes

Les variables membres peuvent elles aussi être déclarées constantes. Elles peuvent être initialisées mais non modifiées : le seul endroit ou on peut leur donner une valeur est la zone d'initialisation du constructeur.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  public :
    Vecteur( double x, double y, double z )
     : x_{x}, y_{y}, z_{z}
     {}
    void change( double x, double y, double z )
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
    Impulsion( double x, double y, double z, double m )
     : vitesse_{x,y,z}, masse_{m}
     {}
    void change_vitesse( double x, double y, double z )
     { vitesse_.change(x,y,z) ; }
    void change_masse( double m )
     { masse_ = m ; }              // ERREUR DE COMPIL
    double norme()
     { return (vitesse_.norme()*masse_) ; }
  private :
    Vecteur vitesse_ ;
    const double masse_ ;
 } ;

int main()
 {
  Impulsion i {1,2,3,4} ;
  std::cout << i.norme() << std::endl ;                      
  i.change_vitesse(4,5,6) ;
  std::cout << i.norme() << std::endl ;
  return 0 ;
 }
```

## Héritage

Lors de la création d'un objet, et si sa classe dérive d'une autre, le constructeur de la classe de base est toujours éxécuté avant le constructeur de la classe dérivée.

Si l'on veut passer des valeurs au constructeur de la classe de base, il faut le faire dans la **zone dinitialisation**' du constructeur de la classe dérivée. En lieu et place d'un nom de variable membre, on utilise le nom de la classe de base.

Inversement aux constructeurs, le destructeur de la classe de base est éxécuté après le destructeur de la classe dérivée.

``` cpp
#include <iostream>
#include <cmath>

class Particule
 {
  public  :
    Particule( double x, double y, double z, double m )
     : impulsion_{x,y,z,m}
     {}
    double norme()
     { return impulsion_.norme() ; }
  private  :
    Impulsion impulsion_ ;
 } ;

class ParticuleChargee : public Particule
 {
  public  :
    ParticuleChargee( double x, double y, double z, double m, double c )
     : Particule{x,y,z,m}, charge_{c}
     {}
  private  :
    double charge_ ;
 } ;

int main()
 {
  ParticuleChargee p{1,2,3,4,5} ;
  std::cout << p.norme() << std::endl ;                      
  return 0 ;
 }
```

## Construction par défaut

Si et seulement si aucun constructeur n'a été déclaré par l'auteur d'une classe, alors le compilateur fournit un constructeur par défaut, sans arguments, dont la zone d'initialisation et le corps sont vides.

Lorsque la zone d'initialisation d'un constructeur est vide, ou pour toutes les variables membres qui n'y sont pas présentes, une initialisation par défaut est réalisée, sans argument. En cas de type prédéfini, la variable a une valeur indéfinie. En cas d'objet, le compilateur recherche un constructeur sans arguments.

Si aucun destructeur n'a été déclaré par l'auteur d'une classe, alors le compilateur fournit un destructeur par défaut, dont le corps est vide.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  public :
    Vecteur()
     : x_{}, y_{}, z_{}
     {}
    void init( double x, double y, double z )
     { x_ = x ; y_ = y ; z_ = z ; }
    double norme() const
     { return sqrt(x_*x_+y_*y_+z_*z_ ) ; }
  private :
    double x_ ;
    double y_ ;
    double z_ ;
 } ;

class Impulsion
 {
  public :
    Impulsion()
     : masse_{}       // IMPLICIT: , vitesse_
     {}
    void init( double x, double y, double z, double m )
     { vitesse_.init(x,y,z) ; masse_ = m ; }
    double masse()
     { return masse_ ; }
    double norme_vitesse()
     { return vitesse_.norme() ; }
  private :
    double masse_ ;
    Vecteur vitesse_ ;
 } ;

class ParticuleChargee
 {
  public  :
    // CONSTRUCTEUR PAR DEFAUT
    // ParticuleChargee()
    //  : impulsion_, charge_
    //  {}
    void init( double x, double y, double z, double m, double c )
     { impulsion_.init(x,y,z,m) ; charge_ = c ; }
    double charge()
     { return charge_ ; }
    double masse()
     { return impulsion_.masse() ; }
    double norme_vitesse()
     { return impulsion_.norme_vitesse() ; }
  private  :
    double charge_ ;
    Impulsion impulsion_ ;
 } ;

int main()
 {
  ParticuleChargee p ;
  std::cout << p.norme_vitesse() << std::endl  ; // 0                      
  std::cout << p.masse() << std::endl  ; // 0                      
  std::cout << p.charge() << std::endl ; // INDEFINI                      
  return 0 ;
 }
```

## Construction explicite

``` cpp
#include <iostream>
#include <string>

class Chaine_nulle {
 public:
  Chaine_nulle(std::size_t taille) : chaine_nulle(taille, '0') { std::cout << "Construction par taille = " << taille << std::endl; }
  const std::size_t taille() const { return chaine_nulle.size(); }
 private:
  std::string chaine_nulle;
};

Chaine_nulle ajouter(const Chaine_nulle& chaine_nulle, const Chaine_nulle& autre_chaine_nulle) {
  Chaine_nulle retour(chaine_nulle.taille() + autre_chaine_nulle.taille());
  return retour;
}

int main(int argc, char* argv[])
{
  Chaine_nulle c1(3);
  Chaine_nulle c2(5);
  Chaine_nulle c3 = ajouter(c1, c2);

  Chaine_nulle c4 = ajouter(c1, 2);  // Exemple d'une faute de frappe du programmeur
  return 0;
}
```

    #!txt
    Construction par taille = 3
    Construction par taille = 5
    Construction par taille = 8
    Construction par taille = 2
    Construction par taille = 5

Pour les constructeurs et fonctions ne prenant qu'un seul argument, le compilateur fait de la déduction de type implicite. Là, en passant un `int` à l'`operator+`, l'utilisateur ne s'attendant sûrement pas à utiliser le constructeur. C'est pour ça qu'il est conseillé de rendre `explicit` les constructeurs avec un seul argument (pour aller plus loin, voire les recommandations des [google code style](https://google.github.io/styleguide/cppguide.html#Implicit_Conversions)).

Il suffit d'utiliser le mot-clé `explicit`, comme cela : `explicit Tableau(int size)`. Cela a pour effet de générer l'erreur suivante à la compilation:

    #!txt
    error: invalid initialization of reference of type 'const Chaine_nulle&' from expression of type 'int'

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
