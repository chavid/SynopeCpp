# [Gestion des ressources dynamiques](TheorieBonnesPratiquesBiblio.md)

Scott Meyers : "Une ressource est quelque chose qui doit être rendu au système lorsque l'on a fini de s'en servir, sous peine d'ennuis. En C++, la ressource la plus classique est la mémoire allouée dynamiquement (si vous allouez de la mémoire sans jamais la relâchez, vous avez une fuite de mémoire), mais c'est loin d'être la seule. Parmi d'autres, on compte les descripteurs de fichier, les cadenas pour gérer le parallèlisme, les polices de caractères dans une interface utilisateur graphique, les connections vers une base de donnée, les connecteurs réseau. Quel que soit la ressource, il est important de la relâcher quand on en a plus l'usage. Effectuer cette tâche soi-même est déjà difficile, mais cela devient impossible quand on fait face à des exceptions, des fonctions à chemins multiples, voire à des programmeurs qui ne comprennent pas totalement l'impact de leurs modifications. L'utilisation d'objets pour gérer les ressources, bâtie sur les constructeurs, destructeurs et opérateurs d'assignation du C++, permet d'automatiser cette tâche."

## Motivation

Imaginons une bibliothèque de classes pour modéliser des particules, basée sur une classe racine, une fabrique (aléatoire pour l'exemple), et une fonction d'affichage.

``` cpp
#include <iostream>
#include <string>
#include <random>

class Particle
 {
  public :
    virtual ~Particle() =default ;
    virtual std::string const & name() const =0 ;
 } ;
class Electron : public Particle
 {
  public :
    virtual std::string const & name() const override
      { static std::string const name{"Electron"} ; return name ; }
 } ;
class Proton : public Particle
 {
  public :
    virtual std::string const & name() const override
      { static std::string const name{"Proton"} ; return name ; }
 } ;
class Neutron : public Particle
 {
  public :
    virtual std::string const & name() const override
      { static std::string const name{"Neutron"} ; return name ; }
 } ;
 
Particle * newParticle()
 {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1,3);
  switch (dis(gen))
   {
    case 1: return new Electron{} ;
    case 2: return new Proton{} ;
    case 3: return new Neutron{} ;
   }
  return nullptr ;
 }

void printInfo( Particle const & p )
 { std::cout<<p.name()<<std::endl ; }

void f()
 {
  Particle * p = newParticle() ;
  //...
  printInfo(p) ;
  //...
  delete p ;
 }

int main()
 {
  //...
  f() ;
  //...
 }
```

Le programmeur doit penser à détruire les particules fabriquées quand il a fini de s'en servir. De nombreuses raisons peuvent empêcher la destruction correcte de ces objets :

  - Un appel prématuré à return avant la fin.
  - Si l'appel à la fabrique et l'appel au destructeur sont

`  dans une boucle : un appel à break, continue ou goto.`

  - Une exception émise avant la fin.

Quand le delete n'est pas exécuté, non seulement on gèle la mémoire associée à l'objet, mais également toutes les ressources qu'il aurait pu acquérir.

Même un programmeur prudent et rigoureux peux être trompé, par exemple si une des fonctions appellées dans f() est "corrigée" par un tiers et se met à émettre des exceptions.

## Solution : l'idiome RAII

Pour s'assurer qu'une ressource est rendue en fin d'utilisation, on peut confier cette ressource à un objet dont le destructeur se chargera de libérer la ressource.

``` cpp
//...
class ParticleGuard
 {
  public :
    ParticleGuard( Particle * p ) : p_(p) {}
    ~ParticleGuard() { delete p_ ; }
  private :
    Particle * p_ ;
 } ;
//...
void f()
 {
  Particle * p = newParticle() ;
  ParticleGuard pg(p) ;
  //...
  printInfo(p) ;
  //...
 }
//...
```

L'utilisation d'objets pour gérer les ressources repose sur deux principes :

  -   - Les ressources fraichement obtenues sont immédiatement confiées à un objet de gestion de ressources. Comme il s'agit le plus souvent d'initialiser le gestionnaire avec les coordonnées de la ressource, on parle de **RAII** (Resource Acquisition Is Initialization).
      - Les objets de gestion de ressources utilisent leur destructeur pour libérer la ressource. En effet, quelque soit la façon dont on quitte la zone de validité de l'objet, on est certain que son destructeur sera appelé.

Attention, il faut éviter d'initialiser plusieurs gestionnaires de ressources avec la même ressource, sous peine de la libérer plusieurs fois. Le code ci-dessous est erroné :

``` cpp
//...
void f()
 {
  Particle * p = newParticle() ;
  ParticleGuard pg1(p) ;
  //...
  ParticleGuard pg2(p) ;
  //...
  printInfo(p) ;
  //...
 }
//...
```

Pour éviter ce genre d'erreur, on veille à confier la ressource à son gestionnaire dès sa création, sans passer par une variable intermédiaire, mais on se retrouve bloqué au moment d'utiliser la ressource...

``` cpp
//...
void f()
 {
  ParticleGuard pg(newParticle()) ;
  //...
  printInfo(???) ;
  //...
 }
//...
```

## Fournir un accès direct aux ressources

Les classes de gestion de ressources nous évitent les fuites de ressources, qui sont une des plaies principales des programmes C++. Dans un monde parfait, on aimerait que toutes les opérations sur les ressources passent par ces classes de gestion, mais si l'on ne veut pas avoir dupliquer l'intégralité de l'interface de la ressource au niveau de son gestionnaire, on doit fournir un accès direct à cette ressource.

#### Conversion explicite

Lors de la conception de vos classes de gestion de ressources, il faut donc toujours prévoir un moyen d'accès direct à la ressource. Par exemple, pour gérer l'accès à des polices de caractères :

``` cpp
...
class ParticleGuard
 {
  public:
    ParticleGuard( Particle * p ) : p_{p} {}
    Particle & get() { return *p_ ; }
    ~ParticleGuard() { delete p_ ; }
  private:
    Particle * p_ ;
 } ;

int main()
 {
  ParticleGuard pg(newParticle()) ;
  //...
  printInfo(pg.get()) ;
  //...
 }
```

#### Conversion automatique

Cependant, devoir appeler la fonction get() encore et encore peut devenir pénible, au point que les utilisateurs préfèrent se passer de la classe de gestion de ressources. Avant d'en arriver là, mieux vaut fournir un opérateur de conversion, qui peut être invoqué automatiquement par le compilateur quand c'est utile :

``` cpp
...
class ParticleGuard
 {
  public:
    ParticleGuard( Particle * p ) : p_{p} {}
    operator Particle &() { return *p_ ; }
    ~ParticleGuard() { delete p_ ; }
  private:
    Particle * p_ ;
 } ;

int main()
 {
  ParticleGuard p(newParticle()) ;
  //...
  printInfo(p) ;
  //...
 }
```

#### Piège et débat sur l'encapsulation

La possibilité de conversion automatique ouvre la porte à des erreurs d'utilisation. Par exemple, ci-dessous, on copie le pointeur de particule, pas forcémment à dessein, et si jamais p1 arrive en fin de vie, p2 va pointer dans le vide...

``` cpp
ParticleGuard p1(newParticle()) ;
//...
Particle * p2 = p1 ;
//...
```

On pourrait estimer que ces méthodes qui donnent accès à la ressource brute sont des violations de l'encapsulation. Ce n'est pas si grave : la classe de gestion de ressource n'a pas été crée pour encapsuler la ressource, mais pour s'assurer qu'elle serait libérée correctement. Si on le désire vraiment, on peut encapsuler le tout dans une couche supérieure.

## Les copies dans les classes de gestion de ressources

Nous avons vu comment créer des objets ParticleGuard, qui automatisent la destruction des objets Particle. Mais que se passe-t-il en cas de copie d'un de ces objets de type ? Plusieurs réponses sont possibles.

#### Interdire la copie

Dans de nombreux cas, cela n'a pas de sens de copier l'objet de gestion de ressources. Nous savons comment interdire une telle copie en C++11 :

``` cpp
class ParticleGuard :
 {
  public :
    ...
    ParticleGuard( ParticleGuard const & ) =delete ;
    ParticleGuard & operator=( ParticleGuard const & ) =delete ;
    ...
 } ;
```

#### Compter les références vers la ressource sous-jacente

Un peu plus compliqué, on peut faire en sorte de compter le nombre de cadenas qui partagent le même mutex, et faire en sorte de libérer le mutex quand ce nombre tombe à zéro.

``` cpp
#include <iostream>

class ParticleGuard
 {
  public:
  
    ParticleGuard( Particle * p ) // acquire resource
     : bloc_(new ParticleControlBloc(p))
     {}                         

    ParticleGuard( ParticleGuard const & pg )
     : bloc_(pg.bloc_)
     { bloc_->nb_ += 1 ; }                         

    ParticleGuard & operator=( ParticleGuard const & pg )
     {
      if (this==&pg) return *this ;
      bloc_->nb_ -= 1 ;
      if (bloc_->nb_==0)
        delete bloc_ ;
      bloc_ = pg.bloc_ ;
      bloc_->nb_ += 1 ;
      return *this ;
     }                         

    ~ParticleGuard() // release resource
     {
      bloc_->nb_ -= 1 ;
      if (bloc_->nb_==0)
        delete bloc_ ;
     }                

  private :
  
    struct ParticleControlBloc
     {
      ParticleControlBloc( Particle * p ) : p_(p) {}
      ~ParticleControlBloc() { delete p_ ; }
      unsigned int nb_{1} ;
      Particle * p_ ;
     } ;
  
    ParticleControlBloc * bloc_ ;
    
 } ;

int main()
 {
  ParticleGuard pg1(createParticle()) ;
  //...
  ParticleGuard pg2(pg1) ;
  //...
  return 0 ;
 }
```

#### Dupliquer la ressource sous-jacente

On préfère parfois dupliquer la ressource à chaque fois qu'on duplique l'objet qui la gère. Quand la ressource s'y prête. On parle alors de "copie profonde". Dans notre exemple, on l'on ne connait pas le type exact de l'objet Particle, il faut disposer d'une méthode `clone()` :

``` cpp
class Particle
 {
  //...
  virtual Particle * clone() const =0 ;
 } ;
class Electron : public Particle
 {
  //...
  virtual Particle * clone() const { return new Electron{*this} } ;
 } ;
//...
class ParticleGuard :
 {
  public :
    ...
    ParticleGuard( ParticleGuard const & pg )
     { p_ = pg.p_->clone() ; }
    ParticleGuard & operator=( ParticleGuard const & pg )
     { delete p_ ; p_ = pg.p_->clone() ; }
    ...
 } ;
```

#### Transférer la propriété de la ressource

Dans de rares cas, on veut autoriser la copie, tout en s'assurant qu'un seul objet référence la ressource à un instant donné. Dans ce cas, au moment de la copie, la responsabilité de la ressource est retirée à l'objet copié, et donnée à l'objet "copiant".

``` cpp
class ParticleGuard :
 {
  public :
    ...
    ParticleGuard( ParticleGuard & pg )
     { p_ = pg.p_ ; pg.p_ = nullptr ; }
    ParticleGuard & operator=( ParticleGuard & pg )
     { delete p_ ; p_ = pg.p_ ; pg.p_ = nullptr ; }
    ...
 } ;
```

#### A retenir

  - Copier un objet de gestion de ressource (RAII) amène à s'interroger sur la copie de la ressource elle-même. La stratégie de copie de la ressource dicte la stratégie de copie de l'objet de gestion de ressource.
  - Les stratégies de copie des RAII les plus communes sont l'interdiction ou le comptage des références, mais d'autres alternatives sont possibles.

<!-- end list -->

1.  Transformez `Boucle` en `Testeurs`, dotez là d'un itérateur et de méthodes `begin()` et `end()` pour pouvoir parcourir les testeurs, et transformez `Boucle::operator()` en fonction extérieure `boucle()` utilisant ces itérateurs. Vous pouvez vous inspirez de l'exemple de `FromTo`.

<!-- end list -->

    #!cpp
    ...
    template< unsigned int max >
    void boucle( Testeurs<max> & tt, int deb, int fin )
     {
      for ( auto t : tt )
       {
        if (!t) continue ;
        try
         {
    ...
    int main()
     {
      try
       {
        // tests du déplacement
        Testeurs<2> tt1{} ;
        tt1.enregistre(new EchecDepuisStable{"Créations d'echecs depuis une valeur stable"}) ;
        tt1.enregistre(new EchecDepuisTemporaire{"Créations d'echecs depuis une valeur temporaire"}) ;
        boucle(tt1,1,1000) ;
    
        // tests des coefs  
        Testeurs<5> tt2{} ;
        tt2.enregistre(new TesteurCoef<unsigned short>{"Test d'un coef"}) ;
        tt2.enregistre(new TesteurSomme<short>{"Test d'une expression"}) ;
        boucle(tt2,1,8) ;
    ...
    </source>
    #Remarquant que notre classe <code>Texte</code> ne modifie jamais la chaîne de caractères qu'on lui confie, réécrivez là de telle sorte que la chaîne de caractère de type <code>char *</code> soit gérée comme une ressource partagée par les objets <code>Texte</code>, de la même façon que les objets <code>ParticleGuard</code> partage une ressource de type <code>Particle *</code>. On mettra le <code>char *</code> dans une structure de contrôle partagée par les objets <code>Texte</code>.

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
