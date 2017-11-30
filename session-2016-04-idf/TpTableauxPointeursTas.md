# [TP : Tableaux, Pointeurs & Gestion du Tas](README.md) \[\#license ©\]

\_\_FORCETOC\_\_

En C++, on peut contrôler finement le cycle de vie des objets et gérer la mémoire qui leur est allouée. C'est ce qui permet de réaliser des applications plus performantes qu'avec d'autres langages, mais c'est aussi la source d'erreurs principale du langage. Pointeurs et gestion dynamique de la mémoire : attention danger \! \\\\ Il est conseillé de partir du fichier [tp4\_operateurs\_templates.cpp](coefs-old/tp4_operateurs_templates.cpp.md). Si vous êtes sous linux, vous pouvez utilisez le script [tp4\_operateurs\_templates.sh](coefs-old/tp4_operateurs_templates.sh.md) qui va compiler le programme et l'éxécuter.\\\\ Accès à la [section théorique correspondante](ComplementTableauxPointeursTas.md).

## 1\) Foncteur

Transformez la fonction `boucle` en méthode `execute(...)` d'une nouvelle classe `Boucle`.

## 2\) Stockage des testeurs

Ajoutez à la classe `Boucle` un tableau membre de 5 testeurs, ainsi qu'une fonction membre pour copier des testeurs dans ce tableau.

` `*`La`` ``déclaration`` ``d'un`` ``tableau`` ``de`` ``cinq`` ``testeurs`` ``s'écrit`` ``:`` ``Testeur testeurs_[5] ;`*`\\`  
` `*`La`` ``fonction`` ``membre`` ``de`` ``copie`` ``pourrait`` ``être`` ``de`` ``la`` ``forme`` ``:`` ``void copie( int position, Testeur testeur ) {...}`*`\\`

Supprimez de la fonction `Boucle::execute(...)` le premier argument de type Testeur, et modifiez cette fonction pour qu'elle s'applique successivement à tous les testeurs du tableau membre.

Corrigez la fonction principale `main()` pour qu'elle commence par copier les deux testeurs dans l'objet de type Boucle, avant d'appeler l'opérateur d'éxécution. Pour simplifier, vous pouvez supprimer le premier appel (avec les arguments 1000000, 4, 16 et 4) :

``` cpp
int main()
 {
  TesteurCoef<unsigned short> tc ;
  TesteurSomme<short> ts ;
  Boucle boucle ;
  boucle.copie(0,tc) ;
  boucle.copie(1,ts) ;
  boucle.execute(1000,1,8,1) ;
  std::cout << std::endl ;
  return 0 ;
 }
```

Comment expliquez-vous ce qui est affiché ?

## 3\) Tableau de pointeurs

Transformez le tableau de 5 testeurs en tableau de 5 pointeurs de testeurs. Corrigez toutes les fonctions affectées.

` `*`La`` ``déclaration`` ``d'un`` ``tableau`` ``de`` ``cinq`` ``pointeurs`` ``de`` ``testeurs`` ``s'écrit`` ``:`` ``Testeur * testeurs_[5] ;`*`\\`  
` `*`Ajoutez`` ``une`` ``méthode`` ``Boucle::init()`` ``pour`` ``mettre`` ``à`` ``0`` ``tous`` ``les`` ``pointeurs`` ``du`` ``tableau.`*`\\`  
` `*`L'ancienne`` ``méthode`` ``de`` ``copie`` ``de`` ``testeur`` ``doit`` ``être`` ``remplacée`` ``par`` ``une`` ``méthode`` ``de`` ``copie`` ``d'adresse`` ``de`` ``pointeur,`` ``qui`` ``pourrait`` ``prendre`` ``la`` ``forme`` ``:`` ``void`` ``enregistre(`` ``int`` ``position,`` ``Testeur`` ``*`` ``testeur`` ``)`` ``{...}`` ``}}}`*`\\`  
` `*`La`` ``méthode`` ``Boucle::execute()`` ``ne`` ``doit`` ``considérer`` ``que`` ``les`` ``adresses`` ``du`` ``tableau`` ``qui`` ``sont`` ``non`` ``nulles.`*`\\`

