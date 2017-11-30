# [Pointeurs améliorés de la bibliothèque standard](TheorieBonnesPratiquesBiblio.md)

On qualifie de pointeurs améliorés (smart), des objets qui se comportent comme des pointeurs (support des opérateurs `*` et `->`) mais qui rendent des services supplémentaires, notamment la destruction automatique des objets pointés.

Nous connaissons les problèmes posés par l'utilisation des pointeurs ordinaires (raw) :

  - la déclaration d'un pointeur ne précise pas si il désigne un objet

`  unique ou un tableau,`

  - on ne sait pas si le pointeur est "propriétaire" de l'objet pointé,

`  c'est à dire si il a en charge sa destruction,`

  - si il faut détruire l'objet pointé, on ne sait pas si il faut un

`  simple delete ou une procédure plus complexe, et si il s'agit`  
`  d'un objet unique ou d'un tableau,`

  - il est parfois difficile de garantir que la destruction n'est faite

`  qu'une seule fois, indépendamment du chemin emprunté par le programme`  
`  à l'exécution,`

  - il est impossible de tester si le pointeur référence un objet existant

`  ou déjà détruit.`

La bibliothèque standard s'efforce de fournir une collection de pointeurs améliorés qui permettent de ne jamais utiliser les pointeurs ordinaires.

## `std::unique_ptr`

Ce type de pointeurs, que nous appellerons "pointeurs propriétaires", ressemble au type `auto_ptr` de C++03, mais sans ses défauts.

#### Un pointeur "move-only"

Le pointeur propriétaire est conçu de telle manière que, à un instant donné, un objet donné ne soit référencé que par un seul pointeur de ce type. Pour ce faire, il exploite la "sémantique de déplacement" : il n'est pas copiable et il ne peut pas être affecté par l'affectation usuelle. En revanche, il peut être soumis au constructeur de déplacement ou à l'affectation par déplacement.

Grâce à cette propriété, il peut-être stocké en toute sécurité dans les conteneurs C++11 qui ont été réécrits en tenant compte de la sémantique de déplacement. Dans l'exemple ci-dessous, notez que si un pointeur est créé dans une instruction séparée (mot1), il faut le transformer en valeur temporaire avant de le confier au vecteur : les pointeurs propriétaires peuvent être déplacés mais non copiés.

``` cpp
  using string_ptr = std::unique_ptr<std::string> ;
  std::vector<string_ptr> mots ;
  
  string_ptr mot1(new std::string("world")) ;

  mots.push_back(std::move(mot1)) ;
  mots.push_back(string_ptr(new std::string("hello"))) ;
  
  std::reverse(mots.begin(),mots.end()) ;
  for ( string_ptr const & mot : mots )
   { std::cout<<(*mot)<<std::endl ; }
   
  return 0 ;
```

Dans ce qui suit, nous appellerons "pointeurs propriétaires" les instances de `std::unique_ptr`.

#### Performance

Un pointeur propriétaire n'est pas plus grand qu'un pointeur ordinaire, et pour la plupart des opérations il est aussi rapide. C'est donc un excellent choix par défaut.

#### Destruction personnalisée

La construction d'un `unique_ptr` permet également de fournir explicitement une fonction de destruction personnalisée.

``` cpp
#include <string>
#include <iostream>
#include <vector>

void delete_string( std::string * chaine )
 { std::cout << "Destruction de " << *chaine << std::endl ; delete chaine ; }

int main()
 {
  using string_ptr = std::unique_ptr<std::string,decltype(&delete_string)> ;
  std::vector<string_ptr> mots ;
  
  mots.push_back(string_ptr(new std::string("world"),delete_string)) ;
  mots.push_back(string_ptr(new std::string("hello"),delete_string)) ;
  
  std::reverse(mots.begin(),mots.end()) ;
  for ( string_ptr const & mot : mots )
   { std::cout<<(*mot)<<std::endl ; }
   
  return 0 ;
 }
```

#### Performance

Dans le cas d'un cas d'une fonction de destruction personnalisée, la taille du pointeur peut augmenter significativement.

#### Tableaux d'objets

Par curiosité, notons qu'il existe une forme d'`unique_ptr` capable de manipuler un tableau d'objets, au lieu d'un objet unique, et donc capable d'appeler la forme `delete []` lors de la destruction. Mais avec tous les conteneurs disponibles dans la bibliothèque standard, l'utilisation de tableaux ordinaires n'est ni utile ni recommandée.

## `std::shared_ptr`

Comme son nom le laisse deviner, contrairement au type `unique_ptr`, ce type permet à un même objet d'être référencé par plusieurs pointeurs. Aucun d'entre eux n'est alors pleinement propriétaire de l'objet. Sa gestion est basée sur un compteur de références et l'objet pointé est détruit lorsque le compteur tombe à zéro.

Par la suite, nous appellerons "pointeurs copropriétaires" les instances de `shared_ptr`.

#### Ramasse-miettes

