# [Constantes](TheorieGenerique.md)

\_\_FORCETOC\_\_

## Variables constantes

Lors de la déclaration d'une variable de type prédéfini, on peut lui donner une valeur initiale et déclarer que la variable est constante. Le compilateur s'assurera qu'aucune instruction ultérieure n'essaie de modifier la valeur de la variable. Les **constantes** sont donc des variables qui peuvent être initialisées mais jamais modifiées.

``` cpp
int x ;                    // variable de valeur indéterminée
int y = 0 ;                // variable initialisée à 0
y = 1 ;                    // affectation de 1 à y
double const pi = 3.1415 ; // constante 
pi = 3.1416 ;              // ERREUR DE COMPILATION
```

## Fonctions membres et objets constants

On peut également déclarer des objets constants, mais il est impossible au compilateur de vérifier automatiquement si l'état d'un objet est modifié ou non par une méthode donnée. Le programmeur doit aider le compilateur en précisant, méthode par méthode, si elle est susceptible ou pas de modifier l'état de l'objet auquel elle est appliquée.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  public :

    Vecteur( double x, double y, double z )
     { init(x,y,z) ; }

    void init( double x, double y, double z )
     { x_ = x ; y_ = y ; z_ = z ; }

    double norme() const
     { return sqrt(x_*x_+y_*y_+z_*z_ ) ; }

  private :

    double x_ ;
    double y_ ;
    double z_ ;
 } ;


int main()
 {
  Vecteur const v(1,2,3) ;
  std::cout << v.norme() << std::endl ;                      
  v.init(4,5,6) ; // ERREUR DE COMPILATION
  return 0 ;
 }
```

## Passage d'arguments constants

Un argument de fonction peut-être tout ou partie constant. Cela permet notamment de passer un argument par adresse tout en évitant qu'il soit modifié, et d'économiser ainsi la recopie de la valeur. Cela permet aussi de passer l'adresse d'un tableau dynamique en protégeant le contenu du tableau.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  public :

    Vecteur( double x, double y, double z )
     { x_ = x ; y_ = y ; z_ = z ; }

    double x() const {return x_ ; }
    double y() const {return y_ ; }
    double z() const {return z_ ; }

    double norme() const
     { return sqrt(x_*x_+y_*y_+z_*z_ ) ; }

  private :

    double x_ ;
    double y_ ;
    double z_ ;
 } ;


void affiche_vecteur( Vecteur const & v )
 {
  std::cout << v.x() ;           
  std::cout << "/" ;           
  std::cout << v.y() ;           
  std::cout << "/" ;           
  std::cout << v.z() ;           
  std::cout << std::endl ;           
 }

void affiche_tableau( int taille, int const * tableau )
 {
  int i ;
  for ( i=0 ; i<taille ; i++ )
   { std::cout << tableau[i] << std::endl ; }           
 }

int main()
 {
  int t[4] = { 1, 2, 3, 4 } ;
  affiche_tableau(10,t) ;
  Vecteur v(1,2,3) ;
  affiche_vecteur(v) ;
  return 0 ;
 }
```

## Variables membres constantes

Les variables membres peuvent elles aussi être déclarées constantes. Elles peuvent être initialisées mais non modifiées : le seul endroit ou on peut leur donner une valeur est la zone d'initilisation du constructeur.

``` cpp
#include <iostream>
#include <cmath>

class Vecteur
 {
  public :

    Vecteur( unsigned int taille )
     : taille_(taille)
     { valeurs_ = new double [taille_] ; }

    ~Vecteur()
     { delete [] valeurs_ ; }

    double & operator[]( unsigned int i )
     {
      if (i>=taille_)
       { exit(1) ; }
      else
       { return valeurs_[i] ; }
     }

    double norme() const
     {
      double somme = 0 ;
      int i ;
      for ( i=0 ; i<taille_ ; ++i )
       { somme += valeurs_[i] ; }
      return sqrt(somme) ;
     }

  private :

    unsigned int const taille_ ;
    double * valeurs_ ;
 } ;

int main()
 {
  Vecteur v(3) ;
  v[0] = 1 ;
  v[1] = 2 ;
  v[2] = 3 ;
  std::cout << v.norme() << std::endl ;                      
  return 0 ;
 }
```

© *[David Chamont](http://llr.in2p3.fr/spip.php?page=view_person&personID=121), Laboratoire Leprince-Ringuet (LLR) / IN2P3 / CNRS, Ecole polytechnique / Université Paris-Saclay*  
© *Ce(tte) œuvre est mise à disposition selon les termes de la [Licence Creative Commons Attribution - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/)*
