
#include <iostream>
#include <iomanip>
#include <string>


//==============================================
// utilitaires
//==============================================

class Echec
 {
  public :
    Echec( unsigned int c, std::string const & comm )
     : code_(c), commentaire_(comm) {}
	  unsigned int code() const { return code_ ; }
	  std::string const & commentaire() const { return commentaire_ ; }
  private :
    unsigned int code_ ;
	  std::string commentaire_ ;
 } ;

int fois_puissance_de_deux( int nombre, int exposant )
 {
  if (nombre<0) { throw Echec(1,"cas imprevu") ; }
  if ((exposant<=-int(sizeof(int)<<3))||(exposant>=int(sizeof(int)<<3))) { throw Echec(1,"exposant trop grand") ; }
  if (exposant<0) { return (nombre>>-exposant) ; }
  if (nombre>(((unsigned int)(-1))>>exposant>>1)) { throw Echec(1,"overflow") ; }
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
  
    class EchecTropDeTesteurs ;
    class EchecIndiceIncorrect ;
    class EchecDivisionParZero ;
  
    static void init( unsigned int max ) ;
    static void enregistre( Testeur * t ) ;
    static unsigned int nb_testeurs() ;
    static Testeur * testeur( unsigned int i ) ;
    static void finalise() ;
	 
    Testeur( int resolution ) ;
    virtual void execute( int bits ) ;
    void erreur( int bits, double exact, double approx, int width ) ;

  private :
  
    static Testeur * * testeurs__ ;
    static unsigned int max__ ;
    static unsigned int indice__ ;

    int const resolution_ ;

 } ;

Testeur * * Testeur::testeurs__ = 0 ;
unsigned int Testeur::max__ = 0 ;
unsigned int Testeur::indice__ = 0 ;

class Testeur::EchecTropDeTesteurs : public Echec
 { public : EchecTropDeTesteurs() : Echec(2,"trop de testeurs") {} } ;

class Testeur::EchecIndiceIncorrect : public Echec
 { public : EchecIndiceIncorrect() : Echec(3,"indice de testeur incorrect") {} } ;
	
class Testeur::EchecDivisionParZero : public Echec
 { public : EchecDivisionParZero() : Echec(4,"division par 0") {} } ;
	
void Testeur::init( unsigned int max )
 {
  max__ = max ;
  indice__ = 0 ;
  testeurs__ = new Testeur * [max__] ;
  unsigned int i ;
  for ( i=0 ; i<max__ ; ++i )
   { testeurs__[i] = 0 ; }
 }
 
void Testeur::enregistre( Testeur * t )
 {
  if (indice__==max__) { throw EchecTropDeTesteurs() ; }
  testeurs__[indice__] = t ;
  indice__++ ;
 }
 
unsigned int Testeur::nb_testeurs()
 { return indice__ ; }
 
Testeur * Testeur::testeur( unsigned int i )
 {
  if (i>=indice__) { throw EchecIndiceIncorrect() ; }
  return testeurs__[i] ;
 }
 
void Testeur::finalise()
 { delete [] testeurs__ ; }

Testeur::Testeur( int resolution )
 : resolution_(resolution)
 { enregistre(this) ; }

void Testeur::execute( int )
 { std::cout << "Qu'est-ce que je fais la ??" << std::endl ; }

void Testeur::erreur( int bits, double exact, double approx, int width  )
 {
  if (exact==0) { throw EchecDivisionParZero() ; }
  int err = arrondi(resolution_*(exact-approx)/exact) ;
  if (err<0) err = -err ;
  if (err>resolution_) err = resolution_ ;
  std::cout
    << bits << " bits : " << exact << " ~ "
    << std::setw(width) << approx
    << " ("<<err<<"/" << resolution_ << ")"
    << std::endl ;
 }

void boucle( int deb, int fin, int inc )
 {
  unsigned int i ;
  for ( i=0 ; i<Testeur::nb_testeurs() ; ++i )
   {
    try
     {
	    Testeur & t = *Testeur::testeur(i) ;
      std::cout<<std::endl ;
	    int bits ;
      for ( bits = deb ; bits <= fin ; bits = bits + inc )
       { t.execute(bits) ; }
     }
    catch ( Echec const & e )
     { std::cout<<"[ERREUR "<<e.code()<<" : "<<e.commentaire()<<"]"<<std::endl ; }
   }
 }


//==============================================
// Coef
//==============================================

template< typename U >
class Coef
 {
  public :
  
    Coef( unsigned int bits ) ;
    unsigned int lit_bits() const ;
    void operator=( double valeur ) ;
    operator double() const ;
    U operator*( U arg ) const ;
    U numerateur() const ;
    int exposant() const ;

  private :
  
    unsigned const int bits_ ;
    U numerateur_ ;
    int exposant_ ;
 } ;

void affiche( Coef<short> const & c )
 { std::cout << c.numerateur() << "/2^" << c.exposant() ; }

template< typename U >
Coef<U>::Coef( unsigned int bits )
 : bits_(bits), numerateur_(0), exposant_(0)
 {}
 
template< typename U >
unsigned int Coef<U>::lit_bits() const
 { return bits_ ; }

template< typename U >
void Coef<U>::operator=( double valeur )
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

template< typename U >
Coef<U>::operator double() const
 {
  if (exposant_<0) { throw Echec(5,"exposant negatif") ; }
  return (double(numerateur_)/fois_puissance_de_deux(1,exposant_)) ;
 }

template< typename U >
U Coef<U>::operator*( U arg ) const
 { return fois_puissance_de_deux(numerateur_*arg,-exposant_) ; }
 
template< typename U >
U Coef<U>::numerateur() const
 { return numerateur_ ; }
 
template< typename U >
int Coef<U>::exposant() const
 { return exposant_ ; }


//==============================================
// Testeurs dedies a Coef
//==============================================

template< typename U >
class TesteurCoef : public Testeur
 {
  public :
  
    TesteurCoef( int resolution )
     : Testeur(resolution)
     {}

    virtual void execute( int bits )
     {
      teste(bits,0.65) ;
      teste(bits,0.35) ;
     }
  
  private :
  
    void teste( int bits, double valeur )
     {
      Coef<U> c(bits) ;
      c = valeur ;
      erreur(bits,valeur,c,8) ;
     }
 } ;


template< typename U >
class TesteurSomme : public Testeur
 {
  public :

    TesteurSomme( int resolution )
     : Testeur(resolution)
     {}

    virtual void execute( int bits )
     { teste(bits,0.65,3515,0.35,4832) ; }

  private :
  
    void teste( int bits, double c1, U e1, double c2, U e2 )
     {
      Coef<U> coef1(bits), coef2(bits) ;
      int exact, approx ;
      exact = (int)(c1*e1+c2*e2) ;
      coef1 = c1 ;
      coef2 = c2 ;
      approx = coef1*e1 + coef2*e2 ;
      erreur(bits,exact,approx,4) ;
     }
 } ;


//==============================================
// fonction principale
//==============================================

int main()
 {
  try
   {
    Testeur::init(5) ;
    TesteurCoef<unsigned short> tc(100) ;
    TesteurSomme<short> ts(1000) ;
    boucle(1,8,1) ;
    Testeur::finalise() ;
    std::cout<<std::endl ;
    return 0 ;
   }
  catch ( Echec const & e )
   {
    std::cout<<"[ERREUR "<<e.code()<<" : "<<e.commentaire()<<"]"<<std::endl ;
	  return e.code() ;
   }
 }


