# [C++11/14 : partage de données entre processus légers](TheorieFonctionnelleConcurrente.md)

Les threads d'un même programme peuvent parfois avoir à interagir au même moment avec les mêmes données, ce qui conduit à des effets indésirables.

Dans l'exemple suivant, où lequel les fonctions `calcul` et `affiche` sont confiées à deux threads différentes, il faut éviter que l'affichage n'intervienne entre l'incrémentation du nombre et le calcul du carré, sous peine d'un affichage erroné. Pour gérer ces situations, C++11 offre la notion de verrou, avec la classe `mutex` :

``` cpp
class A
 {
  public :
    void calcul()
     {
      verrou_.lock() ; // acquisition du verrou
      nombre_++ ;
      carre_ = nombre_*nombre_ ;
      verrou_.unlock() ; // libération du verrou
     }
    void affichage()
     {
      verrou_.lock() ; // acquisition du verrou
      std::cout<<nombre_<<" a pour carré "<<carre_<<std::endl ;
      verrou_.unlock() ; // libération du verrou
     }
  private :
    int nombre_, carre_ ;
    std::mutex verrou_ ; // ne peut être acquis que par un thread à la fois
 } ;
```

Le verrou empêche l'action simultanée de deux threads : tant qu'un des threads a bloqué le verrou, aucun autre thread ne peut le faire. Ceux qui essaient sont condamnés à attendre le débloquage du verrou.

Il faut noter que l'usage d'un verrou s'accompagne toujours d'une perte de performance, qui peut vite devenir importante. Les programmes multitâches doivent donc être conçus de façon à éviter au possible cette synchronisation en minimisant le partage de données entre tâches. Et lorsque l'usage d'un verrou est nécessaire, on doit limiter autant que possible la durée pendant laquelle ce verrou est bloqué.

Le code précédent n'est pas protégé contre la survenue d'une exception, qui pourrait laisser des verrous bloqués à l'infini. Comme pour toutes les ressources, on peut améliorer ce code à l'aide d'un objet chargé d'acquérir un verrou à la construction, et de le libérer à la destruction. Pour gérer les `mutex`, C++11 fournit les `lock_guard` :

``` cpp
class A
 {
  public :
    void calcul()
     {
      std::lock_guard<std::mutex> lg(verrou_) ;
      nombre_++ ;
      carre_ = nombre_*nombre_ ;
     }
    void affichage()
     {
      std::lock_guard<std::mutex> lg(verrou_) ;
      std::cout<<nombre_<<" a pour carré "<<carre_<<std::endl ;
     }
  private :
    int nombre_, carre_ ;
    std::mutex verrou_ ; // ne peut être acquis que par un thread à la fois
 } ;
```

Quelques remarques :

  - `mutex::try_lock()` permet de savoir si le mutex est verrouillé ou non.
  - `unique_lock` est une variante "déplacable" de `lock_guard`.
  - `recursive_mutex` : peut-etre acquis plus d'une fois (incrémentation d'un compteur) et libéré autant de fois (décrementation du compteur), pour les approches récursives.
  - `timed_mutex` : les méthodes `try_lock_for` et `try_lock_until` permettent de fixer une limite de temps pour l'acquisition du verrou.
  - `recursive_timed_mutex` : cumule les propriétés de `recursive_mutex` et `timed_mutex`.

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
