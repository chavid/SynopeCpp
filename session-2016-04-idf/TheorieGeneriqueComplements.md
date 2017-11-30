# [Compléments de programmation générique](TheorieGenerique.md)

\_\_FORCETOC\_\_

## Spécialisation partielle

Imaginons une classe template contenant un tableau `Type[N]` :

``` cpp
#include <iostream>

template <class Type, int N> class Tableau {
 public:
  Type tableau[N];
  Tableau(Type valeur, int index) {
    std::cout << "Cas général" << std::endl;
    tableau[index] = valeur;
  }
};

template <int N> class Tableau<int, N> {
 public:
  int tableau[N];
  Tableau(int valeur, int index) {
    std::cout << "1ere spécialisation partielle - Type = int" << std::endl;
    tableau[index] = valeur;
  }
};

template <class Type> class Tableau<Type, 10> {
 public:
  Type tableau[10];
  Tableau(Type valeur, int index) {
    std::cout << "2eme spécialisation partielle - N = 10" << std::endl;
    tableau[index] = valeur;
  }
};

template <> class Tableau<long, 5> {
 public:
  long tableau[5];
  Tableau(long valeur, int index) {
    std::cout << "Spécialisation totale - Type = long N = 5" << std::endl;
    tableau[index] = valeur;
  }
};

using Tableau_5_int = Tableau<int, 5> ;  // raccourci de syntaxe

template <class Type>
using Tableau_5 = Tableau<Type, 5> ;     // raccourci de syntaxe

int main() {
    Tableau<double, 15> tab(10.0, 0);
    std::cout << tab.tableau[0] << std::endl;
    Tableau<int, 20> tab_int(20, 0);
    std::cout << tab_int.tableau[0] << std::endl;
    Tableau<float, 10> tab_10(30.0, 0);
    std::cout << tab_10.tableau[0] << std::endl;
    Tableau<long, 5> tab_long_5(125, 0);
    std::cout << tab_long_5.tableau[0] << std::endl;
    Tableau_5_int tab_5_int(421, 0);
    std::cout << tab_5_int.tableau[0] << std::endl;
    Tableau_5<long> tab_5_long(123, 0);
    std::cout << tab_5_long.tableau[0] << std::endl;
    return 0;
}
```

    #!txt
    Cas général
    10
    1ere spécialisation partielle - Type = int
    20
    2eme spécialisation partielle - N = 10
    30
    Spécialisation totale - Type = long N = 5
    125
    1ere spécialisation partielle - Type = int
    421
    Spécialisation totale - Type = long N = 5
    123

  
  
© *INRIA & CNRS 2016*  
*Assemblé et rédigé par Vincent Rouvreau et David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