``` cpp
int main()
 {
  TesteurCoef<unsigned short> tc ;
  TesteurSomme<short> ts ;
  Boucle boucle ;
  boucle.init() ;
  boucle.enregistre(0,&tc) ;
  boucle.enregistre(1,&ts) ;
  boucle.execute(1000,1,8,1) ;
  std::cout << std::endl ;
  return 0 ;
 }
```

## 4\) Simplification

Supprimez l'argument `"position"` de la méthode `Boucle::enregistre()`. Enregistrer automatiquement la nouvelle adresse dans la première case nulle du tableau.

` `*`On`` ``peut`` ``améliorer`` ``simplement`` ``la`` ``méthode`` ``à`` ``l'aide`` ``d'une`` ``variable`` ``membre`` ``supplémentaire`` ``pour`` ``stocker`` ``l'indice`` ``de`` ``la`` ``dernière`` ``case`` ``utilisée,`` ``ou`` ``celle`` ``à`` ``venir.`*

``` cpp
int main()
 {
  TesteurCoef<unsigned short> tc ;
  TesteurSomme<short> ts ;
  Boucle boucle ;
  boucle.init() ;
  boucle.enregistre(&tc) ;
  boucle.enregistre(&ts) ;
  boucle.execute(1000,1,8,1) ;
  std::cout << std::endl ;
  return 0 ;
 }
```

## 5\) Utilisation du tas

Au lieu d'utiliser un tableau de taille fixe et de cinq éléments, créez dynamiquement dans `Boucle::init(...)` un tableau de taille variable, la taille étant passée en argument à `Boucle::init(...)`.

` `*`La`` ``déclaration`` ``d'un`` ``tableau`` ``dynamique,`` ``dont`` ``les`` ``éléments`` ``sont`` ``eux-même`` ``des`` ``pointeursde`` ``testeur,`` ``nécessite`` ``une`` ``double`` ``étoile`` ``:``Testeur * * testeurs ;`*`\\`  
` `*`La`` ``création`` ``sur`` ``le`` ``tas`` ``d'un`` ``tableau`` ``de`` ``N`` ``éléments,`` ``ces`` ``éléments`` ``étant`` ``eux-même`` ``des`` ``pointeurs`` ``de`` ``testeur,`` ``s'écrit`` ``:}{`{{new Testeur * [N]}}*`\\`

Lors de l'enregistrement d'un nouveau testeur, vérifiez que le tableau n'est pas déjà rempli. Si c'est le cas, appelez la fonction `echec(...)`.

Ajoutez aussi une fonction `Boucle::finalise(...)` qui rend au système la mémoire allouée au tableau, et qui doit être appelée en fin de `main(...)` :

``` cpp
int main()
 {
  TesteurCoef<unsigned short> tc ;
  TesteurSomme<short> ts ;
  Boucle boucle ;
  boucle.init(5) ;
  boucle.enregistre(&tc) ;
  boucle.enregistre(&ts) ;
  boucle.execute(1000,1,8,1) ;
  boucle.finalise() ;
  std::cout << std::endl ;
  return 0 ;
 }
```

## 6\) La juste taille

Dans votre `main()`, vous n'utilisez que deux testeurs, donc réduisez à 2 le nombre de testeurs que vous réservez lors de l'appel à `Boucle::init(...)` :

``` cpp
int main()
 {
  TesteurCoef<unsigned short> tc ;
  TesteurSomme<short> ts ;
  Boucle boucle ;
  boucle.init(2) ;
  boucle.enregistre(&tc) ;
  boucle.enregistre(&ts) ;
  boucle.execute(1000,1,8,1) ;
  boucle.finalise() ;
  std::cout << std::endl ;
  return 0 ;
 }
```

Ca s'éxécute toujours correctement ?

© *[David Chamont](http://llr.in2p3.fr/spip.php?page=view_person&personID=121), Laboratoire Leprince-Ringuet (LLR) / IN2P3 / CNRS, Ecole polytechnique / Université Paris-Saclay*\\\\ © *Ce(tte) œuvre est mise à disposition selon les termes de la [Licence Creative Commons Attribution - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/)*
