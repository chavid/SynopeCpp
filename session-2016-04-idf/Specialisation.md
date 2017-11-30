# [Complications liées aux possibilités de spécialisation](TheorieGenerique.md)

La possibilité de spécialiser un patron offre un moyen exceptionnellement souple de traiter les cas particuliers, sans obscurcir le cas général. On peut tout faire dans une spécialisation, y compris revoir totalement l'interface d'un patron de classe, et les variantes spécialisées n'ont pas besoin d'être regroupées avec le cas général, et peuvent surgir "par surprise". En conséquence, il y a des situations où le compilateur ne veut pas se risquer à faire des hypothèses sur l'interface d'un patron qui n'est pas encore entièrement connu, et il a besoin d'être "accompagné" par le programmeur.

## Les deux rôles de typename

#### Premier rôle

Quelle différence entre les deux déclarations ci-dessous ?

``` cpp
template<class T> class Widget;                 // uses "class"

template<typename T> class Widget;              // uses "typename"
```

Du point de vue du compilateur C++, aucune. Le choix est affaire de gouts et de conventions. Certains programmeurs preferent `class` par habitude ou parce qu'il est plus rapide à taper. D'autres préfèrent `typename` parce qu'il souligne le fait qu'il peut s'agir aussi d'une classe que d'un type prédéfini. D'autres encore se servent parfois de `typename`, et parfois de `class` pour souligner qu'un patron est fait pour des classes, plutôt que pour des types prédéfinis.

#### Second rôle

Le mot clé `typename` sert également à aider le compilateur quand il ne sait pas si un identifiant donné désigne une variable ou un type. Prenons cet exemple :

``` cpp
template<typename C>
void print2nd( const C & container )
 {
  if (container.size() >= 2)
   {
     C::const_iterator iter(container.begin()) ;
     ++iter ;
     int value = *iter ;
     std::cout << value ;
   }
 }
```

Le type de `iter` est `C::const_iterator`. Comme ce nom dépend d'un paramètre du patron, on dit qu'il est "dépendant", et comme il est défini au sein d'une classe, on le dit "imbriqué et dépendant". Ce genre de nom peut poser des problèmes au compilateur. Imaginons que `print2nd` ressemble à ceci :

``` cpp
template<typename C>
void print2nd( const  C& container )
 {
  C::const_iterator * x ;
  ...
 }
```

Il peut s'agir d'une déclaration de variable locale `x` de type `C::const_iterator`. Mais, si le type `C` dispose d'une variable statique publique nommée `const_iterator`, il peut aussi s'agir d'une multiplication de `const_iterator` par `x`... le compilateur n'a aucun moyen de le savoir, tant que `C` n'est pas connu. Pour résoudre cette ambiguité, le compilateur considère par défaut qu'il s'agit d'une variable, à moins que le programmeur lui dise explicitement qu'il s'agit d'un type, à l'aide du mot-clé `typename`.

Dans ces conditions, le code que nous avons présenté au début ne compilera pas. Le code valide est le suivant :

``` cpp
template<typename C>
void print2nd( const C & container )
 {
  if (container.size() >= 2)
   {
    typename C::const_iterator iter(container.begin()) ;
    ...
  }
 }
```

Si notre `const_iterator` n'est pas directement imbriqué dans un type inconnu `C`, mais dans un un type dépendant de `C`, comme `MaClasse`<C>, et même si `MaClasse`, on n'est jamais à l'abir d'une spécialisation "sale" qui change la nature de `const_iterator`, car en cas de spécialisation tout est permis :

``` cpp
 
template<typename U> // patron maître
struct MaClasse
 {
  typedef U const_iterator ;
 } ;

template<typename T> // spécialisation
struct MaClasse<const T>
 {
  static int const_iterator = 0 ;
 } ;
```

Bref, mot-clé `typename` doit précéder tous les noms imbriqués et dépendants. Mais seulement ceux-là :

``` cpp
template<typename C>               // typename autorisé
void f( const C & container,       // typename interdit (C n'est pas un nom imbriqué)
     typename C::iterator iter) ;  // typename requis   (iterator est imbriqué et dépendant)
```

