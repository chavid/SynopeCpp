# [Compléments](TheorieClassique "wikilink")

\_\_FORCETOC\_\_

## Espaces de noms (namespaces)

### Définition

Quand un programme commence à grossir, on se retrouve parfois à devoir nommer des variables, des fonctions, des définitions de type ou des classes avec le même nom. Ce qui n'est pas permis par le compilateur. Pour palier à cela, le C++ permet de regrouper des variables, des fonctions, des définitions de type et des classes dans un même espace de noms.

``` cpp
  std::cout << "Bonjour !" << std::endl;
```

`cout` et `endl` font partie du namespace `std`. Cet espace de nom regroupe toutes les fonctionnalités de la librairie standard.

### Création

Pour créer un espace de nom, il suffit de placer des variables, des fonctions, des définitions de type ou des classes entre les accolades d'un `namespace espace_de_nom { ... }`

``` cpp
#include <iostream>
#include <math.h>

namespace geometrie {
    namespace rond {
        double perimetre(double rayon) {
            return (2 * M_PI * rayon);
        }
    }  // namespace rond

    namespace carre {
        double perimetre(double cote) {
            return (4 * cote);
        }
    }  // namespace carre
}  // namespace geometrie

int main(int argc, char* argv[])
{
  std::cout << "Périmètre d'un carré de côté 3 = " << geometrie::carre::perimetre(3.0) << std::endl;
  std::cout << "Périmètre d'un rond de rayon 5 = " << geometrie::rond::perimetre(5.0) << std::endl;
  return 0;
}
```

    #!txt
    Périmètre d'un carré de côté 3 = 12
    Périmètre d'un rond de rayon 5 = 31.4159

### Raccourci

Pour éviter de taper tout le chemin du namespace à chaque fois, il est possible d'utiliser le mot clé `using`

``` cpp
#include <iostream>
#include <math.h>

namespace geometrie {
    namespace rond {
        double perimetre(double rayon) {
            return (2 * M_PI * rayon);
        }
    }  // namespace rond

    namespace carre {
        double perimetre(double cote) {
            return (4 * cote);
        }
    }  // namespace carre
}  // namespace geometrie

using namespace std;  // Plus besoin de taper std::
using namespace geometrie;  // Plus besoin de taper geometrie::
using namespace geometrie::carre;  // Plus besoin de taper geometrie::carre:: - /!\ Attention : perimetre par défaut est celui du carré

int main(int argc, char* argv[])
{
  cout << "Périmètre d'un carré de côté 3 = " << perimetre(3.0) << endl;  // Utilisation des raccourcis
  std::cout << "Périmètre d'un rond de rayon 5 = " << rond::perimetre(5.0) << std::endl;  // On peut continuer à nommer les namespaces std::, le compilateur s'en sort.
  return 0;
}
```

**Attention :** Il est vivement déconseillé d'utiliser `using namespace` dans les headers (\*.h ou \*.hpp). Sinon, lorsque l'on va inclure le header, on va embarquer de facto l'espace de nom. Ce qui peut générer des conflits difficilement identifiables.

### Les espaces de noms anonymes

``` cpp
#include <iostream>
#include <math.h>

namespace {  // espace de nom anonyme
    namespace rond {
        double perimetre(double rayon) {
            return (2 * M_PI * rayon);
        }
    }  // namespace rond

    namespace carre {
        double perimetre(double cote) {
            return (4 * cote);
        }
    }  // namespace carre
}  // namespace geometrie

int main(int argc, char* argv[])
{
  std::cout << "Périmètre d'un carré de côté 3 = " << carre::perimetre(3.0) << std::endl;
  std::cout << "Périmètre d'un rond de rayon 5 = " << rond::perimetre(5.0) << std::endl;
  return 0;
}
```

Cela permet de limiter les définitions au seul fichier où l'espace de nom est défini.

**Attention :** Il est vivement déconseillé d'utiliser les espaces de noms anonymes dans les headers (\*.h ou \*.hpp). Sinon, lorsque l'on va inclure le header, on va embarquer de facto l'espace de nom anonyme. Ce qui peut générer des conflits difficilement identifiables.

  
  
© *INRIA 2016*  
*Assemblé et rédigé par Vincent Rouvreau, revu par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