On peut voir les pointeurs copropriétaires comme une façon d'implémenter la notion de "garbage collection", présente dans des langages de plus haut niveau, qui libèrent le programmeur de la gestion du cycle de vie des objets. Si tout objet est alloué dynamiquement puis manipulé à travers des pointeurs copropriétaires, il sera automatiquement détruit lorsque plus personne ne s'en sert, et nous avons une forme de nettoyage automatique.

``` cpp
#include <iostream>
#include <string>
#include <memory>

struct C { int * data ; } ;

void delete_int( int * p )
 { std::cout<<"Deleting an int"<<std::endl ; delete p ; }

void count( std::string const & name, std::shared_ptr<int> const & ptr )
 { std::cout<<name<<" count : "<<ptr.use_count()<<std::endl ; }

int main ()
 {
  std::cout<<"\n";
    
  std::shared_ptr<int> p1;
  std::shared_ptr<int> p2 (nullptr);
  count("p1",p1) ;
  count("p2",p2) ;

  std::cout<<"\n";

  std::shared_ptr<int> p3 (new int);
  std::shared_ptr<int> p4 (new int);
  count("p3",p3) ;
  count("p4",p4) ;
 
  std::cout<<"\n";

  std::shared_ptr<int> p5 (new int, delete_int);
  std::shared_ptr<int> p6 (p5);
  std::shared_ptr<int> p7 (std::move(p6));
  count("p5",p5) ;
  count("p6",p6) ;
  count("p7",p7) ;
  
  std::cout<<"\n";

  std::shared_ptr<int> p8 (std::unique_ptr<int>(new int));
  count("p8",p8) ;
  
  std::cout<<"\n";

  std::shared_ptr<C> obj (new C);
  std::shared_ptr<int> p9 (obj, obj->data);
  count("p9",p9) ;
  
  std::cout<<"\n";

  return 0;
```

#### Performance

Si vous êtes sensibles aux performances de votre application, prenez garde à ne pas surutiliser les pointeurs copropriétaires. En effet, ils ont un coup certain :

  - ils ont une taille doublée, comparée à un pointeur ordinaire, car ils pointent

`  également un "bloc de contrôle" qui contient notamment le nombre actuel de`  
`  références sur l'objet pointé`

  - la création du premier pointeur copropriétaire vers un objet donné implique

`  la création dynamique du "bloc de contrôle" associé à l'objet pointé.`

  - augmenter ou diminuer le nombre de références doit être fait de

`  façon thread-safe, donc un peu plus lente.`

#### Destruction personnalisée

Comme pour les pointeurs propriétaires, on peut spécifier un destructeur personnalisé, qui cette fois n'augmente pas la taille des pointeurs copropriétaires, car il est stocké avec le compteur de références.

#### Piège

On prendra garde à ne transformer un pointeur ordinaire donné (ou un pointeur unique) en pointeur copropriétaire qu'une seule fois, puis à ne réaliser des copies que de pointeur copropriétaire à pointeur copropriétaire. En effet, si vous initiez plusieurs pointeurs copropriétaires à partir du même pointeur ordinaire, vous créez plusieurs blocks de contrôle, ce qui conduira à une double destruction...

``` cpp
#include <iostream>
#include <string>
#include <memory>

int main ()
 {
  int * i1 = new int{1} ;
  //...
  std::shared_ptr<int> pa{i1} ;
  //...
  std::shared_ptr<int> pb{i1} ;
  //...
  return 0;
}
```

Pour ne pas être tenté de transformer ainsi plusieurs fois un pointeur ordinaire en pointeur copropriétaire, mettez directement le résultat d'un appel à new dans le constructeur de pointeur copropriétaire :

``` cpp
#include <iostream>
#include <string>
#include <memory>

int main ()
 {
  //...
  std::shared_ptr<int> pa{new int{1}} ;
  //...
  std::shared_ptr<int> pb{pa} ;
  //...
  return 0;
}
```

Ou encore mieux, utilisez la fonction `make_shared` décrite plus tard.

## `std::weak_ptr`

Les types `shared_ptr` et `unique_ptr` ne sont pas adaptés au cas d'un graphe d'objets qui se pointent mutuellement, en créant potentiellement des cycles. Pour ce cas (et pour quelques autres), on a besoin de pointeurs différents, qui ne soient pas propriétaires des objets pointés. C'est l'objectif de la classe `weak_ptr`. Nous les appellerons "pointeurs faibles".

#### Pointeurs non-propriétaires

Les pointeurs faibles ressemblent aux pointeurs copropriétaires, mais n'agissent pas sur le compteur de références de l'objet pointé. On pourrait être tenté d'utiliser des pointeurs ordinaires pour cette tâche, mais le pointeur faible possède un atout supplémentaire : il est capable de dire si l'objet pointé existe encore ou si il a été détruit, car il a accès à son "bloc de contrôle". Et pour avoir accès à ce bloc de contrôle, un pointeur faible se construit à partir d'un pointeur copropriétaire :

