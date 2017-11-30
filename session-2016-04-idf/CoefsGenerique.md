# [TP Coefs : Programmation Générique](README.md "wikilink")

\_\_FORCETOC\_\_

Partez du fichier [tp4\_generique\_etape00.cpp](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp4_generique_etape00.cpp).\\\\ Accès à la [section théorique correspondante](TheorieGenerique "wikilink").

## 1\) Coef<int>

Le type entier utilisé pour l'attribut `numerateur_` de la classe `Coef` a été choisi arbitrairement. Faites-en un paramètre de la classe. On ne modifiera que le numérateur : le dénominateur et le nombre de bits restent des `int`.

` `*`Il`` ``faut`` ``ajouter`` ``"template`<typename U>`"`` ``avant`` ``la`` ``definition`` ``de`` ``classe,`` ``et`` ``remplacer`` ``par`` ``U`` ``le`` ``type`` ``de`` ``numerateur_``.`*  
` `*`Pensez`` ``également`` ``à`` ``changer`` ``le`` ``type`` ``retournée`` ``par`` ``Coef::numerateur()``.`*  
` `*`A`` ``chaque`` ``utilisation`` ``de`` ``la`` ``classe`` ``Coef``,`` ``il`` ``faut`` ``ajouter`` ``entre`` ``<>`` ``le`` ``nom`` ``de`` ``type`` ``à`` ``substituer`` ``à`` ``U``.`*

Corrigez les testeurs `TesteurCoef` ... `TesteurSomme` pour qu'ils utilisent des `Coef`<int>.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp4_generique_etape01.cpp)

## 2\) Ajustement de l'opérateur \*

On suppose que tous les entiers de notre simulation sont stockés avec une précision comparable. En particulier, les entiers de notre somme pondérée sont stockés de la même façon que les numérateurs de nos coefficients : dans la méthode `operator*` de la classe `Coef`, remplacez le type entier par le paramètre `U`.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp4_generique_etape02.cpp)

## 3\) \!TesteurSomme<short>

On veut pouvoir tester la classe `Coef` avec des types entiers différents. Transformez `TesteurSomme` en classe paramétrée. Utilisez ce paramètre pour définir le type de ses coefficients internes.

Ajouter au programme principal un testeur supplémentaire `TesteurSomme`<short>.

` `*`Vous`` ``aurez`` ``un`` ``problème`` ``d'ambiguité`` ``pour`` ``la`` ``compilation`` ``de`` ``TesteurSomme::teste()``.`` ``Assez`` ``logiquement,`` ``le`` ``compilateur`*  
` `*`ne`` ``sait`` ``pas`` ``quelle`` ``est`` ``la`` ``meilleure`` ``façon`` ``de`` ``multiplier`` ``des`` ``Coef`<short>` ``avec`` ``des`` ``int``.`` ``Modifiez`` ``la`` ``méthode`*  
` `*`TesteurSomme::teste()`` ``pour`` ``qu'elle`` ``prenne`` ``en`` ``entrée,`` ``comme`` ``type`` ``d'entiers,`` ``le`` ``même`` ``type`` ``qui`` ``est`` ``utilisé`` ``pour`*  
` `*`les`` ``coefficients`` ``testés.`*

Que remarquez-vous à l'éxécution ? Essayez avec `unsigned short`.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp4_generique_etape03.cpp)

## 4\) \!TesteurCoef<unsigned short>

De même, essayez de transformer `TesteurCoef` en classe paramétrée, et d'ajouter un `TesteurCoef065`<unsigned short> au programme principal.

Cela impliquera de faire de même pour ses classes dérivées,... et vous devrez aider le compilateur à accepter l'idée que la classe `TesteurCoef` a bien un eméthode `teste()` que l'on peut appeler dans les classes dérivées...

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp4_generique_etape04.cpp)

## 5\) \!TesteurCoef<unsigned char>

Pour tester des coefficients encore plus petits, on essaie également un nombre de bits allant de 1 à 8, avec le type `unsigned char`, qui tient sur un octet :

