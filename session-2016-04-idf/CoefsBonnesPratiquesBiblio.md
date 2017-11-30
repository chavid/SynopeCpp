# [TP Coefs : Bonnes pratiques & bibliothèque standard](README.md "wikilink")

\_\_FORCETOC\_\_

Dans cette séquence du TP, on a volontairement enlever le traitement des erreurs et réduit le nombre de testeurs pour alléger le code. Par ailleurs, pour mieux évaluer les approximations du calcul, on réalise les tests sur 1000 coefficients tirés aléatoirement.

Partez du fichier [tp5\_biblio\_etape00.cpp](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp5_biblio_etape00.cpp).\\\\ Accès à la [section théorique correspondante](TheorieBonnesPratiquesBiblio "wikilink").

## 1\) Utilisation de `std::vector`

Changez la définition de la variable membre `Testeurs::testeurs__` pour qu'elle soit de type `std::vector<RandTesteur *>`. Corrigez le reste du code de façon appropriée.

Lors de l'accès aux éléments de `Testeurs`, on laissera le soin à `std::vector` de vérifier lui-même si l'indice est correct, en utilisant sa méthode `at()` au lieu de l'opérateur `[]`.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp5_biblio_etape01.cpp)

## 2\) Tentative de création d'un pointeur amélioré

La classe `Testeurs` est maintenant quasi-vide, si ce n'est la destruction finale des testeurs qu'elle a acquis. Essayons d'aller plus loin, en confiant cette destruction à un pointeur amélioré.

Créez une classe paramétrée `Pointeur`<Value>, dont le destructeur s'assure de détruire également l'objet pointé. Veuillez à définir :

  - l'opérateur `*`, qui doit renvoyer une référence à la valeur pointée `Value &`,
  - l'opérateur `->`, qui doit renvoyer un pointeur brut vers la valeur pointée `Value *`,

Essayez d'utiliser ce nouveau pointeur pour simplifier encore la classe `Testeurs` :

``` cpp
class Testeurs
 {
  public :
    void acquiere( RandTesteur * t ) { testeurs_.push_back(t) ; }
    int nb_elements() { return testeurs_.size() ; }
    Pointeur<RandTesteur> operator[]( int i ) { return testeurs_.at(i) ; }
  private :
    std::vector<Pointeur<RandTesteur>> testeurs_ ;
 } ;
```

Pourquoi l'exécution du programme échoue-t-elle ? (plusieurs raisons possibles)

Vous pouvez essayez les classes bavardes données dans la solution ci-dessous pour mieux suivre ce qui se passe. Pour éclaircir le cheminement, nous avons volontairement commenter l'appel réel à delete.

["solution"](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp5_biblio_etape02.cpp)

## 3\) Copie par vidage

a) Et si on essaie de modifier la classe Pointeur pour que son constructeur par copie et son operateur d'affectation "vide" le pointeur d'origine en le mettant à nullptr, à la façon d'un `std::auto_ptr` ?

b) Et si on ajoute un constructeur par copie et une affectation depuis une référence temporaire (&&) ? (en corrigeant l'erreur commise volontairement dans l'operateur crochets de `Testeurs`...)

["solution"](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp5_biblio_etape03.cpp)

## 4\) Utilisation de `std::shared_ptr` et `std::make_shared`

On a vu que l'écriture d'un pointeur amélioré est loin d'être triviale. Essayons ceux de la bibliothèque standard :

  - remplacez le pointeur "maison" par `std::shared_ptr`,
  - utilisez `std::make_shared` à la place de `new` dans le programme principal.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp5_biblio_etape04.cpp)

## 5\) Utilisation de `std::unique_ptr` et `std::make_unique`

La solution précécente fonctionne, mais il est dommage d'utiliser un pointeur avec comptage des références, relativement lourd, alors que dans notre cas les pointeurs ne sont stockés que dans le tableau des testeurs, et peuvent être communiqués aux tiers par des références, sans jamais faire de copie. Essayons de faire mieux :

  - remplacez `std::shared_ptr` par `std::unique_ptr`,
  - remplacez `std::make_shared` par `std::make_unique`.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp5_biblio_etape05.cpp)

## 6\) Plus de besoin de `Testeurs`

En fait, la classe `Testeurs` n'apporte maintenant plus aucune valeur ajoutée... Utilisez directement `std::vector<std::unique_ptr`<RandTesteur>`>` à la place.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp5_biblio_etape06.cpp)

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
