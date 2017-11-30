# [Conclusion](README.md "wikilink")

## Morale

On ne peut pas comprendre le C++ sans comprendre les pointeurs. Pour autant, rares sont les cas où on doit encore les utiliser directement aujourd'hui. On ne peut pas décrypter un message d'erreur de compilation sans avoir un minimum de bases sur les patrons (templates). Pour autant, personne ne vous oblige à vous lancer dans la métaprogrammation générique. La bibliothèque standard a été bâtie pour vous simplifier la vie, et écrite par des spécialistes pour avoir les meilleures performances. Servez-vous en \!

  - [Do not use f\*cking pointers \!](http://klmr.me/slides/modern-cpp.pdf)

## C++17

Côté langage, des petits ajouts et améliorations :

  - if constexpr
  - lambdas constexpr
  - operator. (smart references)
  - génération de comparateurs par défaut (==, \!=, \<, \<=, \>, \>=)
  - attributs `[[fallthrough]]`, `[[nodiscard]]`, `[[maybe_unused]]`

Côté bibliothèque, du plus lourd :

  - version parallèle/vectorisée des conteneurs et algorithmes
  - interface standard au système de fichiers
  - extensions diverses (any, optional, string\_view, ...)

Et toujours pas de "concepts"... (une extension du langage permettant de définir des contraintes sur les paramètres des patrons, et de simplifier les messages d'erreur de compilation, en autres).

## Nouvelles règles de codage

L'automne dernier, Bjarne Stroustrup (créateur de C++) et Herb Sutter (responsable du comité de normalisation) ont officialisé un projet de règles de codage, destiné à faciliter l'adoption du C++ "moderne" et la diffusion de bonnes pratiques établies avec quelques experts.

Une bibliothèque est censée faciliter le respect de ces règles (Guideline Support Library, GSL). L'objectif est également de privilégier des règles vérifiables, et de permettre le développement d'un outil de vérification automatique (projet "Checker tool").

  - [Les règles](https://github.com/isocpp/CppCoreGuidelines)
  - [La première implémentation de GSL](https://github.com/microsoft/gsl)

## Actualité

  - [GCC 6.1 disponible : la norme C++14 utilisée par défaut](http://www.nextinpact.com/news/99604-gcc-6-1-disponible-norme-c14-utilisee-par-defaut.htm)

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
