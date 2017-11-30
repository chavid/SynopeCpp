# [C++11 : les threads](TheorieFonctionnelleConcurrente "wikilink")

Depuis C++11, la bibliothèque standard C++ permet à un programmeur de lancer l'exécution d'une fonction en parallèle du programme actuel. On parle de processus légers ou threads.

## Lancement d'un thread

Depuis une fonction :

``` cpp
#include <thread>
.....
void f()
 { ..... }
.....
std::thread t(f) ;
```

Depuis un objet fonction :

``` cpp
#include <thread>
.....
class A
 {
  public :
    void operator()()
     { ..... }
 } ;
.....
std::thread t(A()) ;
```

Depuis une fonction membre :

``` cpp
#include <thread>
.....
class A
 {
  public :
    void f()
     { ..... }
 } ;
.....
A a ;
std::thread t(&A::f,&a) ;
```

Avec des arguments supplémentaires :

``` cpp
#include <thread>
.....
class A
 {
  public :
    void f( int n, double x )
     { ..... }
 } ;
.....
A a ;
int p ; double z ;
std::thread t(&A::f,&a,p,z) ;
```

## Synchronisation

Lorsqu'on a besoin d'attendre qu'un thread ait fini son exécution, on utilise la fonction membre `join` :

``` cpp
t.join() ; // attend la fin de t
```

Dans le cas contraire, il est fortement recommandé d'utiliser la fonction membre `detach` pour indiquer à la bibliothèque standard C++ qu'on accepte que le thread continue de s'exécuter en tâche de fond après la fin de la fonction actuelle. En effet, si au moment où un objet de type \`std::thread\` doit être détruit, le thread est associé toujours actif et `detach` n'a pas été appelé, le programme sera considéré comme incorrect et arrêté violemment par \`std::terminate\`...

Il existe aussi des manières d'interagir avec un thread en cours d'exécution. Par exemple, on peut mettre un thread "en sommeil" pour une durée indéterminée à l'aide de la méthode `sleep_for`. Ou on peut demander d'attendre qu'une condition donnée (variable de type `condition_variable`) soit réalisée.

Nous verrons plus loin que ces fonctionnalités de synchronisation sont souvent essentielles pour écrire des programmes multitâches corrects.

## Passage d'arguments par référence

ATTENTION : lors de la création d'une instance de `std::thread`, le temps de préparer le processus, les arguments de la fonctions sont dupliqués **par valeur**, avant d'être passés ultérieurement à la fonction confiée au processus. Si vous souhaitez passer des arguments par référence à la fonction, pour préserver leur qualification de référence, vous devez encapsuler la valeur transmise par `std::ref()`.

``` cpp
#include <thread>
.....
void f( int input, int & output )
 { ..... }
.....
int i1, o1 ;
.....
std::thread t(f,i1,std::ref(o1)) ;
```

A vous de modifier `mt_complex.cpp` en utilisant des instances de `std::thread` pour aboutir à une exécution parallèle plus rapide.

</pre>

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
