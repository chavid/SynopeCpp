# [TP : Introduction](README.md) \[\#license ©\]

Tout nombre réel peut être approximé par le rapport entre deux nombres entiers. Pour simplifier les éventuels calculs, on décide que l'entier au dénominateur sera une puissance de deux. En effet, la multiplication ou la division d'un entier par une puissance de deux se réalise simplement en décalant les bits de l'entier vers la gauche ou vers la droite.

On approximera donc tout réel r par une fraction `numerateur`/2^`exposant`^. Plus les valeurs de `numerateur` et `exposant` sont élevées, plus l'approximation peut-être précise. Par exemple, 0.65 peut être approximé successivement par 1/2^1^ (0.5), 3/2^2^ (0.75), 5/2^3^ (0.625), ... On choisira donc les nombres les plus élevés possibles, dans les limites fixés par le système, c'est à dire par le nombre de bits disponibles pour coder ces nombres.

Dans le cadre du TP, on va faire varier arbitrairement le nombre de bits autorisés pour stocker le numérateur, et calculer l'effet sur la précision de l'approximation. On notera que si on dispose de N bits pour stocker un entier, le plus grand entier possible est égal à 2^N^-1.

Pour amorcer le TP : comprenez, compilez et éxécutez le fichier [tp0\_code\_initial.cpp](coefs-old/tp0_code_initial.cpp.md), comprenant de petites fonctions utiles pour la suite. Si vous travaillez sous linux, vous pouvez utiliser le script [tp0\_code\_initial.sh](coefs-old/tp0_code_initial.sh.md) pour enchainer compilation et éxécution.

© *[David Chamont](http://llr.in2p3.fr/spip.php?page=view_person&personID=121), Laboratoire Leprince-Ringuet (LLR) / IN2P3 / CNRS, Ecole polytechnique / Université Paris-Saclay*\\\\ © *Ce(tte) œuvre est mise à disposition selon les termes de la [Licence Creative Commons Attribution - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/)*