``` cpp
#include <iostream>
#include <memory>

struct Neurone 
 {
  Neurone() { std::cout<<"Construction"<<std::endl ; }
  ~Neurone() { std::cout<<"Destruction"<<std::endl ; }
 } ;

int main()
 {
  std::shared_ptr<Neurone> shptr(new Neurone) ;
  std::weak_ptr<Neurone> wptr(shptr) ;
  std::cout<<"Count : "<<shptr.use_count()<<std::endl ;
  std::cout<<"Expired ? "<<wptr.expired()<<std::endl ;
  shptr.reset() ;
  std::cout<<"Count : "<<shptr.use_count()<<std::endl ;
  std::cout<<"Expired ? "<<wptr.expired()<<std::endl ;
 }
```

Par ailleurs, les concepteurs de la bibliothèque standard on également voulu que les pointeurs améliorés soit thread-safe. A ce titre, il faut empêcher qu'on objet pointé disparaisse entre le moment ou on testerait son existence, et le moment ou on l'utiliserait. Das ce but, les pointeurs sont doté d'une méthode "atomique" qui teste l'existence et retourne un pointeur copropriétaire, lequel contient l'objet pointé si il existait, et rien sinon. En quelque sorte, la création du nouveau pointeur copropriétaire verrouille l'existence de l'objet pointé :

``` cpp
#include <iostream>
#include <string>
#include <memory>
#include <vector>

struct Neurone 
 {
  int id_ ;
  std::vector<std::weak_ptr<Neurone>> connexions_ ;
  
  explicit Neurone( int id ) : id_(id)
   { std::cout<<"Construction du neurone "<<id_<<std::endl ; }
  ~Neurone()
   { std::cout<<"Destruction du neurone "<<id_<<std::endl ; }
   friend std::ostream & operator<<( std::ostream & os, Neurone const & n )
    {
     os<<"neurone "<<n.id_<<", lié à" ;
     for ( std::weak_ptr<Neurone> c : n.connexions_ )
      {
       Neurone * p = c.lock() ;
       if (p) os<<" "<<p->id_ ;
      }
     return os ;
    }
 } ;

int main()
 {
  // crée
  std::vector<std::shared_ptr<Neurone>> v ;
  v.push_back(std::shared_ptr<Neurone>(new Neurone(0)));
  v.push_back(std::shared_ptr<Neurone>(new Neurone(1)));
  v.push_back(std::shared_ptr<Neurone>(new Neurone(2)));
  v.push_back(std::shared_ptr<Neurone>(new Neurone(3)));

  // connecte
  for ( std::shared_ptr<Neurone> n1 : v )
    for ( std::shared_ptr<Neurone> const & n2 : v )
      if ( n1->id_ != n2->id_ )
       { n1->connexions_.push_back(std::weak_ptr<Neurone>(n2)) ; }    

  // inspecte
  for ( std::shared_ptr<Neurone> neurone : v )
   {
    std::cout
      <<neurone.use_count()
      <<" pointeurs vers le "
      <<*neurone
      <<std::endl ;
   }   
  v.pop_back() ;
  v.erase(++(v.begin())) ;
  for ( std::shared_ptr<Neurone> neurone : v )
   {
    std::cout
      <<neurone.use_count()
      <<" pointeurs vers le "
      <<*neurone
      <<std::endl ;
   }   
}
```

#### Performance

Le test d'existence de l'objet pointé repose sur le bloc de controle associé à cet objet. Ce bloc contient non seulement le nombre de pointeurs copropriétaires qui pointe sur l'objet, mais également le nombre de pointeurs faibles. Lorsque le nombre de copropriétaires tombe à 0, l'objet est détruit, mais pas si bloc, qui reste aussi longtemps qu'il y a encore au moins un pointeur faible, ce qui permet à ces dernier de détecter que l'objet qu'ils pointent n'existe plus.

## `std::make_unique` et `std::make_shared`

A chaque fois que c'est possible, il est recommandé d'utiliser cette façon indirecte de créer les pointeurs, plutôt que d'appeler soi même l'opérateur `new`. Cela évite de devoir écrire deux fois le type de l'objet pointé, et cela évite une possible fuite de mémoire en cas d'exception.

``` cpp
std::unique_ptr<Widget> upw2(new Widget); // without make func
std::unique_ptr<Widget> upw1(std::make_unique<Widget>());    // with make func

std::shared_ptr<Widget> spw2(new Widget); // without make func
std::shared_ptr<Widget> spw1(std::make_shared<Widget>());    // with make func

processWidget(std::shared_ptr<Widget>(new Widget), priority()) ; // without make func
processWidget(std::make_shared<Widget>(), priority()) ; // with make func
```

A noter : `make_unique` n'apparait qu'avec C++14.

#### Performance

Par ailleurs, l'implémentation de `make_shared` est optimisée de façon à faire une seule allocation de mémoire à la fois pour l'objet pointé et pour son bloc de contrôle. En contre-partie, la mémoire contenant l'objet et le bloc ne peut etre rendue que lorsque tous les `shared_ptr` ET les tous les `weak_ptr` ont été détruits.

#### Limites

L'utilisation de `make_unique` ou `make_shared` n'est pas appropriée si on souhaite déclarer un destructeur personnalisé, si on a besoin d'utiliser la construction avec accolades (car ces fonctions utilisent la construction avec parenthèses), ou en présence d'objets très volumineux et de `weak_ptr` qui survivent longtemps à leurs objets.

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
