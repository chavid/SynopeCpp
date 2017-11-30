# [Entrées/sorties standards](TheorieClassique.md)

\_\_FORCETOC\_\_

Le C++ standard comprend une bibliothèque d'entrées/sorties qui spécifie une interface commune pour tous les échanges de données avec l'extérieur, reposant notamment sur les opérateurs d'insertion \<\< et d'extraction \>\>.  
Accès à la [section correspondante des travaux pratiques](TpEntreesSortiesStandards.md).

## Canaux prédéfinis

Trois objets globaux (`std::cout`, `std::cerr` et `std::cin`) assurent la liaison avec les trois canaux prédéfinis d'Unix (`stdout`, `stderr` et `stdin`). Sur le canal d'entrée, les passages à la ligne sont ignorés (assimilés aux espaces et aux tabulations).

``` cpp
#include <iostream>

int main()
 {
  char rep ;
  std::cout << "bonjour\nca va ? (o/n) " ;           
  std::cin >> rep ;           

  if (rep=='o')
    std::cout << "tant mieux\n" ;           
  else if (rep=='n')
    std::cout << "tant pis\n" ;           
  else
    std::cerr << "réponse invalide\n" ;           

  return 0 ;
 }
```

## Format de sortie

Chaque canal possède une collection de bits permettant de contrôler son format d'affichage. La manipulation de cette collection se fait globalement à l'aide de la méthode `flags` ou sur une sous-partie par la méthode `setf`. Les opérateurs de bits sont souvent utiles.

``` cpp
#include <iostream>

int main()
 {
  std::cout.setf(std::ios::showpos) ;
  std::cout.setf(std::ios::fixed,std::ios::floatfield) ;
  std::cout.precision(2) ;

  std::cout.width(8) ;
  std::cout << 1.237 ;           
  std::cout.width(8) ;
  std::cout << -0.01 << '\n' ;          

  std::cout.width(8) ;
  std::cout << 1e-2 ;           
  std::cout.width(8) ;
  std::cout << 12. << '\n' ;          

  return 0 ;
 }
```

## Manipulateurs

Les manipulateurs sont des objets prédéfinis qui peuvent être insérés dans le flot d'entrée/sorties et agir sur les canaux.

``` cpp
#include <iostream>
#include <iomanip>

int main()
 {
  using namespace std ;

  cout.setf(ios::showpos) ;
  cout.setf(ios::fixed,ios::floatfield) ;

  cout
   << setprecision(2)           
   << setw(8) << 1.237 << setw(8) << -0.01 << '\n'       
   << setw(8) << 1e-2 << setw(8) << 12. << endl ;       

  return 0 ;
 }
```

## Fichiers textuels

Les fichiers textuels répondent à la même interface que les canaux prédéfinis. Seule différence, il faut construire les objets qui jouent les intermédiaires.

``` cpp
#include <iostream>
#include <fstream>
#include <string>

int main()
 {
  std::ofstream ofs("demo.txt") ;
  ofs << "bonjour\n" << 1 << " " << 2 << " " << 3.2 << '\n' ;     
  ofs.close() ;

  int i ;
  std::string entete ;
  std::ifstream ifs("demo.txt") ;
  ifs >> entete ;           
  std::cout << entete << '\n' ;          
  while (ifs >> i)           
    std::cout << i << '\n' ;          
  std::cout << std::flush ;           

  return 0 ;
 }
```

## Chaînes de caractères

On peut également dialoguer avec des tableaux de caractères avec la même interface. Si on veut utiliser de tels tableaux avec les fonctions qui attendent des chaînes de caractères, il ne faut pas oublier d'insérer le caractère nul marquant la fin de la chaîne.

``` cpp
#include <iostream>
#include <strstream>

int main( int argc, char * * argv )
 {
  if (argc!=2) return 1 ;

  int i ;
  std::istrstream iss(argv[1]) ; // atoi
  iss >> i ;                // atoi           
  std::cout << "numéro : " << i << '\n' ;         

  int const BUFFER_SIZE = 30 ;
  char buffer[BUFFER_SIZE] ;
  std::ostrstream oss(buffer,BUFFER_SIZE) ; // sprintf
  oss << "tempfile" << i << '\0' ;     // sprintf         
  std::cout << buffer << std::endl ;          

  return 0 ;
 }
```

## Lecture ligne à ligne

La lecture d'un fichier textuel ligne par ligne n'est pas prévue dans l'interface standard. Il faut l'implémenter soi-même.

``` cpp
#include <iostream>
#include <strstream>
#include <string>

int main()
 {
  std::string word ;
  char buffer[20] ;
  while (std::cin.getline(buffer,20,'\n'))
   {
    std::istrstream line(buffer) ;
    while (line>>word)
      std::cout << word << " " ;          
    std::cout << std::endl ;           
   }
  if (std::cin.eof()) std::cout << "EOF" << std::endl ;          
  else std::cout << "line too long ?" << std::endl ;          
  return 0 ;
 }
```

## Fichiers binaires

L'écriture ou la lecture sous forme binaire se fait à l'aide des méthodes `write` et `read` en manipulant les variables comme des tableaux d'octets.

``` cpp
#include <iostream>
#include <fstream>

int main()
 {
  double f = -1e2, g = 1e2, h = 0.001234 ;
  std::ofstream ofs("binaire.dat") ;
  ofs.write((char *)&f,sizeof(double)) ;
  ofs.write((char *)&g,sizeof(double)) ;
  ofs.write((char *)&h,sizeof(double)) ;
  ofs.close() ;

  double x ;
  std::ifstream ifs("binaire.dat") ;
  while (ifs.read((char *)&x,sizeof(double)))
    std::cout << x << '\n' ;          

  return 0 ;
 }
```

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