La règle comprend deux exceptions. Parce qu'il n'y a pas d'ambiguité possible, il y a deux emplacements où les noms imbriqués et dépendants **doivent** être utilisés sans `typename` :

  - la liste des classes de base d'une classe
  - la liste d'initialisation des membres d'un constructeur

<!-- end list -->

``` cpp
template<typename T>
class Derived: public Base<T>::Nested // typename interdit
 {                       
  public:                             
    explicit Derived(int x)
    : Base<T>::Nested(x)              // typename interdit
     {                                
      typename Base<T>::Nested temp ; // typename requis
      .....
 } ;
```

Un dernier exemple d'utilsiation de `typename` auquel vous pourriez être confrontés :

``` cpp
template<typename IterT>
void workWithIterator( IterT iter )
 {
  typename std::iterator_traits<IterT>::value_type temp(*iter);
  ...
 }
```

L'utilisation de `std::iterator_traits`<IterT>`::value_type` est la façon standard de récupérer le type des valeurs pointées par l'itérateur, en utilisant une classe de traits. L'instruction ci-dessus permet de créer de créer un objet temporaire et de lui assigner la valeur pointée par l'itérateur. Si `IterT` est `std::list`<std::string>`::iterator`, alors `temp` sera de type `std::string`. Parce que le nom `std::iterator_traits`<IterT>`::value_type` est à la fois imbriqué dans `std::iterator_traits`<IterT> et dépendant de `IterT`, il faut le faire précéder de `typename`.

Comme il est long de taper un tel nom, une pratique commune consiste à créer un alias local de même nom :

``` cpp
template<typename IterT>
void workWithIterator( IterT iter )
 {
  typedef typename std::iterator_traits<IterT>::value_type value_type ;
  value_type temp(*iter) ;
  ...
 }
```

## Accès aux éléments des classes de base paramétrées

Prenons l'exemple d'une application servant à envoyer des messages à différentes compagnies, ces messages pouvant être cryptés ou non :

``` cpp
class CompanyA
 {
  public :
    ...
    void sendCleartext( const std::string & msg ) ;
    void sendEncrypted( const std::string & msg ) ;
    ...
 } ;

class CompanyB
 {
  public :
    ...
    void sendCleartext( const std::string & msg ) ;
    void sendEncrypted( const std::string & msg ) ;
    ...
 } ;
...                                          // classes for other companies

class MsgInfo { ... } ;                      // class for holding information
                                             // used to create a message
template <typename Company>
class MsgSender
 {
  public :
    ...                                      // ctors, dtor, etc.

    void sendClear( const MsgInfo & info )
     {
      std::string msg ;
      ...                                    // create msg from info ;

      Company c ;
      c.sendCleartext(msg) ;
     }

   void sendSecret( const MsgInfo & info )   // similar to sendClear, except
     { ... }                                 // calls c.sendEncrypted
 } ;
```

Imaginons maintenant que nous souhaitons ajouter, à l'aide d'une classe dérivée, la possibilité de garder une trace de chaque envoi de message :

``` cpp
template <typename Company>
class LoggingMsgSender : public MsgSender<Company>
 {
  public :
    ...                                    // ctors, dtor, etc.
    void sendClearMsg( const MsgInfo & info )
     {
      .....                                // write "before sending" info to the log ;
      sendClear(info) ;                    // call base class function : this code will not compile!
      .....                                // write "after sending" info to the log;
     }
    ...
 } ;
```

Nous avons choisi un nom de méthode (`sendClearMsg`) différent de celui de la classe de base (`sendClear`) afin d'éviter de cacher les noms hérités et afin d'éviter de redéfinir une méthode héritée non-virtuelle. Et pourtant, ce code n'est pas valide, et les compilateurs conforme à la norme prétendront que `sendClear` est introuvable. Pourquoi ? Parce que selon la valeur de `Company`, la définition de `MsgSender`<Company> pourrait être une spécialisation qui ne possède pas de méthode `sendClear`.

Supposons une classe `CompanyZ` qui exige des communicaitons cryptées :

``` cpp
class CompanyZ
 {                                              // this class offers no
  public :                                      // sendCleartext function
    ...
    void sendEncrypted( const std::string & msg ) ;
    ...
 } ;
```

