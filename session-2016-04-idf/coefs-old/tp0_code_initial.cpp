
#include <iostream>
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
// fonction principale
//==============================================

int main()
 {
  std::cout << std::endl ;
  std::cout << "fois_puissance_de_deux(1,3) : "
   << fois_puissance_de_deux(1,3) << std::endl ;
  std::cout << "fois_puissance_de_deux(3,2) : "
   << fois_puissance_de_deux(3,2) << std::endl ;
  std::cout << "fois_puissance_de_deux(10,-1) : "
   << fois_puissance_de_deux(10,-1) << std::endl ;
 
  std::cout << std::endl ;
  std::cout << "arrondi(-0.75) : " << arrondi(-0.75) << std::endl ;
  std::cout << "arrondi(-0.25) : " << arrondi(-0.25) << std::endl ;
  std::cout << "arrondi(+0.25) : " << arrondi(+0.25) << std::endl ;
  std::cout << "arrondi(+0.75) : " << arrondi(+0.75) << std::endl ;
 
  std::cout << std::endl ;
  
  int denominateur1 = fois_puissance_de_deux(1,1) ;
  int numerateur1 = arrondi(0.65*denominateur1)  ;
  std::cout << "0.65 ~ " << numerateur1 << "/" << denominateur1 << std::endl ;
  
  int denominateur2 = fois_puissance_de_deux(1,2) ;
  int numerateur2 = arrondi(0.65*denominateur2)  ;
  std::cout << "0.65 ~ " << numerateur2 << "/" << denominateur2 << std::endl ;
  
  int denominateur3 = fois_puissance_de_deux(1,3) ;
  int numerateur3 = arrondi(0.65*denominateur3)  ;
  std::cout << "0.65 ~ " << numerateur3 << "/" << denominateur3 << std::endl ;
  
  std::cout << std::endl ;
  int exact = arrondi(0.65*3515) ;
  std::cout << "0.65*3515 = " << exact << " ~ " << fois_puissance_de_deux(numerateur1*3515,-1) << std::endl ;
  std::cout << "0.65*3515 = " << exact << " ~ " << fois_puissance_de_deux(numerateur2*3515,-2) << std::endl ;
  std::cout << "0.65*3515 = " << exact << " ~ " << fois_puissance_de_deux(numerateur3*3515,-3) << std::endl ;
 
  std::cout << std::endl ;
  std::cout << "entier_max(8) : " << entier_max(8) << std::endl ;
  std::cout << "entier_max(16) : " << entier_max(16) << std::endl ;
  std::cout << "entier_max(32) : " << entier_max(32) << std::endl ;

  std::cout << std::endl ;
  return 0 ;
 }


