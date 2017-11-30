# [C++11/14 : compléments](TheorieBonnesPratiquesBiblio.md)

## Références universelles

Il y a un dernier comportement spécifique à connaître, et qui se produit dans le cas d'un patron de fonction ayant un paramètre `T`, et un argument qui soit **très exactement** `T&&`. Si la fonction est invoquée avec une valeur temporaire, le type déduit pour l'argument est bien `T&&`, mais il est également légal d'invoquer la fonction avec une valeur stable, et le type déduit pour l'argument est alors `T&`.

Donc, dans le cadre d'un patron de fonction, un argument de type `T&&` peut à la fois accepter une valeur stable ou temporaire. On qualifie alors `T&&`de "référence universelle". L'idée est de pouvoir écrire des patrons de fonction qui sachent manipuler à la fois des valeurs stables et temporaires, et tirer parti des possibilités de déplacement quand c'est possible. Prenons un exemple de patron de fonction :

``` cpp
template<typename T>
void foo(T&& t)
{}
```

Si on l'appelle en lui passant une valeur temporaire, alors `T` est déduit du même type que la valeur :

``` cpp
foo(42);            // Calls foo<int>(int&&)
foo(3.14159);       // Calls foo<double>(double&&)
foo(std::string()); // Calls foo<std::string>(std::string&&)
```

Par contre, si on l'appelle avec une valeur non temporaire, alors `T` et le type de l'argument sont considérés comme des références classiques :

``` cpp
int i=42;
foo(i); // Calls foo<int&>(int&)
```

Cela permet d'avoir un seul patron de fonction acceptant à la fois les valeurs temporaires et non temporaires, et capable de réaliser un déplacement plutôt qu'une copie quand c'est possible.

## std::forward

Dans le cas d'une fonction appelant une autre fonction, on fait de nouveau face au problème de préservation du caractère temporaire d'une valeur. Pour les fonctions ordinaires, std::move faisait l'affaire. Mais dans le cadre d'un patron de fonction et d'une référence universelle, on voudrait pouvoir préserver la propriété temporaire des variables temporaires, mais ne pas ajouter cette propriété aux variables stables. C'est précisemment le rôle de la fonction `std::forward`.

``` cpp
template<typename T>
void do_stuff(T&& t)
{
 ...
  do_other_stuff(std::forward<T>(t));
}
```

On parle alors de **suivi parfait** (perfect forwarding), car la fonction do\_other\_stuff recoit un argument qui est exactement du même type que la valeur initialement passée à do\_stuff.

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
