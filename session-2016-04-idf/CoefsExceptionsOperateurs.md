# [TP Coefs : Exceptions & Opérateurs](README.md)

\_\_FORCETOC\_\_

Partez du fichier [tp3\_exceptions\_etape00.cpp](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp3_exceptions_etape00.cpp).\\\\ Accès à la [section théorique correspondante](TheorieExceptionsOperateurs.md).

## 1\) Création d'une classe pour les exceptions

Remplacez la fonction `echec(...)` par une classe `Echec`, qui se construit à partir d'un entier et d'une `std::string`, et qui fournit des fonctions pour lire ces valeurs. Partout où l'on appellait la fonction `echec(...)`, lancez une exception à la place. Dans la fonction `main(...)`, placez les instructions dans un bloc `try` et ajoutez une clause `catch` qui attrape les exceptions de type `Echec` et affiche leur contenu, comme le faisait l'ancienne fonction `echec(...)`.

` `*`Pour`` ``verifier`` ``qu'une`` ``exception`` ``est`` ``bien`` ``levée`` ``puis`` ``attrapée,`` ``remplacez`` ``l'appel`` ``à`` ``Testeurs(3)`` ``en`` ``début`` ``de`` ``main(...)`` ``par`` ``un`` ``appel`` ``à`` ``Testeurs(1)``.`` ``Une`` ``fois`` ``que`` ``le`` ``bon`` ``fonctionnement`` ``est`` ``démontré,`` ``remettre`` ``l'appel`` ``original`` ``à`` ``Testeurs(3)``.`*

La solution ci-dessous contient volontairement une erreur destinée à déclencher le lancement d'une exception.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp3_exceptions_etape01.cpp)

## 2\) Poursuivre l'éxécution malgré une exception

Ajoutez des blocs `try` et `catch` dans la fonction `boucle(...)`, de telle sorte que lorsqu'une exception est émise dans l'un des testeurs, l'éxécution puisse reprendre normalement pour les autres testeurs.

` `*`Pour`` ``verifier`` ``que`` ``l'objectif`` ``est`` ``atteint`` ``:`` ``dans`` ``la`` ``fonction`` ``TesteurCoef065::execute(...)``,`` ``remplacez`` ``l'appel`` ``à`` ``teste(bits,0.65)`` ``par`` ``un`` ``appel`` ``à`` ``teste(bits,0)``.`` ``Une`` ``fois`` ``que`` ``le`` ``bon`` ``fonctionnement`` ``est`` ``démontré,`` ``remettre`` ``l'appel`` ``original`` ``à`` ``teste(bits,0.65)``.`*

La solution ci-dessous contient volontairement une erreur destinée à déclencher le lancement d'une exception.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp3_exceptions_etape02.cpp)

## 3\) operator\*

Transformez la méthode `multiplie(...)` de la classe `Coef` en opérateur de multiplication.

` `*`L'opérateur`` ``de`` ``multiplication`` ``s'appelle`` ``"operator*"``.`*

Corrigez les méthodes `teste(...)`.

` `*`Quand`` ``une`` ``classe`` ``est`` ``dotée`` ``de`` ``cet`` ``opérateur,`` ``on`` ``peut`` ``écrire`` ``"obj1 * obj2"`` ``comme`` ``on`` ``le`` ``fait`` ``avec`` ``les`` ``types`` ``prédéfinis.`*

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp3_exceptions_etape03.cpp)

## 4\) operator\[\]

Transformez la méthode `Testeurs::element(...)` en opérateur d'accès direct.

` `*`L'opérateur`` ``d'accès`` ``direct`` ``s'appelle`` ``"operator[]"``.`*

Corrigez la fonction `boucle(...)`.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp3_exceptions_etape04.cpp)

\== 5) operator=

Transformez la méthode `approxime` de la classe `Coef` en opérateur d'affectation.

` `*`L'opérateur`` ``d'affectation`` ``s'appelle`` ``"operator="``.`*

Corrigez les méthodes `teste`.

` `*`Quand`` ``une`` ``classe`` ``est`` ``dotée`` ``de`` ``cet`` ``opérateur,`` ``on`` ``peut`` ``écrire`` ``"obj1 = ..."`` ``comme`` ``on`` ``le`` ``fait`` ``avec`` ``les`` ``types`` ``prédéfinis.`*

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp3_exceptions_etape05.cpp)

## 6\) Opérateur \<\<

Transformez la fonction `"affiche()"` en fonction `"operator<<(std::ostream &,...)"`.

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp3_exceptions_etape06.cpp)

## 7\) Conversion en double

Transformez la méthode `approximation` de la classe `Coef` en opérateur de conversion en `double`.

` `*`L'opérateur`` ``de`` ``conversion`` ``en`` ``double`` ``s'appelle`` ``"operator double"``.`` ``Cet`` ``opérateur`` ``n'a`` ``pas`` ``de`` ``type`` ``de`` ``retour`` ``(on`` ``ne`` ``met`` ``rien).`*

Corrigez les méthodes `teste`.

` `*`Quand`` ``une`` ``classe`` ``est`` ``dotée`` ``de`` ``cet`` ``opérateur,`` ``On`` ``peut`` ``passer`` ``un`` ``objet`` ``de`` ``cette`` ``classe`` ``en`` ``argument`` ``à`` ``une`` ``fonction`` ``qui`` ``s'attend`` ``à`` ``recevoir`` ``un`` ``double``.`*

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp3_exceptions_etape07.cpp)

## 8\) Objets-fonctions

Transformez la méthode `execute(...)` de la classe `Testeur` et de ses dérivées en opérateur d'éxécution.

` `*`L'opérateur`` ``d'éxécution`` ``s'appelle`` ``"operator()"``.`` ``Lors`` ``de`` ``sa`` ``définition,`` ``on`` ``a`` ``donc`` ``deux`` ``paires`` ``de`` ``parenthèses`` ``successives.`*

Corrigez la fonction `boucle(...)`.

` `*`Un`` ``objet`` ``dont`` ``la`` ``classe`` ``possède`` ``un`` ``opérateur`` ``d'éxécution`` ``peut`` ``être`` ``utilisé`` ``comme`` ``si`` ``il`` ``était`` ``une`` ``fonction`` ``:`` ``"obj(...) ;"``.`*

[solution](https://github.com/ReseauDevlog/SynopeCpp/raw/master/session-2016-04-idf/coefs/tp3_exceptions_etape08.cpp)

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