La définition générale de `MsgSender` ne convient pas, parce que sa méthode `sendClear` n'a pas de sens pour la classe `CompanyZ`. on peut répondre à ce problème en fournissant une définition totalement spécialisée de `MsgSender` pour `CompanyZ` (à l'aide de la notation `<>` :

``` cpp
template <>                                 
class MsgSender<CompanyZ>                    // a total specialization of
 {                                           // MsgSender; the same as the
  public :                                   // general template, except
    ...                                      // sendCleartext is omitted
    void sendSecret( const MsgInfo & info )
     { ... }
 } ;
```

A son tour, la définition générale de `LoggingMsgSender` devient inopérante ... à cause des possibilités de spécialisation, le compilateur refuse toute hypothèse à propos de l'interface `MsgSender`, et refuse de présumer l'existence de `sendClear` dans la classe paramétrée `MsgSender`, même pour des paramètres autres que `CompanyZ`. En quelque sorte, en croisant la route de la programmation générique, l'héritage s'arrête (voir règle [1](Regle01.md)). Pour restaurer cet héritage, il y a trois moyens.

Le premier, appeler la méthode hériter à travers le nom de l'objet courant `this` :

``` cpp
template <typename Company>
class LoggingMsgSender : public MsgSender<Company>
 {
  public :
    .....
    void sendClearMsg(const MsgInfo& info)
     {
      .....                  
      this->sendClear(info) ;
      .....                  
     }
    .....
 } ;
```

Le second, utiliser la directive `using`, déjà utilisée pour exposer des méthodes cachées par une redéfinition (voir règle [33](Regle33.md)) :

``` cpp
template <typename Company>
class LoggingMsgSender : public MsgSender<Company>
 {
  public :
    using MsgSender<Company>::sendClear ;
    .....
    void sendClearMsg(const MsgInfo& info)
     {
      .....                  
      sendClear(info) ;
      .....                  
     }
    .....
 } ;
```

Le troisième, préfixer l'appel explicitement avec le nom de la classe de base :

``` cpp
template <typename Company>
class LoggingMsgSender : public MsgSender<Company>
 {
  public :
    .....
    void sendClearMsg(const MsgInfo& info)
     {
      .....                  
      MsgSender<Company>::sendClear(info) ;
      .....                  
     }
    .....
 } ;
```

Ce dernier choix n'est pas le meilleur, parce que si la méthode appelée est virtuelle, l'utilisation de cet opérateur inhibe la liaison dynamique. Dans les trois approches, il s'agit de rassurer le compilateur en lui promettant que toutes les éventuelles spécialisations de `MsgSender` comprendront une méthode `sendClear`. Si la promesse n'est pas tenue, une erreur de compilation viendra le sanctionner plus tard. Par exemple, si un code ultérieur est le suivant :

``` cpp
LoggingMsgSender<CompanyZ> zMsgSender ;
.....
MsgInfo msgData ;
.....
zMsgSender.sendClearMsg(msgData) ; // error! won't compile
```

L'appel à `sendClearMsg` ne compilera pas. On pourrait considérer cette détection comme suffisante, mais les auteurs de C++ préférant une détection la plus précoce possible des erreurs, ils préfèrent ne faire aucune hypothèse sur l'interface des classes de base paramétrées.

## A retenir

  - Dans la déclaration de paramètres de patrons, on peut utiliser indiféremment `class` ou `typename`.
  - Utilisez `typename` pour qualifier les noms de type imbriqués dépendants, excepté dans la liste des classes de base ou pour la classe de bases dans les listes d'initialisation.
  - Dans les classes dérivées paramétrées (patrons), on peut accéder aux éléments des classes de base paramétrées à l'aide du préfixe `this->`, à l'aide de déclarations `using`, ou bien en préfixant explicitement avec le nom de la classe de base et l'opérateur de portée `::`.

`2. Dans main(), emplacez le !TesteurCoef`<unsigned short>` par un !TesteurCoef`<unsigned char>`. Quel est le problème ? Spécialisez l'operator<< pour le traitement des Coef`<unsigned char>`.`

#### Solution

  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef9.cpp>
  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef9.ref>

</source>
