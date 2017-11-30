# [TP : Statiques, Construction & Constantes](README.md) \[\#license ©\]

\_\_FORCETOC\_\_

Il est conseillé de partir du fichier [tp5\_pointeurs.cpp](coefs-old/tp5_pointeurs.cpp.md). Si vous êtes sous linux, vous pouvez utilisez le script [tp5\_pointeurs.sh](coefs-old/tp5_pointeurs.sh.md) qui va compiler le programme, l'éxécuter et vérifier que l'affichage ne change pas.\\\\ Accès à la [section théorique correspondante](ComplementStatiquesConstructionConstance.md).

## 1\) Variables membres statiques

Rendez statiques les variables membres de la classe `Boucle`.

## 2\) Fonctions membres statiques

Rendez statiques toutes les fonctions membres de la classe `Boucle`. Dans la fonction `main(...)`, supprimez l'objet de type `Boucle` et appelez directement les fonctions statiques à l'aide de l'opérateur de portée de classe `Boucle::`.

## 3\) Constructeurs de testeurs

On estime que le choix de la résolution du calcul d'erreur est propre à chaque testeur, et ne devrait pas être décidé au niveau de la boucle. Ajoutez à `TesteurCoef` et `TesteurSomme` un constructeur qui prend en argument la résolution attribuée à ce testeur, puis retirez cet argument à `Boucle::execute(...)` et corrigez en conséquence le reste du programme. La fonction `main(...)` doit devenir :

``` cpp
int main()
 {
  TesteurCoef<unsigned short> tc(100) ;
  TesteurSomme<short> ts(1000) ;
  Boucle::init(5) ;
  Boucle::enregistre(&tc) ;
  Boucle::enregistre(&ts) ;
  Boucle::execute(1,8,1) ;
  Boucle::finalise() ;
  std::cout << std::endl ;
  return 0 ;
 }
```

## 4\) Constructeur de base

Transformez `Testeur::init(...)` en constructeur, et corrigez le reste du programme en conséquence.

## 5\) Déplacement des membres statiques

Enlevez les attributs et méthodes statiques (sauf `execute()`) de la classe Boucle et placez les dans la classe Testeur.

` `*`La`` ``fonction`` ``Boucle::execute(...)`` ``n'a`` ``plus`` ``d'accès`` ``direct`` ``à`` ``la`` ``liste`` ``des`` ``testeurs.`` ``Il`` ``faut`` ``ajouter`` ``à`` ``la`` ``classe`` ``Testeur`` ``deux`` ``méthodes`` ``:`` ``l'une`` ``pour`` ``lire`` ``le`` ``nombre`` ``de`` ``testeurs`` ``enregistrés,`` ``l'autre`` ``pour`` ``récupérer`` ``l'adresse`` ``du`` ``énième`` ``testeur.`` ``Ceci`` ``afin`` ``de`` ``pouvoir`` ``écrire`` ``:`*

``` cpp
class Boucle
 {
  public :
    static void execute( int deb, int fin, int inc )
     {
      for ( unsigned int i=0 ; i<Testeur::nb_testeurs() ; ++i )
       {
        std::cout<<std::endl ;
        for ( int bits = deb ; bits <= fin ; bits = bits + inc )
         { Testeur::testeur(i)->execute(bits) ; }
       }
     }
 } ;
```

## 6\) Rewind

Retransformez la classe `Boucle` en simple fonction `boucle(...)`.

## 7\) Coefs de taille fixe

Transformez la méthode `Coef::init()` en constructeur.

` `*`Le`` ``nombre`` ``de`` ``bits`` ``alloués`` ``à`` ``une`` ``instance`` ``de`` ``Coef`` ``est`` ``fixé`` ``à`` ``la`` ``construction`` ``et`` ``ne`` ``peut`` ``plus`` ``varier.`` ``De`` ``ce`` ``fait,`` ``un`` ``tel`` ``objet`` ``ne`` ``peut`` ``plus`` ``être`` ``réutilisé`` ``en`` ``changeant`` ``à`` ``chaque`` ``fois`` ``le`` ``nombre`` ``de`` ``bits,`` ``comme`` ``le`` ``font`` ``actuellement`` ``les`` ``testeurs.`` ``Il`` ``faut`` ``supprimer`` ``les`` ``variables`` ``membres`` ``de`` ``type`` ``Coef`` ``des`` ``testeurs,`` ``et`` ``les`` ``remplacer`` ``par`` ``des`` ``objets`` ``temporaires,`` ``recréés`` ``à`` ``chaque`` ``fois`` ``qu'on`` ``éxécute`` ``les`` ``tests`` ``pour`` ``un`` ``nouveau`` ``nombre`` ``de`` ``bits.`*

## 8\) Auto-enregistrement des testeurs

Faites en sorte que tout nouveau testeur créé soit automatiquement enregistré dans le tableau statique des testeurs.

` `*`Rappel`` ``:`` ``la`` ``variable`` ``"this"`` ``contient`` ``toujours`` ``l'adresse`` ``de`` ``l'objet`` ``courant.`*

``` cpp
int main()
 {
  Testeur::init(5) ;
  TesteurCoef<unsigned short> tc(100) ;
  TesteurSomme<short> ts(1000) ;
  boucle(1,8,1) ;
  Testeur::finalise() ;
  std::cout << std::endl ;           
  return 0 ;
 }
```

## 9\) Variable membre constante

Tranformez `Coef::bits_` et `Testeur::resolution_` en attributs constants.

## 10\) Argument et fonctions membres constants

Ajoutez la fonction ci-dessous, et modifiez le reste du programme pour qu'il compile correctement.

``` cpp
void affiche( Coef<short> const & c )
 { std::cout << c.numerateur() << "/2^" << c.exposant() ; }
```

## \*) Question bonus

Transformez la partie statique de `Testeur` pour stocker des pointeurs vers des testeurs constants :

``` cpp
class Testeur
 {
  ...
    static Testeur const * * testeurs__ ;
  ...
 } ;
```

Corrigez le reste du code en conséquence :

  - `Testeur::init(...)` doit créer un tableau de pointeurs de testeurs constants.
  - `Testeur::testeur(...)` doit retourner un pointeur de testeur constant.
  - `boucle(...)` doit manipuler des testeurs constants.
  - `TesteurCoef::teste(...)` et `TesteurSomme::teste(...)` doivent être constantes.
  - `Testeur::erreur(...)` doit être constante.

© *[David Chamont](http://llr.in2p3.fr/spip.php?page=view_person&personID=121), Laboratoire Leprince-Ringuet (LLR) / IN2P3 / CNRS, Ecole polytechnique / Université Paris-Saclay*\\\\ © *Ce(tte) œuvre est mise à disposition selon les termes de la [Licence Creative Commons Attribution - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/)*
