# [TP : Variables & Fonctions](README.md) \[\#license ©\]

\_\_FORCETOC\_\_

Dans cette première partie, on survole la syntaxe de la partie "procédurale" du langage, essentiellement issue du C. Il est conseillé de partir du fichier [tp0\_code\_initial.cpp](coefs-old/tp0_code_initial.cpp.md), que vous pouvez compiler et éxécuter avec le script [tp0\_code\_initial.sh](coefs-old/tp0_code_initial.sh.md) si vous êtes sous linux.\\\\ Accès à la [section théorique correspondante](FondamentauxVariablesFonctions.md).

## 1\) Ajout d'une boucle

On veut approximer la valeur **0.65** par le rapport d'un entier et d'une puissance de deux. Ecrivez un programme qui calcule la valeur du numérateur pour un dénominateur valant **2**, **2^2^**, **2^3^**... **2^8^**. Afficher la valeur que l'on approxime, la valeur du numérateur, et la puissance de deux au dénuménateur :

    0.65 ~ 1/2^1
    0.65 ~ 3/2^2
    0.65 ~ 5/2^3
    0.65 ~ 10/2^4
    0.65 ~ 21/2^5
    0.65 ~ 42/2^6
    0.65 ~ 83/2^7
    0.65 ~ 166/2^8

## 2\) Ajout d'une fonction

Placez le programme précédent dans une fonction appelée `approxime(...)`, qui prend en argument la valeur réelle à approximer. Modifiez le programme principal pour qu'il appelle cette fonction avec les valeurs **0.65** et **0.35**. Le programme principal doit être :

``` cpp
int main()
 {
  std::cout << std::endl ;
  approxime(0.65) ;
  std::cout << std::endl ;
  approxime(0.35) ;
  std::cout << std::endl ;
  return 0 ;
 }
```

## 3\) Calcul de l'approximation

Ajoutez dans la fonction `approxime(...)` l'affichage de la valeur approximée, qui se calcule en divisant chaque numérateur par la puissance de deux associée. Attention, la division d'un entier par un entier retourne un entier : **3/4** vaut **0**. Une division retourne un réel si l'un des termes est réel : **3./4** ou **double(3)/4** vaut **0.75**. L'affichage attendu est le suivant :

    0.65 ~   1/2^1 = 0.5
    0.65 ~   3/2^2 = 0.75
    0.65 ~   5/2^3 = 0.625
    0.65 ~  10/2^4 = 0.625
    0.65 ~  21/2^5 = 0.65625
    0.65 ~  42/2^6 = 0.65625
    0.65 ~  83/2^7 = 0.648438
    0.65 ~ 166/2^8 = 0.648438
    
    0.35 ~   1/2^1 = 0.5
    0.35 ~   1/2^2 = 0.25
    0.35 ~   3/2^3 = 0.375
    0.35 ~   6/2^4 = 0.375
    0.35 ~  11/2^5 = 0.34375
    0.35 ~  22/2^6 = 0.34375
    0.35 ~  45/2^7 = 0.351562
    0.35 ~  90/2^8 = 0.351562

## 4\) Recherche de la meilleure approximation pour un numérateur maximum donné.

Plus le numérateur et l'exposant au dénominateur sont grands, plus l'approximation est précise. Dans `approxime(...)`, modifiez la boucle pour qu'elle recherche le meilleur couple numerateur/exposant, sans que le numérateur ne dépasse une certaine valeur maximale passez en argument à la fonction. Ne conservez les affichages que pour ce meilleur couple, mais afficher aussi la valeur du numérateur maximum autorisé. Utilisez le programme principal suivant :

``` cpp
int main()
 {
  std::cout << std::endl ;
  approxime(15,0.65) ;
  approxime(255,0.65) ;
  std::cout << std::endl ;
  approxime(15,0.35) ;
  approxime(255,0.35) ;
  std::cout << std::endl ;
  return 0 ;
 }
```

## 5\) Calcul du numérateur maximum en fonction du nombre de bits

La plus grande valeur utilisable pour le numérateur dépend du nombre de bits utilisés pour représenter ce nombre entier. Dans les arguments de `approxime(..)`, remplacez l'argument désignant la valeur maximale par un argument désignant le nombre maximum de bits et corrigez en conséquence le corps de la fonction, en utilisant la fonction `entier_max(...)`. Dans l'affichage, remplacez le numérateur maximum par le nombre de bits. Dans le programme principal, faites deux boucles qui font varier le nombre de bits de 2 à 8 par pas de 2, l'une qui appelle `approxime(...)` pour **0.65**, et l'autre pour **0.35**. L'affichage attendu est le suivant :

    2 bits : 0.65 ~   3/2^2 = 0.75
    4 bits : 0.65 ~  10/2^4 = 0.625
    6 bits : 0.65 ~  42/2^6 = 0.65625
    8 bits : 0.65 ~ 166/2^8 = 0.648438
    
    2 bits : 0.35 ~   3/2^3 = 0.375
    4 bits : 0.35 ~  11/2^5 = 0.34375
    6 bits : 0.35 ~  45/2^7 = 0.351562
    8 bits : 0.35 ~ 179/2^9 = 0.349609

