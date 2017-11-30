# [TP : Entrées/Sorties Standards](README.md "wikilink") \[\#license ©\]

\_\_FORCETOC\_\_

Le C++ standard comprend une bibliothèque d'entrées/sorties qui spécifie une interface commune pour tous les échanges de données avec l'extérieur, reposant notamment sur les opérateurs d'insertion \<\< et d'extraction \>\>.\\\\ Il est conseillé de partir du fichier [tp7\_exceptions.cpp](coefs-old/tp7_exceptions.cpp "wikilink"). Si vous êtes sous linux, vous pouvez utilisez le script [tp7\_exceptions.sh](coefs-old/tp7_exceptions.sh "wikilink") qui va compiler le programme, l'éxécuter et vérifier que l'affichage ne change pas.\\\\ Accès à la [section théorique correspondante](ComplementEntreesSortiesStandards "wikilink").

## 1\)

Transformez la fonction `"affiche()"` en fonction `"operator<<(std::ostream &,...)"`.

## 2\)

Modifiez la fonction `"operator<<()"` précédente pour qu'elle puisse traiter toutes les sortes de `Coef`.

© *[David Chamont](http://llr.in2p3.fr/spip.php?page=view_person&personID=121), Laboratoire Leprince-Ringuet (LLR) / IN2P3 / CNRS, Ecole polytechnique / Université Paris-Saclay*\\\\ © *Ce(tte) œuvre est mise à disposition selon les termes de la [Licence Creative Commons Attribution - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/)*