``` cpp
int main()
 {
  try
   {
    Testeurs ts(5) ;
    ts.acquiere(new TesteurCoef065<int>(1000000)) ;
    ts.acquiere(new TesteurCoef035<int>(1000000)) ;
    ts.acquiere(new TesteurSomme<int>(1000000)) ;
    ts.acquiere(new TesteurCoef065<unsigned short>(1000000)) ;
    ts.acquiere(new TesteurSomme<unsigned short>(1000000)) ;
    boucle(4,16,4,ts) ;
    std::cout<<std::endl ;
    Testeurs ts2(1) ;
    ts.acquiere(new TesteurCoef065<unsigned char>(1000)) ;
    boucle(1,8,1,ts2) ;
    std::cout<<std::endl ;
    return 0 ;
   }
  ...
 }
```

Que remarquez-vous à l'éxécution ? Comment améliorer l'affichage ?

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp4_generique_etape05.cpp)

## 6\) Testeurs<int>

Dotez la classe `Testeurs` d'un paramètre `SIZE` qui soit un entier (et non un type), et transformez l'attribut `testeurs__` en tableau statique `Testeur * testeurs__[SIZE]`.

``` cpp
int main()
 {
  try
   {
    Testeurs<5> ts ;
    ts.acquiere(new TesteurCoef065<int>(1000000)) ;
    ts.acquiere(new TesteurCoef035<int>(1000000)) ;
    ts.acquiere(new TesteurSomme<int>(1000000)) ;
    ts.acquiere(new TesteurCoef065<unsigned short>(1000000)) ;
    ts.acquiere(new TesteurSomme<unsigned short>(1000000)) ;
    boucle(4,16,4,ts) ;
    std::cout<<std::endl ;
    Testeurs<1> ts2 ;
    ts2.acquiere(new TesteurCoef065<unsigned char>(1000)) ;
    boucle(1,8,1,ts2) ;
    std::cout<<std::endl ;
    return 0 ;
   }
  catch ( Echec const & e )
   {
    std::cout<<"[ERREUR "<<e.code()<<" : "<<e.commentaire()<<"]"<<std::endl ;
    return e.code() ;
   }
 }
```

Ajoutez un `static_assert` pour vous assurer que l'entier passé en paramètre n'est pas un nombre négatif. Pourquoi est-ce mieux que de changer le type du paramètre pour `unsigned int` ?

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp4_generique_etape06.cpp)

## 7\) Vérification du nombre de bits utilisé pour un Coef

Lorsque l'on créé une instance de `Coef`<T> avec un certain nombre de bits, on voudrait vérifier que ce nombre est compatible avec le type `T`. On sait que le nombre d'octet d'un type `T` s'obtient par `sizeof(T)`, et qu'on octet contient 8 bits.

  - créez une fonction `nombre_bits`, à la fois `constexpr` et

`  paramétrée par le type T, qui renvoie le nombre de bits de ce`  
`  type.`

  - dans la classe `Coef`<T>, ajouter une variable membre statique

`  et constexpr, qui contient le nombre maximum de bits autorisé.`  
`  et modifiez le constructeur pour vérifier que le nombre de bits`  
`  demandé est correct.`

``` cpp
...
template<typename U>
class Coef
 {
  ...
  private :
    ...
    static constexpr unsigned int max_bits__ = ??? ;
 } ;
...
```

Essayez de modifier le programme principal pour déclencher la vérification.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp4_generique_etape07.cpp)

## 8\) Question bonus

Dans le programme principal `main()`, que se passe-t-il si un remet le type `short` à la place de `unsigned short` ?

Pour résoudre ce problème, il faudrait détecter la nature `signed` ou `unsigned` d'un type, et spécialiser l'implémentation de `nombre_bits()` pour retrancher 1 en présence d'un type signé.

  - Créez le template suivant, et spécialisez le pour les types entiers non

`  signés qui sont utilisés dans le programme (en retournant false) :`

``` cpp
...
template<typename T>
constexpr bool avec_signe()
 { return true ; }
...
```

  - Transformez `nombre_bits()` en `nombre_bits_hors_signe()`, qui utilise

`  avec_signe() pour retrancher 1 en présence de type signé.`

  - Corrigez le constructeur de `Coef` pour utiliser `nombre_bits_hors_signe()`.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp4_generique_etape08.cpp)

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
