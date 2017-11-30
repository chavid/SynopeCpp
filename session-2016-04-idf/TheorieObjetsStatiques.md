# [Variables de classe et fonctions statiques](TheorieObjets "wikilink")

\_\_FORCETOC\_\_

## Variables de classe

Au sein d'une définition de classe, chaque variable peut être déclarée statique. Une telle variable n'existe qu'à un seul exemplaire au moment de l'éxécution, au lieu d'être dupliquée pour chaque objet. On peut considérer que les variables membres statiques sont communes et partagées entre tous les objets d'une classe.

Les variables statiques sont régies par les mêmes droits d'accès que les autres variables. Si elles sont privées, seules les fonctions membres de la classe peuvent y accéder. Les variables membres statiques sont aussi appellées variables de classe, ou attributs de classe.

``` cpp
#include <iostream>
#include <string>

class Particule
 {
  public  :
    std::string nom() { return nom__ ; }
    void change_masse( double masse ) { masse_ = masse ; }
    double masse() { return masse_ ; }
  private  :
    static std::string nom__ ;
    double masse_ ;
 } ;

std::string Particule::nom__ = "Particule" ;

class ParticuleNonChargee : public Particule
 {
  public :
    std::string nom() { return nom__ ; }
  private  :
    static std::string nom__ ;
 } ;

std::string ParticuleNonChargee::nom__ = "Particule non chargée" ;

class ParticuleChargee : public Particule
 {
  public  :
    std::string nom() { return nom__ ; }
    void change_charge( double charge ) { charge_ = charge ; }
    double charge() { return charge_ ; }
  private  :
    static std::string nom__ ;
    double charge_ ;
 } ;

std::string ParticuleChargee::nom__ = "Particule chargée" ;
```

## Fonctions statiques

Au sein d'une définition de classe, chaque fonction peut être déclarée statique, à condition qu'elle n'utilise que des variables membres statiques. Une telle fonction peut-être appelée de façon ordinaire sur un objet. Elle peut aussi être appelée directement à l'aide de l**'opérateur de portée de la classe**. Les fonctions membres statiques sont aussi appellées fonctions de classe, ou méthodes de classe.

``` cpp
#include <iostream>
#include <string>

class Particule
 {
  public  :
    static std::string nom() { return nom__ ; }
    void change_masse( double masse ) { masse_ = masse ; }
    double masse() { return masse_ ; }
  private  :
    static std::string nom__ ;
    double masse_ ;
 } ;

std::string Particule::nom__ = "Particule" ;

class ParticuleNonChargee : public Particule
 {
  public :
    static std::string nom() { return nom__ ; }
  private :
    static std::string nom__ ;
 } ;

std::string ParticuleNonChargee::nom__ = "Particule non chargée" ;

class ParticuleChargee : public Particule
 {
  public  :
    static std::string nom() { return nom__ ; }
    void change_charge( double charge ) { charge_ = charge ; }
    double charge() { return charge_ ; }
  private  :
    static std::string nom__ ;
    double charge_ ;
 } ;

std::string ParticuleChargee::nom__ = "Particule chargée" ;

int main()
 {
  std::string rep ;
  std::cout << ParticuleChargee::nom() << " ? " ;                      
  std::cin >> rep ;           
  if (rep=="oui")
   {
    ParticuleChargee p1 ;
    p1.change_masse(1) ;
    p1.change_charge(1) ;
    std::cout << p1.nom() << std::endl ;          
    std::cout << "masse = " << p1.masse() << std::endl ;         
    std::cout << "charge = " << p1.charge() << std::endl ;         
   }
  else
   {
    ParticuleNonChargee p2 ;
    p2.change_masse(2) ;
    std::cout << p2.nom() << std::endl ;          
    std::cout << "masse = " << p2.masse() << std::endl ;         
   }
  return 0 ;
 }
```

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
