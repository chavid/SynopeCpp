# [Principes généraux de métaprogrammation](TheorieGenerique "wikilink")

## Motivation

La métaprogrammation à base de patrons (TMP, pour Template Meta Programming) consiste à écrire du code C++ qui s'éxécute à la compilation. Le langage n'a pas été initialement conçu pour, mais le mécanisme des patrons a ouvert des portes inattendues et efficaces, si bien que le langage évolue maintenant pour faciliter cette façon de programmer, qui a deux forces :

  - elle facilite l'implémentation de certaines idées, presque impossible sinon,
  - elle déplace une part du travail de l'exécution vers la compilation, ce qui permet de détecter certaines erreurs plus tôt et de produire un code plus rapide.

Les programmes exécutables produits sont plus rapides, parfois plus compacts, mais comme rien n'est gratuit, on le paie par un temps de compilation parfois très long, et des messages d'erreur... déroutants.

## Emuler un if dépendant du type

Prenons ce pseudo-code qui sert à avancer un itérateur, et qui voudrait supporter tous les cas, mais tirer parti de l'implémentation la plus rapide en présence d'un itérateur direct :

``` cpp
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d)
 {
  if (iter is a random access iterator)
   {
    iter += d ;                                     // use iterator arithmetic
   }                                                // for random access iters
  else
   {
    if (d >= 0) { while (d--) ++iter ; }            // use iterative calls to
    else { while (d++) --iter ; }                   // ++ or -- for other
   }                                                // iterator categories
 }
```

En imaginant que l'itérateur définit un type `iterator_category`, qui est équivalent à `std::random_access_iterator_tag` si c'est un itérateur à accès direct, on peut imaginer écrire la classe ainsi :

``` cpp
template<typename IterT, typename DistT>
void advance( IterT & iter, DistT d )
 {
  if (typeid(IterT::iterator_category) ==
      typeid(std::random_access_iterator_tag))
   { iter += d ; } 
  else
   {
    if (d >= 0) { while (d--) ++iter ; }
    else { while (d++) --iter ; }
   }
 }
```

Cette implémentation souffre de deux problèmes :

  - l'évaluation de typeid est faite à l'exécution, ainsi que la sélection de la

`  bonne branche de code, alors que le compilateur dispose de toutes les informations`  
`  nécessaire pour faire ce choix en amont. `

  - le code ne compile pas \! en effet, le compilateur esssaie de compiler l'ensemble

`  de la fonction quel que ce soit le type d'itérateur, et cherchera à compiler`  
`  "iter+=d" même si l'itérateur n'est pas à accès direct, et que la ligne ne`  
`  sera jamais exécutée.`

Pour résoudre ces deux problèmes, on délègue la réalisation l'action à une autre fonction surchargée, à qui on passe un argument fictif de type "iterator\_tag", ce qui permet de sélectionner l'implémentation à exécuter, de ne pas compiler de lignes inutiles (code plus compact), et de réaliser le choxi à la compilation (code plus rapide) :

``` cpp
template< typename IterT, typename DistT >
void doAdvance
 ( IterT& iter, DistT d, std::random_access_iterator_tag )
 { iter += d ; }

template< typename IterT, typename DistT >
void doAdvance
 ( IterT & iter, DistT d, std::bidirectional_iterator_tag )
 {
  if (d >= 0) { while (d--) ++iter ; }
  else { while (d++) --iter ; }
 }

template< typename IterT, typename DistT >
void advance( IterT & iter, DistT d )
 { doAdvance( iter, d, typename IterT::iterator_category() ) ; }
```

Cette implémentation est une forme de TMP, car elle réalise une forme de `if...else` sur les types au moment de la compilation.

Il a été démontré que la métaprogrammation à base de patrons est Turing-complète : capable de tout calculer, de déclarer des variables, de réaliser des boucles, appeler des fonctions, etc. Bien sur, c'est une façon d'écrire du code très différente d'un code C++ traditionnel. Mais il s'agit de cuisine de bas-niveau. Il est impressionnant de voir comment certaines bibliothèques offre une syntaxe de haut-niveau, qui ne peut certes pas être confondue avec du C++ classique.

## Emuler des boucles par récursivité

Pour un autre apercu de la métaprogrammation à base de patrons, considérons les boucles. Ces dernières n'ont pas de mécanisme dédié : elle sont réalisées à l'aide de techniques de récursion. Et encore ne s'agit-il pas de récursion ordinaire (une fonction qui s'appelle elle-même), mais d'instanciations récursives de patrons. Pour pratiquer cette métaprogrammtion, vous devez être à l'aide avec ces pratiques récursives : elles sont inévitables.

Le "hello world" de la récursivité est le calcul de factorielles :

