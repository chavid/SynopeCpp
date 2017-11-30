# [Marqueurs et traits](TheorieGenerique.md)

## Motivation

Un des petits utilitaires de la bibliothèque standard est la fonction `advance`, qui déplace un itérateur d'un nombre donné de pas :

``` cpp
template<typename IterT, typename DistT>       // move iter d units
void advance( IterT & iter, DistT d ) ;        // forward; if d < 0,
                                               // move iter backward
```

Conceptuellement, il s'agit simplement de faire `iter+=d`, mais l'opérateur `+=` n'est disponible que pour les itérateurs à accès direct. Pour les autres, il faut appliquer `++` ou `--` autant de fois que nécessaire.

## Itérateurs et marqueurs de la bibliothèque standard

Il y a cinq catégories d'itérateurs, offrant des fonctionnalités croissantes :

  - Les **itérateurs d'entrée** ne peuvent qu'avancer, un pas à la fois, ne peuvent que

`  lire ce qu'ils pointent, et ne peuvent le lire qu'une fois. Ils sont conçus à l'image`  
`  du pointeur de lecture d'un fichier. istream_iterator est représentatif de`  
`  cette catégorie.`

  - Les **itérateurs de sortie** sont analogues à ceci qu'ils ne peuvent qu'écrire ce

`  qu'ils pointent. ostream_iterator est représentatif de cette catégorie. istream_iterator`  
`  et ostream_iterator sont très limités et ne conviennent qu'aux algorithmes à une seule passe.`

  - Les **itérateurs avant** cumulent les capacités des deux précédents, et peuvent lire/écrire

`  les données pointées plusieurs fois. Ils conviennent donc aux algorithmes à multiples passes.`  
`  Les listes simplement chaînées forward_list et les conteneurs`  
`  avec table de hachage tels que unordered_set fournissent des`  
`  itérateurs avant.`

  - Les **itérateurs bidirectionnels** ajoutent la capacité de faire marche arrière. On trouve dans

`  cette catégorie les itérateurs des conteneurs standards list, set, multiset, map,`  
`  et multimap.`

  - Les **itérateurs à accès direct** ajoutent la capacité de faire des bonds en avant et en arrière,

`  en temps constant, de façon comparable à l'arithmétique des pointeurs ordinaires. Emargent à cette`  
`  catégorie les itérateurs des classes vector, deque, et string.`

Pour chacune de ces catégories, la bibliothèque standard définit une structure vide, que nous qualifierons de "marqueur" :

    struct input_iterator_tag {} ;
    struct output_iterator_tag {} ;
    struct forward_iterator_tag: public input_iterator_tag {} ;
    struct bidirectional_iterator_tag: public forward_iterator_tag {} ;
    struct random_access_iterator_tag: public bidirectional_iterator_tag {} ;

L'héritage ci-dessus exprime bien une relation "est une sorte de", et nous verrons plus tard comment s'en servir.

## Création de traits

On pourrait écrire `advance()` en nous basant sur le plus petit comportement commun des itérateurs, mais cela serait dommage d'avoir tout le temps une exécution en temps linéaire, alors que pour les itérateurs à accès direct nous pourrions bénéficier d'une exécution en temps constant. Ce que nous aimerions, en pseudo-code :

``` cpp
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d)
 {
  if (iter is a random access iterator)
   {
     iter += d;                                      // use iterator arithmetic
   }                                                  // for random access iters
  else
   {
    if (d >= 0) { while (d--) ++iter ; }              // use iterative calls to
    else { while (d++) --iter ; }                     // ++ or -- for other
   }                                                  // iterator categories
 }
```

Cela demande de déterminer si `iter` est un itérateur à accès direct, donc si `IterT` est un iterateur à accès direct, donc d'obtenir des informations sur le type `IterT` au moment de la compilation. C'est l'objectif de la technique des **traits**, avec une exigence supplémentaire : pouvoir les utiliser également avec les types prédéfinis du langage, notamment les pointeurs ordinaires. Pour revenir à l'exemple, on aimerait qu'`advance()` soit capable de déplacer des pointeurs tels que `const char *`.

A cause de la dernière contrainte, impossible d'ajouter des informations dans les types eux-même (on ne peut rien ajouter à un pointeur ordinaire), donc cette information doit être extérieure aux types. La technique classique consiste à créer un patron, paramétré par le type à décrire, ainsi qu'un jeu de spécialisations. Pour les itérateurs de la bibliothèque standard, le patron s'appelle `iterator_traits` :

``` cpp
template<typename IterT>  // template for information about
struct iterator_traits ;  // iterator types
```

La convention veut qu'il s'agisse d'une structure (éléments publics par défaut), que son nom comprenne "traits", et qu'elle contienne des éléments accessible à travers l'opérateur de classe, tels que des typedefs ou des variables statiques. Dans le cas d' `iterator_traits`, on trouve un `typedef` nommé `iterator_category` qui renvoie à un des marqueurs définis précédemment.

L'écriture de `iterator_traits` repose sur deux parties. D'une part, les itérateurs des conteneurs de la bibliothèque standard sont supposés définir en interne leur propre alias `iterator_category` renvoyant à l'un des marqueurs :

``` cpp
template < ..... >
class deque
 {
  public :
    class iterator
     {
      public:
        typedef random_access_iterator_tag iterator_category;
      .....
     } ;
    .....
 } ;
.....
template < ..... >
class list
 {
  public :
    class iterator
     {
      public :
        typedef bidirectional_iterator_tag iterator_category;
        .....
     } ;
    .....
 } ;
```

