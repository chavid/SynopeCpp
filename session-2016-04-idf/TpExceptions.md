# [TP : Exceptions](README.md) \[\#license ©\]

\_\_FORCETOC\_\_

Il est conseillé de partir du fichier [tp6\_static\_const.cpp](coefs-old/tp6_static_const.cpp.md). Si vous êtes sous linux, vous pouvez utilisez le script [tp6\_static\_const.sh](coefs-old/tp6_static_const.sh.md) qui va compiler le programme, l'éxécuter et vérifier que l'affichage ne change pas.\\\\ Accès à la [section théorique correspondante](ComplementExceptions.md).

## 1\) Création d'une classe pour les exceptions

Remplacez la fonction `echec(...)` par une classe `Echec`, qui se construit à partir d'un entier et d'une `std::string`, et qui fournit des fonctions pour lire ces valeurs. Partout où l'on appellait la fonction `echec(...)`, lancez une exception à la place. Dans la fonction `main(...)`, placez les instructions dans un bloc `try` et ajoutez une clause `catch` qui attrape les exceptions de type `Echec` et affiche leur contenu, comme le faisait l'ancienne fonction `echec(...)`.

` `*`Pour`` ``verifier`` ``qu'une`` ``exception`` ``est`` ``bien`` ``levée`` ``puis`` ``attrapée,`` ``remplacez`` ``l'appel`` ``à`` ``Testeur::init(5)`` ``en`` ``début`` ``de`` ``main(...)`` ``par`` ``un`` ``appel`` ``à`` ``Testeur::init(1)``.`` ``Une`` ``fois`` ``que`` ``le`` ``bon`` ``fonctionnement`` ``est`` ``démontré,`` ``remettre`` ``l'appel`` ``original`` ``à`` ``Testeur::init(5)``.`*

## 2\) Poursuivre l'éxécution malgré une exception

Ajoutez des blocs `try` et `catch` dans la fonction `boucle(...)`, de telle sorte que lorsqu'une exception est émise dans l'un des testeurs, l'éxécution puisse reprendre normalement pour les autres testeurs.

` `*`Pour`` ``verifier`` ``que`` ``l'objectif`` ``est`` ``atteint`` ``:`` ``dans`` ``la`` ``fonction`` ``TesteurCoef::execute(...)``,`` ``remplacez`` ``l'appel`` ``à`` ``teste(bits,0.65)`` ``par`` ``un`` ``appel`` ``à`` ``teste(bits,0)``.`` ``Une`` ``fois`` ``que`` ``le`` ``bon`` ``fonctionnement`` ``est`` ``démontré,`` ``remettre`` ``l'appel`` ``original`` ``à`` ``teste(bits,0.65)``.`*

© *[David Chamont](http://llr.in2p3.fr/spip.php?page=view_person&personID=121), Laboratoire Leprince-Ringuet (LLR) / IN2P3 / CNRS, Ecole polytechnique / Université Paris-Saclay*\\\\ © *Ce(tte) œuvre est mise à disposition selon les termes de la [Licence Creative Commons Attribution - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/)*
