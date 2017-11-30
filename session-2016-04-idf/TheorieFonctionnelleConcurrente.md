# [Programmation fonctionnelle et concurrente](README.md "wikilink")

## Introduction

Pour utiliser efficacement le matériel informatique moderne, il est nécessaire d'écrire des programmes pouvant tirer parti de son parallélisme, c'est à dire de sa capacité à effectuer plusieurs tâches simultanément. Ce parallélisme est présent à toutes les échelles, on peut ainsi :

  - Combiner la puissance de plusieurs d'ordinateurs au sein d'une même infrastructure (supercalculateurs, grilles de calcul)
  - Traiter simultanément plusieurs tâches indépendantes au sein d'une même machine (multi-coeurs, hyper-threading, E/S asynchrone)
  - Effectuer une opération simple, une addition par exemple, sur plusieurs valeurs à la fois (vectorisation)
  - ...et ce ne sont que des exemples, le matériel moderne (GPU, Xeon Phi, APU) offre d'autres possibilités encore

On trouve aujourd'hui toutes sortes d'outils pour écrire des programmes tirant parti de ce parallélisme, chacun y allant de sa propre approche. Mais la plupart d'entre eux sont basés sur le paradigme de programmation concurrente, où un programme est décomposé en plusieurs tâches pouvant être effectuées de façon plus ou moins indépendante.

Simultanément, le langage C++ évolue lui aussi pour mieux supporter le parallélisme et la programmation concurrente. Nous allons maintenant voir de quelle façon.

## Mécanismes d'inspiration fonctionnelle

Parce qu'elle facilite la parallélisation des codes, en évitant un certain nombre de problèmes liés à la modification de données en mémoire, la programmation fonctionnelle connaît un retour en grâce depuis quelques années. C++ a donc intégré plusieurs outils facilitant ce type de programmation.

  - [Inférence de type](TheorieClassiqueInference "wikilink") *[(question 1)](CoefsFonctionnelleConcurrente "wikilink")*
  - [Fonctions anonymes ("lambdas")](Cpp11Lambdas "wikilink") *[(question 2)](CoefsFonctionnelleConcurrente#a2Lambdas "wikilink")*
  - [For généralisé](Cpp11ForGeneralise "wikilink") *[(question 3)](CoefsFonctionnelleConcurrente#a3Forgénéralisé "wikilink")*

## Programmation concurrente

C++ permet désormais de décomposer un programme en un ensemble de tâches, lesquelles susceptibles de s'exécuter simultanément quand le matériel le permet. On parle de programmation concurrente.

  - [Processus légers ("threads")](Cpp11Threads "wikilink") *[(question 4)](CoefsFonctionnelleConcurrente#a4Parallélisationdestests "wikilink")*
  - [Données partagées](Cpp11SharedData "wikilink") *[(questions 5 et 6)](CoefsFonctionnelleConcurrente#a5Stresstest "wikilink")*
  - [Promesses de futurs](Cpp11Future "wikilink") *[(question 7)](CoefsFonctionnelleConcurrente#a7Appelsasynchrone "wikilink")*

## Compléments

  - [Variables de threads](Cpp11Tls "wikilink")
  - [Ce qui se prépare pour C++17/20](Cpp17Parallele "wikilink")

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
