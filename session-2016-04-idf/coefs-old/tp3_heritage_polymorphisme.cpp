
#include <iostream>
#include <iomanip>
#include <string>
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
// framework general de test
//==============================================

class Testeur
 {
  public :
  
    void init( int resolution )
     { resolution_ = resolution ; }

    virtual void execute( int )
     { std::cout << "Qu'est-ce que je fais la ??" << std::endl ; }
  
	void erreur( int bits, double exact, double approx, int width )
	 {
      int err = arrondi(resolution_*(exact-approx)/exact) ;
      if (err<0) err = -err ;
      if (err>resolution_) err = resolution_ ;
      std::cout
	   << bits << " bits : " << exact << " ~ "
	   << std::setw(width) << approx
	   << " ("<<err<<"/" << resolution_ << ")"
	   << std::endl ;
	 }

  private :
  
    int resolution_ ;
    
 } ;

void boucle( Testeur & t, int resolution, int deb, int fin, int inc )
 {
  int bits ;
  std::cout<<std::endl ;
  t.init(resolution) ;
  for ( bits = deb ; bits <= fin ; bits = bits + inc )
   { t.execute(bits) ; }
 }


//==============================================
// Coef
//==============================================

class Coef
 {
  public :
  
    void init( int bits )
     { bits_ = bits ; numerateur_ = 0 ; exposant_ = 0 ; }
 
    int lit_bits()
     { return bits_ ; }

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
// Testeurs dedies à Coef
//==============================================

class TesteurCoef : public Testeur
 {
  public :
  
    virtual void execute( int bits )
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
      erreur(c_.lit_bits(),valeur,c_.approximation(),8) ;
     }
 } ;

 
class TesteurSomme : public Testeur
 {
  public :
  

    virtual void execute( int bits )
     {
      c1_.init(bits) ;
      c2_.init(bits) ;
      teste(0.65,3515,0.35,4832) ;
     }
  
  private :
  
    Coef c1_, c2_ ;
    
    void teste( double c1, int e1, double c2, int e2 )
     {
      int exact, approx ;
      exact = arrondi(c1*e1+c2*e2) ;
      c1_.approxime(c1) ;
      c2_.approxime(c2) ;
      approx = c1_.multiplie(e1) + c2_.multiplie(e2) ;
      erreur(c1_.lit_bits(),exact,approx,4) ;
     }
 } ;

 
//==============================================
// fonction principale
//==============================================

int main()
 {
  TesteurCoef tc ;
  TesteurSomme ts ;
  boucle(tc,1000000,4,16,4) ;
  boucle(ts,1000,1,8,1) ;
  std::cout<<std::endl ;
  return 0 ;
 }