## 6\) Déplacement des affichages dans une nouvelle fonction intermédiaire

Entre `main(...)` et `approxime(...)`, insérez la nouvelle fonction `teste_approxime(...)` ci-dessous, dans laquelle on regroupe tous les affichages. Corriger la fonction `main(...)` pour qu'elle appelle `teste_approxime(...)` à la place d'`approxime(...)`. Enlevez les affichages de la fonction `approxime(...)`, et ajoutez deux arguments par lesquels elle doit retourner les meilleurs numérateur et exposant qu'elle a calculé.

``` cpp
#include <iomanip>  
  
void teste_approxime( int bits, double valeur )
 {
  int numerateur, exposant, erreur ;
  approxime(bits,valeur,numerateur,exposant) ;
  double approximation = double(numerateur)/fois_puissance_de_deux(1,exposant) ;
  erreur = arrondi(100*(valeur-approximation)/valeur) ;
  if (erreur<0) { erreur = -erreur ; }
  std::cout
    << bits << " bits : " << valeur << " ~ "
    << std::setw(8) << approximation
    << " (" << erreur << "/100)" << std::endl ;
 }
```

## 7\) Multiplication approximée

Ecrivez la fonction `multiplie(...)` qui calcule le produit approximé d'un coefficient réel par un nombre entier et retourne un entier. Cette fonction doit approximer le réel à l'aide de la fonction `approxime(...)` ci-dessus. Les arguments de `multiplie(...)` sont le nombre maximum de bits pour l'approximation, le coefficient réel et le nombre entier. On ajoute une nouvelle boucle dans la fonction principale, afin de calculer l'approximation de **0.65\*3515+0.35\*4832** à l'aide de la nouvelle fonction `multiplie(...)`, pour un nombre de bits allant de 1 à 8 :

``` cpp
int main()
 {
  int bits ;
  
  std::cout << std::endl ;
  for ( bits=2 ; bits<=8 ; bits=bits+2 )
   { teste_approxime(bits,0.65) ; }

  std::cout << std::endl ;
  for ( bits=2 ; bits<=8 ; bits=bits+2 )
   { teste_approxime(bits,0.35) ; }

  std::cout<<std::endl ;
  for ( bits=1 ; bits<=8 ; bits=bits+1 )
   {
    int exact = arrondi(0.65*3515+0.35*4832) ;
    int approx = multiplie(bits,0.65,3515) + multiplie(bits,0.35,4832) ;
    std::cout << bits << " bits : 0.65*3515+0.35*4832 = " << exact << " ~ " << approx << std::endl ;
   }

  std::cout << std::endl ;
  return 0 ;
 }
```

## 8\) Déplacement des affichages dans une nouvelle fonction intermédiaire

De même que nous l'avons déjà fait pour la fonction `test_approxime(...)`, créez une fonction `test_somme(...)` qui va servir à centraliser les affichages liés aux tests sur la somme pondérée **0.65\*3515+0.35\*4832**. `test_somme(...)` doit différer de `test_approxime(...)` sur quelques points. Notamment, le calcul d'erreur doit être fait en millièmes, et la fonction `test_somme(...)` a cinq arguments : nombre de bits, premier coefficient réel, premier nombre entier, deuxième coefficient réel, deuxième nombre entier. Simplifiez le programme principal en ne laissant qu'un appel à `teste_somme(bits,0.65,3515,0.35,4832)` dans la dernière boucle. L'affichage à obtenir est le suivant :

    2 bits : 0.65 ~     0.75 (15/100)
    4 bits : 0.65 ~    0.625 (4/100)
    6 bits : 0.65 ~  0.65625 (1/100)
    8 bits : 0.65 ~ 0.648438 (0/100)
    
    2 bits : 0.35 ~    0.375 (7/100)
    4 bits : 0.35 ~  0.34375 (2/100)
    6 bits : 0.35 ~ 0.351562 (0/100)
    8 bits : 0.35 ~ 0.349609 (0/100)
    
    1 bits : 3976 ~ 2965 (254/1000)
    2 bits : 3976 ~ 4448 (119/1000)
    3 bits : 3976 ~ 4008 (8/1000)
    4 bits : 3976 ~ 3857 (30/1000)
    5 bits : 3976 ~ 3967 (2/1000)
    6 bits : 3976 ~ 4004 (7/1000)
    7 bits : 3976 ~ 3977 (0/1000)
    8 bits : 3976 ~ 3968 (2/1000)

## \*) Diagramme de classes UML de la solution

![img/02-variables-et-fonctions.uml.png](img/02-variables-et-fonctions.uml.png "img/02-variables-et-fonctions.uml.png")

© *[David Chamont](http://llr.in2p3.fr/spip.php?page=view_person&personID=121), Laboratoire Leprince-Ringuet (LLR) / IN2P3 / CNRS, Ecole polytechnique / Université Paris-Saclay*\\\\ © *Ce(tte) œuvre est mise à disposition selon les termes de la [Licence Creative Commons Attribution - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/)*