La définition par défaut du patron `iterator_traits` se contente de propager cette valeur :

``` cpp
// the iterator_category for type IterT is whatever IterT says it is;
// see Item [[Regle42|42]] for info on the use of "typedef typename"
template<typename IterT>
struct iterator_traits
 {
  typedef typename IterT::iterator_category iterator_category ;
  .....
 } ;
```

Ce qu'il faut compléter par une specialisation partielle pour le cas des pointeurs ordinaires :

``` cpp
template<typename IterT>               // partial template specialization
struct iterator_traits< IterT * >      // for built-in pointer types
 {
  typedef random_access_iterator_tag iterator_category ;
  .....
 } ;
```

A ce stade, nous savons concevoir et écrire une classe de traits :

1.  Identifier l'information sur les types que nous voulons rendre accessible (pour les itérateurs, leur catégorie).
2.  Choisir un nom pour cette information (pour les itérateurs, `iterator_category`).
3.  Ecrire le patron et ses spécialisations pour les types à renseigner (pour les itérateurs, `iterator_traits`).

## Utilisation de traits

Etant donné `iterator_traits` - en fait, `std::iterator_traits`, inclus dans la biliothèque standard - on peut réécrire `advance()` ainsi :

``` cpp
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d)
 {
  if (typeid(typename std::iterator_traits<IterT>::iterator_category) ==
      typeid(std::random_access_iterator_tag))
   {
     iter += d;                                      // use iterator arithmetic
   }                                                  // for random access iters
  else
   {
  .....
 }
```

Cela semble prometteur, mais c'est n'est pas satisfaisant de conserver un `if`, qui consommera du temps à l'exécution et alourdira le code, alors que toutes les informations sont disponibles à la compilation, qui devraient permettre de savoir quelle branche du `if` sera toujours appliquée. Et de fait, ce code ne compile pas, car même en présence d'itérateur non direct, le compilateur cherche à compiler l'ensemble de la fonction, et échoue sur le `+=`.

**Ce que l'on cherche, c'est un moyen de choisir telle ou telle portion de code, selon les types incriminés, et au moment de la compilation. Ce moyen existe depuis longtemps en C++ : c'est la résolution de la surcharge**. En effet, la surcharge permet d'avoir plusieurs fonctions de même nom, et lorsqu'un appel à ce nom doit être compilé, le compilateur choisi, selon les arguments de l'appel, la véritable fonction à exécuter. Il suffit de glisser une instance de l'un des marqueurs d'itérateur déjà mentionnés, pour déclencher la sélection de la fonction la plus appropriée.

Pour appliquer ce principe à `advance()`, il faut commencer par déléguer l'essentiel de son contenu à une fonction paramétrée externe, dans laquelle on glisse un argument supplémentaire de type marqueur :

``` cpp
template<typename IterT, typename DistT>              // use this impl for
void doAdvance(IterT& iter, DistT d,                  // random access
               std::random_access_iterator_tag )      // iterators
 {
  iter += d ;
 }

template<typename IterT, typename DistT>              // use this impl for
void doAdvance(IterT& iter, DistT d,                  // bidirectional
               std::bidirectional_iterator_tag)       // iterators
 {
  if (d >= 0) { while (d--) ++iter; }
  else { while (d++) --iter;         }
 }

template<typename IterT, typename DistT>              // use this impl for
void doAdvance(IterT& iter, DistT d,                  // input iterators
               std::input_iterator_tag)
 {
  if (d < 0 ) {
     throw std::out_of_range("Negative distance");    // see below
  }
  while (d--) ++iter;
 }
```

Parce que le marqueur `forward_iterator_tag` hérite de `input_iterator_tag`, le `doAdvance()` correspondant sait traiter les deux cas.

Tout ce qu'il reste à faire dans `advance()`, c'est d'appeler `doAdvance()` en glissant l'objet temporaire approprié :

``` cpp
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d)
 {
  doAdvance
   (                                                      // call the version
    iter, d,                                              // of doAdvance
    typename                                              // that is
      std::iterator_traits<IterT>::iterator_category()    // appropriate for
  ) ;                                                      // iter's iterator
 }                                                         // category
```

On peut à présent résumer comment utiliser des traits :

  - créer un jeu de fonctions de travail, paramétrée ou non, qui diffèrent

`  par un argument de type traits, et optimiser chaque fonction selon ces traits.`

  - créer un fonction principale, paramétrée ou non, qui appele la fonction de

`  travail en ajoutant l'information de traits.                   `

Les traits sont très largement utilisés dans la bibliothèque standard. Il y a `iterator_traits`, bien sur, qui en plus de `iterator_category`, offre quatre autres information, dont la plus utile est `value_type`, le type de la donnée pointée. Il y aussi `char_traits`, qui apporte des informations sur les types de caractères, et `numeric_limits`, qui apporte des informations sur les types numériques, tels que leurs valeurs minimales et maximales.

## A retenir

  - Les traits fournissent des informations sur les types au moment de la compilation. Ils sont écrits à l'aide de patrons et de spécialisations de patrons.
  - Les marqueurs permettent de classer les types en catégorie, avec la possibilité d'utiliser l'héritage.
  - Combinés à la surcharge, les marqueurs permettent de réaliser à la compilation une forme de `if...else` basés sur les types impliqués.
