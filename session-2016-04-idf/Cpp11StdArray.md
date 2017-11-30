# [C++11 : std::array](TheorieBonnesPratiquesBiblio "wikilink")

## Motivation : améliorer la gestion des tableaux de taille fixe

Comme il est compliqué et dangeureux d'initialiser des tableaux de taille fixe ou des pointeurs, leur utilisation n'est plus recommandée. Depuis C++11, il est plutôt conseillé d'utiliser les `std::array` qui sont très faciles à initialiser, à condition de spécifier la taille et le type du tableau :

``` cpp
#include <array>

int main()
{
  std::array<double, 5> tableau_fixe_double { 1.2, 3.4, 5.0, 6.7, 8.9 };  // Ok.
  std::array<float, 5> tableau_fixe_float { 1.0, 2.0 };  // Ok, les valeurs non initialisées seront remplacées par la valeur par défaut : 0.0
  return 0;
}
```

On accède aux éléments d'un `std::array` avec la fonction `at()` ou l'opérateur `[]`

``` cpp
#include <iostream>
#include <array>
 
int main()
{
  std::array<double, 5> tableau_double { 1.2, 3.4, 5.6 };
  std::cout << "4eme valeur = " << tableau_double[3] << std::endl;  
  tableau_double[4] = 9.;
  tableau_double[10] = 1.1;  // Comportement non défini - pas d'exception
  std::cout << "5eme valeur = " << tableau_double.at(4) << std::endl;  // "4ieme valeur = 9.9"
  tableau_double.at(10) = 1.1;  // Exception std::out_of_range
  return 0;
}
```

## Size

Les `std::array` sont des tableaux de taille fixe, efficaces, et qui ne prennent pas plus de place en mémoire qu'un tableau de taille fixe classique.

``` cpp
#include <iostream>
#include <array>
#include <algorithm>    // for std::sort

template <long unsigned int N>
std::array<double,N> array_addition(const std::array<double,N>& a, const std::array<double,N>& b)
{
  std::array<double,N> result;
  for(std::size_t i = 0; i < a.size(); i++)
    result[i] = a[i] + b[i];
  return result;
}

int main()
{
  std::array<double, 5> tableau_1 { 1.2, 3.4, 5.6 };
  std::cout << "taille 1 = " << tableau_1.size() << std::endl;  // "taile 1 = 5" et non pas 3 !!

  std::array<double, 5> tableau_2 { 0.1, 0.2, 0.3, 1.4, 0.5 };
  std::cout << "taille 2 = " << tableau_2.size() << std::endl;  // "taile 2 = 5"
  
  std::cout << "tableau 3 = ";
  std::array<double, 5> tableau_3 = array_addition(tableau_1, tableau_2);
  for (auto d : tableau_3)
    std::cout << d << " ";
  std::cout << std::endl;  // "tableau 3 = 1.3 3.6 5.9 1.4 0.5"

  std::sort(tableau_3.begin(), tableau_3.end());
  std::cout << "tableau 3 trié = ";
  for (auto d : tableau_3)
    std::cout << d << " ";
  std::cout << std::endl;  // "tableau 3 trié = 0.5 1.3 1.4 3.6 5.9"

  return 0;
}
```

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
