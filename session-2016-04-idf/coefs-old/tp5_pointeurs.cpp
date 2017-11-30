
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

class Boucle
 {
  public :
    void init( unsigned int max )
     {
      max_ = max ;
      indice_ = 0 ;
      testeurs_ = new Testeur * [max_] ;
      unsigned int i ;
      for ( i=0 ; i<max_ ; ++i )
       { testeurs_[i] = 0 ; }
     }
    void finalise()
     { delete [] testeurs_ ; }
    void enregistre( Testeur * t )
     {
      if (indice_==max_) { echec(2,"trop de testeurs") ; }
      testeurs_[indice_] = t ;
      indice_++ ;
     }
    void execute( int resolution, int deb, int fin, int inc )
     {
      unsigned int i ;
      for ( i=0 ; i<indice_ ; ++i )
       {
        int bits ;
        std::cout<<std::endl ;
        testeurs_[i]->init(resolution) ;
        for ( bits = deb ; bits <= fin ; bits = bits + inc )
         { testeurs_[i]->execute(bits) ; }
       }
     }
  private :
    Testeur * * testeurs_ ;
    unsigned int max_ ;
    unsigned int indice_ ;
 } ;


//==============================================
// Coef
//==============================================

template< typename U >
class Coef
 {
  public :
  
    void init( int bits )
     { bits_ = bits ; numerateur_ = 0 ; exposant_ = 0 ; }
 
    int lit_bits()
     { return bits_ ; }

    void operator=( double valeur )
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

    operator double()
     { return (double(numerateur_)/fois_puissance_de_deux(1,exposant_)) ; }

    U operator*( U arg )
     { return fois_puissance_de_deux(numerateur_*arg,-exposant_) ; }

  private :
  
    int bits_ ;
    U numerateur_ ;
    int exposant_ ;
 } ;


//==============================================
// Testeurs dedies a Coef
//==============================================

template< typename U >
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
  
    Coef<U> c_ ;
    
    void teste( double valeur )
     {
      c_ = valeur ;
      erreur(c_.lit_bits(),valeur,c_,8) ;
     }
 } ;


template< typename U >
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
  
    Coef<U> c1_, c2_ ;
    
    void teste( double c1, U e1, double c2, U e2 )
     {
      int exact, approx ;
      exact = arrondi(c1*e1+c2*e2) ;
      c1_ = c1 ;
      c2_ = c2 ;
      approx = c1_*e1 + c2_*e2 ;
      erreur(c1_.lit_bits(),exact,approx,4) ;
     }
 } ;


//==============================================
// fonction principale
//==============================================

int main()
 {
  TesteurCoef<unsigned short> tc ;
  TesteurSomme<short> ts ;
  Boucle boucle ;
  boucle.init(5) ;
  boucle.enregistre(&tc) ;
  boucle.enregistre(&ts) ;
  boucle.execute(1000,1,8,1) ;
  boucle.finalise() ;
  std::cout << std::endl ;
  return 0 ;
 }


