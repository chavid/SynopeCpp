# [Variables de threads](TheorieFonctionnelleConcurrente "wikilink") (`thread_local`)

## Motivation

Les variables globales et statiques ont pour caractéristique habituelle de n'exister qu'en un seul exemplaire dans un programme. Cela en fait un moyen parfois pratique (et dangereux) pour permettre à des fonctions de communiquer par effet de bord, à travers ces variables, au lieu d'utiliser leurs entrées et leurs résultats. En règle générale, on déconseille cette pratique. C'est encore plus vrai dans le contexte d'une application multi-thread, puisque ces variables sont partagées entre tous les processus légers, et donc soumises à tous les aléas d'accès qui en découlent.

## Mot-clé `thread_local`

Dans certaines situations, et selon le rôle que tiennent ces variables dans un programme, on peut évacuer les problèmes de synchronisation en les dupliquant dans chaque thread. C'est le rôle du mot-clé `thread_local`. Nous les appelerons "variables de thread".

``` cpp
thread_local int x;                   // A thread-local variable at namespace scope
class X
{
  static thread_local std::string s;  // A thread-local static class data member
};
static thread_local std::string X::s; // The definition of X::s is required
void foo()
{
  thread_local std::vector<int> v;    // A thread-local local variable
}
```

## Construction

#### Variables globales et des variables de classe

Les variables de thread situées au niveau global ou dans une classe sont initialisées avant la première utilisation d'une variable de thread au sein de la même unité de compilation (fichier), mais la norme n'est pas beaucoup plus précise, et le moment exact dépend de l'implémentation et du contexte. Elles peuvent n'être jamais accédée et jamais initialisées. Elles peuvent l'être tardivement après le chargement dynamique d'un module.

#### Variables statiques de fonction

Les variables de thread situées au sein d'une fonction sont initialisées la première fois que l'exécution traverse leur déclaration, pour un processus léger donné. Si cette déclaration n'est jamais traversée, elle ne sera jamais initialisée, tout comme les variables statiques habituelles dans un programme séquentiel.

#### Mise à 0 et exceptions

Comme les variables statiques ordinaires, les variables de thread sont mise à 0 avant toute autre initialisation, et si la construction d'une variable de thread émet une exception, le programme s'arrête immédiatement via un appe à `std::terminate()`.

## Destruction

Les variables de threads sont détruites à la sortie du thread, dans l'ordre inverse de leur construction... mais l'ordre de construction n'est pas spécifié \! Evitez donc d'écrire un code qui dépend de l'ordre d'appel des destructeurs.

Tout comme pour la construction, toute exception déclenche un appel à `std::terminate()` et l'arrêt du programme.

Si un processus léger déclenche explicitement la fin du programme, notamment par un appel à `std::exit()`, ses variables de thread sont détruites, mais pas celles des autres processus légers qui seraient encore en cours d'exécution.

## Placement en mémoire

Les variables de thread ont des adresses différentes dans chaque processus léger. On peut en prendre l'adresse, comme pour n'importe quelle variable, et on peut même passer cette adresse à un autre processus pour lui permettre d'y accéder. Comme d'habitude dans ce genre de manipulation de pointeur, on doit s'assurer que le pointeur ne sera pas déréférencé après que la variable aura été détruite...

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
