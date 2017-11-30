# [TP Coefs : Programmation fonctionnelle et concurrente](README.md "wikilink")

\_\_FORCETOC\_\_

Nous allons à nouveau utiliser notre code testant beaucoup de coefficients aléatoires, en augmentant sensiblement le nombre d'itérations pour allonger les temps de calcul. Nous essaierons ensuite d'exécuter nos testeurs en parallèle.

Partez du fichier [tp6\_parallele\_etape00.cpp](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp6_parallele_etape00.cpp).\\\\ Accès à la [section théorique correspondante](TheorieFonctionnelleConcurrente "wikilink").

## 1\) Inférence de type

Essayez d'utilisez le nouveau mot-clef `auto` partout ou cela vous semble améliorer la lisibilité du code, sans pour autant nuire à sa compréhension.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp6_parallele_etape01.cpp)

## 2\) Lambdas

Pour s'écarter du comportement séquentiel attaché aux boucles for classiques, remplacez les boucles `for` de la classe `RandTesteur` et de la fonction `boucle()` par un appel à `std::for_each`, combiné à une fonction lambda contenant le code à appliquer à chaque élément de la collection.

(à noter : certaines bibliothèques de classes expériementent déjà une variante `std::for_each_parallel`, appellée à devenir standard.)

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp6_parallele_etape02.cpp)

## 3\) For généralisé

On peut considérer le nouveau `for` généralisé de C++11 comme un raccourci du code ci-dessus : utilisez ce nouveau `for` pour les boucles de la classe `RandTesteur` (ne pas le faire pour la fonction `boucle()`).

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp6_parallele_etape03.cpp)

## 4\) Parallélisation des tests

Modifiez la fonction `boucle()` pour qu'elle lance chaque testeur dans un \`std::thread\` dédié. Nous vous recommandons de remplacer le `for_each` par un `for` gééralisé, et de stocker vos threads dans un vecteur au fur et à mesure de leur création ; puis, dans un autre `for`, faites un appel à `join()` sur chacune des threads. Ainsi, elles sont toutes terminées lorsqu'on sort de la fonction `boucle()`.

Bien entendu, les affichages des threads s'entremêlent... Dans la solution ci-dessous, nous stockons les résultats dans un tableau intermédiaire pour les afficher postérieurement, dans un ordre correct.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp6_parallele_etape04.cpp)

## 5\) Stress test

Pour mettre volontairement le système à rude épreuve, vous trouverez ci-dessous une variante qui stockent tous les résultats intermédiaires dans une énorme `std::multimap`. Bien entendu, l'ensemble est beaucoup plus lent, mais c'est nécessaire pour ce qui va suivre. Le nombre de coefficients aléatoires a été baissé d'un facteur 10.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp6_parallele_etape05.cpp)

## 6\) Mémoire partagée

Modifiez à nouveau la fonction `boucle()` pour qu'elle lance un \`std::thread\` séparé pour chaque appel à `testeur->execute(bits)`. Lancez le programme plusieurs fois... Quel est la nature du problème ? Comment le résoudre ?

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp6_parallele_etape06.cpp)

## 7\) Appels asynchrone

On voudrait à présent se passer de la gestion des données partagées et des mutex, en essayant d'utiliser la fonction `std::async()`. Cela suppose que la fonction `boucle()` récupère les futurs résultats des appels asynchrones à `RandTesteur::execute()`, les stockent quelque part, puis les redonne aux testeurs pour assurer l'affichage. Il faut donc débarasser `RandTesteur` du stockage interne des résultats, modifier `RandTesteur::execute()` pour qu'il renvoie un `Resultat`, et fournir une méthode `RandTesteur::execute( Resultat const &)` public pour permettre à `boucle()` de sous-traiter l'afficahe final des résultats :

``` cpp
...

template<int SIZE>
class RandTesteur
 {
  public :

    ...

    struct Resultat
     {
      Resultat() : exacts {0}, approxs {0}, erreurs {0} {}
      double exacts, approxs, erreurs ;
     } ;
    
    Resultat execute( int bits )
     {
      double exact, approx ;
      Resultat res ;
      for ( double coef : coefs_ )
       {
        execute(bits,coef,exact,approx) ;
        res.exacts +=exact ; res.approxs += approx ;
        res.erreurs += fabs(exact-approx)/exact ;
       }
      res.exacts /= SIZE ; res.approxs /= SIZE ; res.erreurs /= SIZE ;
      res.erreurs *= resolution_ ;
      return res ;
     }
     
    void affiche( int bits, Resultat const & res )
     {
      std::cout
        <<bits<<" bits : "
        <<std::left<<res.exacts<<" ~ "
        <<std::setw(width_)<<res.approxs
        <<" ("<<arrondi(res.erreurs)<<"/"<<resolution_<<")"
        <<std::endl ;
     }
    
    ... 
    
 } ;

...
```

Il n'y a plus d'accès simultané à une `multimap` partagée qui plombe les performances. Vous pouvez restaurer le nombre initial de coefficients aléatoires dans le programme principal, avant que nosu passions en mode "stress".

Du côté de la fonction `boucle()`, nous vous proposons de stocker les futurs résultats dans une `std::map` indexée par le nombre de bits, et d'imbriquer cette `std::map` dans une autre `std::map` indexée par l'adresse du testeur concerné. Complétez les `???` dans le code ci-dessous :

``` cpp
...

#include <map>
#include <future>

template<typename Testeurs>
void boucle( int deb, int fin, int inc, Testeurs & ts )
 {
  using Testeur = typename Testeurs::value_type ;
  using Resultat = typename Testeur::element_type::Resultat ;
  using TesteurResultats = std::map<int,???> ;
  std::map<Testeur*,TesteurResultats> resultats ;
  
  for ( auto & testeur : ts )
   {
    resultats[&testeur] = TesteurResultats() ;
    for ( int bits = deb ; bits <= fin ; bits = bits + inc )
     {
      resultats[&testeur][bits] = ???(???,
          [bits,&testeur](){ return testeur->execute(bits) ; }) ;
     }
   }
   
  for ( auto & testeur : ts )
   {
    std::cout<<std::endl ;
    for ( int bits = deb ; bits <= fin ; bits = bits + inc )
      { testeur->affiche(bits,resultats[&testeur][bits]???) ; }
   }
 }

...
```

Pourquoi toute cette gymnastique avec les `using`, au lieu d'utiliser directement `RandTesteur::Resultat` ?

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp6_parallele_etape07.cpp)

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont et Hadrien Grasland, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