``` cpp
template <unsigned n >
struct Factorial
 {
  enum { value = n * Factorial<n-1>::value } ;
 } ;

template <>
struct Factorial<0>
 {
  enum { value = 1 } ;
 } ;

int main()
 {
  std::cout << Factorial<5>::value << std::endl ;  // prints 120
  std::cout << Factorial<10>::value << std::endl ; // prints 3628800
  return 0 ;
 }
```

La récursion intervient quand `Factorial`<n> fait référence à `Factorial`<n-1>, et comme dans tout bon code récursif, il faut implémenter un cas particulier qui stoppe la récursion, ici la spécialisation `Factorial<0>`. Chaque instance du patron est une structure, et chaque structure utilise l'astuce `enum` pour déclarer une variable `value`. Parce que nous utilisons une instanciation récursive de patrons au lieu d'une boucle traditionnelle, chaque passage de la boucle a sa propre structure et sa propre `value`. C'est très rapide au moment de l'exécution, puisque tout le travail est prémâché par le compilateur, mais on n'imagine sans peine comment cela rallonge le temps de compilation et la taille de l'exécutable.

## Quelques autres possibilités

#### Garantir le respect des unités de mesure

En calcul scientifique, il est essentiel de ne pas mélanger des grandeurs de nature différente : c'est une erreur de copier une variable représentant une masse dans une variable représentant une vitesse, mais il est correct de diviser une variable de distance par une variable de temps et de stocker le résultat dans une variable de vitesse. La TMP est capable de vérifier à la compilation la validité des combinaisons de variables, quelle qu'en soit la complexité.

#### Optimiser les opérations matricielles

Pour implémenter des opérateurs mathématiques tels que la multiplication (`*`), tout en respectant les notations et les attendus des mathématiciens, il est recommandé que ces opérateurs construisent et retournent une nouvelle valeur, sans toucher aux opérandes. Si on multiplie d'énormes matrices, les conséquences sur le temps de calcul peuvent être dramatiques, en particulier à cause de toutes les valeurs intermédiaires qui sont produites avant d'arriver au résultat final :

``` cpp
typedef SquareMatrix<double, 10000> BigMatrix ;
BigMatrix m1, m2, m3, m4, m5 ;               // create matrices and
...                                          // give them values
BigMatrix result = m1 * m2 * m3 * m4 * m5 ;  // compute their product
```

Les techniques de programmation classique entraineront la création de quatre matrices temporaires (une pour chaque appel à `*`), et le parcours d'une séquence de quatre boucles à travers tous les éléments de matrice. Avec la technique dite des **expressions à base de patrons**, il est possible d'éliminer les matrices temporaires et de fusionner les boucles, le tout sans changer la syntaxe du code client ci-dessus. Le code résultant utilise moins de mémoire et s'exécute incroyablement plus vite.

#### Générer des patrons de conception personnalisés

Les patrons de conception tels que la "Stratégie", l'"Observateur", le "Visiteur"... peuvent être réalisées de multiples façons. En utilisant la technique dite de "conception à base de règles" (policy-based design), il est possible de créer des patrons représentant des "règles de conception" indépendantes, qui peuvent être recombinées de façon arbitraire et conduisant à des versions personnalisées des patrons de conception. Par exemple, des programmeurs ont montré comment la combinaison de quelques patrons implémentant des caractéristiques de pointeurs améliorés permet, à la compilation, de générer des centaines de pointeurs intelligents différents. En généralisant l'idée, on parle de **programmation générative**.

## Conclusion

La métaprogrammation à base de patrons n'est pas pour le commun des développeurs. C'est un paradigme surgi par accident dans le C++, dont la syntaxe n'est pas intuitive, et le support par les compilateurs variable (avant C++11), sans parler des débogeurs. Pourtant, l'efficacité qu'elle apporte en déplacant une partie du travail de l'exécution vers la compilation, et sa capacité à exprimer certains comportements et certaines notations (**DSEL**) en fait un outil très prometteur. Elle ne sera sans doute jamais le mode de programmation dominant de C++, mais pour les auteurs de bibliothèques, c'est une tuerie :)

## Exercice

#### Fichiers à récupérer

  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef13.cpp>

#### Enoncé

Le chronométrage des testeurs fait sens pour `EchecDepuisStable` et `EchecDepuisStable`, mais n'est pas indispensable pour `TesteurCoef` et `TesteurSomme`, et on aimerait s'en passer :

1.  Creez deux classes vides `TesteurTag` et `TesteurChronoTag`, et définissez un type imbriqué `categorie` dans `EchecDepuisStable` et `EchecDepuisStable`, qui sera un alias de `TesteurChronoTag`, et un type imbriqué `categorie` dans `TesteurCoef` et `TesteurSomme`, qui sera un alias de `TesteurTag`.
2.  Modifiez le code de `boucle()` afin qu'il fasse ou non le chronométrage, selon que le `Testeur` courant est de type `TesteurTag` ou `TesteurChronoTag`, en appelant une surcharge différente d'une fonction `boucle_impl()`.

#### Solution

  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef14.cpp>
