
#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>


//==============================================
// utilitaires
//==============================================

void echec( unsigned int code, std::string commentaire )
 {
  std::cout<<"[ERREUR "<<code<<" : "<<commentaire<<"]"<<std::endl ;
  exit(code) ;
 }

int fois_puissance_de_deux( int nombre, int exposant )
 {
  if (nombre<0) { echec(1,"cas imprevu") ; }
  if ((exposant<=-int(sizeof(int)<<3))||(exposant>=int(sizeof(int)<<3))) { echec(1,"exposant trop grand") ; }
  if (exposant<0) { return (nombre>>-exposant) ; }
  if (nombre>(((unsigned int)(-1))>>exposant>>1)) { echec(1,"overflow") ; }
  return (nombre<<exposant) ; 
 }

int arrondi( double d )
 {
  if (d>0) { return int(d+.5) ; }
  else { return int(d-.5) ; }
 }

int entier_max( int nombre_bits )
 { return (fois_puissance_de_deux(1,nombre_bits)-1) ; }


//==============================================
// Coef
//==============================================

class Coef
 {
  public :
  
    void init( int bits )
     { bits_ = bits ; numerateur_ = 0 ; exposant_ = 0 ; }
 
    void approxime( double valeur )
     {
      numerateur_ = exposant_ = 0 ;
      if (valeur==0) { return ; }
      double min = (entier_max(bits_)+0.5)/2 ;
      while (valeur<min)
       {
        exposant_ = exposant_ + 1 ;
        valeur = valeur * 2 ;
       }
      numerateur_ = arrondi(valeur) ;
     }

    int lit_bits()
     { return bits_ ; }

    double approximation()
     { return (double(numerateur_)/fois_puissance_de_deux(1,exposant_)) ; }

    int multiplie( int arg )
     { return fois_puissance_de_deux(numerateur_*arg,-exposant_) ; }

  private :
  
    int bits_ ;
    int numerateur_ ;
    int exposant_ ;
 } ;


//==============================================
// tests
//==============================================

void erreur( int bits, double exact, double approx, int width, int resolution )
 {
  int err = arrondi(resolution*(exact-approx)/exact) ;
  if (err<0) err = -err ;
  if (err>resolution) err = resolution ;
  std::cout
   << bits << " bits : " << exact << " ~ "
   << std::setw(width) << approx
   << " ("<<err<<"/" << resolution << ")"
   << std::endl ;
 }
 
class TesteurCoef
 {
  public :
  
    void execute( int bits )
     {
      c_.init(bits) ;
      teste(0.65) ;
      teste(0.35) ;
     }
  
  private :
  
    Coef c_ ;
    
    void teste( double valeur )
     {
      c_.approxime(valeur) ;
      erreur(c_.lit_bits(),valeur,c_.approximation(),8,100) ;
     }
 } ;


class TesteurSomme
 {
  public :
  

    void execute( int bits )
     {
      c1_.init(bits) ;
      c2_.init(bits) ;
      teste(0.65,3515,0.35,4832) ;
     }
  
  private :
  
    Coef c1_, c2_ ;
    
    void teste( double c1, int e1, double c2, int e2 )
     {
      c1_.approxime(c1) ;
      c2_.approxime(c2) ;
      int approx = c1_.multiplie(e1) + c2_.multiplie(e2) ;
      int exact = arrondi(c1*e1+c2*e2) ;
      erreur(c1_.lit_bits(),exact,approx,4,1000) ;
     }
 } ;


//==============================================
// fonction principale
//==============================================

int main()
 {
  int bits ;
  TesteurCoef tc ;
  TesteurSomme ts ;

  std::cout<<std::endl ;
  for ( bits = 2 ; bits <= 8 ; bits = bits + 2 )
   { tc.execute(bits) ; }

  std::cout<<std::endl ;
  for ( bits = 1 ; bits <= 8 ; bits = bits + 1 )
   { ts.execute(bits) ; }

  std::cout<<std::endl ;
  return 0 ;
 }
