# [Inférence de types](TheorieFonctionnelleConcurrente.md)

## Le mot-clé `auto`

C++ est statiquement et fortement typé : le programmeur doit préciser au compilateur le type de toutes les variables qu'il utilise, ce qui peut conduire à un code difficile à écrire comme à lire :

``` cpp
std::map<std::string,std::unique_ptr<some_data>> m ;
std::map<std::string,std::unique_ptr<some_data>>::iterator iter { m.find("my key") } ;
```

Traditionnellement, on allégeait les notations en ayant recours aux typedefs. Cela fonctionne toujours en C++11, mais on dispose à présent d'un nouvel outil : si une variable est initialisée, lors de sa déclaration, à l'aide d'une valeur de même type, on peut spécifier le type de la variable à l'aide du mot clé `auto`, indiquant au compilateur qu'il doit utiliser le même type que celui de la valeur initiale. Ainsi, l'exemple de notre itérateur peut être simplifié ainsi :

``` cpp
std::map<std::string,std::unique_ptr<some_data>>  ;
auto iter = m.find("my key");
```

Notez, dans cet exemple, le remplacement des accolades par le signe \`=\`. Il existe en effet en C++11 et en C++14 une interaction indésirable entre les règles régissant l'initialisation de variables par des accolades, et celles définissant comment une variable \`auto\` déduit le type de la valeur qu'on lui affecte. Il est prévu de résoudre ce problème dans la norme C++17.

L'utilisation d'auto peut grandement simplifier la déclaration des variables, en particulier quand le nom complet du type est très long. Et ce mot clé est particulièrement efficace en conjonction avec l'instruction `for` généralisée. Ainsi, à la place de :

``` cpp
std::vector<int> v ;
for ( std::vector<int>::iterator itr = v.begin() ; itr = v.end() ; ++itr )
{
   int & valeur = *itr;
   // Code utilisant ou modifiant "valeur"
}
```

On peut maintenant écrire :

``` cpp
std::vector<int> v ;
for ( auto & valeur : v )
{
   // Code utilisant ou modifiant "valeur"
}
```

## Formes modifiées de `auto`

Le type déduit par \`auto\` ne conserve pas certaines caractéristiques du type de la valeur d'origine. Par exemple,

  - Les modificateurs \`const\` sont ignorés, sauf dans le cas d'un pointeur ou d'une référence vers une constante.
  - Les modificateurs de référence \`&\` sont aussi ignorés.
  - Les tableaux et fonctions sont traités comme des pointeurs.

<!-- end list -->

``` cpp
const int a { 3 };
const int * const p { &a };
auto b = a; // b est de type int
auto q = p; // q est de type const int *

...

int d { 3 };
int & e { d };
auto f = e; // f est une copie de d de type int, et non une référence vers d de type int &

...

int tab[3];
auto ptr = tab; // ptr est de type int *
```

Les [règles d'inférence de type sous-jacentes](http://en.cppreference.com/w/cpp/language/template_argument_deduction#Other_contexts) sont basées sur celles utilisées par les compilateurs pour tenter de déduire automatiquement les paramètres des fonctions génériques lorsque ces derniers ne sont pas fournis par le programmeur. Elles permettent, entre autres choses, d'obtenir plus de flexibilité dans l'utilisation d'\`auto\`. En effet, si le modificateur \`const\` était conservé, par exemple, on ne pourrait pas utiliser \`auto\` pour créer un entier à partir d'une constante entière.

Pour restaurer certaines des caractéristiques ignorées par l'inférence de type, ou pour créer des types différents du type original, il est possible d'ajouter des modificateurs à \`auto\` :

``` cpp
const int a { 3 };
const int * const p { &a };
const auto b = a; // b est de type const int
const auto q = p; // c est de type const int * const

...

int d { 3 };
int & e { d };
auto & f = e; // f est une référence vers d de type int &
const auto & g = e; // g est une référence de type const int &, ne permettant pas de modifier d
```

## Le mot-clé `decltype`

On peut également déclarer une variable du même type qu'une expression quelconque, à l'aide du nouveau mot-clé `decltype` :

``` cpp
int n ;
double x ;
decltype (2*x+n) z ; // z sera du type de 2*x+n, soit ici double
.....
std::map<int,std::string> m1 ;
decltype(m1) m2 ;    // m2 aura le même type que m1 ;
```

## Afficher le type déduit par `auto`

Pour tirer certaines informations d'un compilateur C++, il est courant de lui fournir une instruction volontairement erronée, écrite de telle sorte que le message d'erreur résultant contiendra l'information recherchée.

Ci-dessous, nous déclarons un patron de classe qui n'est jamais défini, ce qui permet d'en savoir plus sur le type déduit par auto pour x et y :

``` cpp
const int theAnswer = 42;
auto x = theAnswer;
auto y = &theAnswer;

template<typename T> // declaration only for TD;
class TD;            // TD == "Type Displayer"

TD<decltype(x)> xType; // elicit errors containing
TD<decltype(y)> yType; // x's and y's types
```

Certains outils d'édition de code tentent de vous informer de façon interactive sur les types associés aux utilisations de `auto` dabs votre code, vace plsu ou moins de succès.

De même vous pourriez être tentés d'utiliser `typeid` et `std::type_info::name`, mais ces fonctions sont requises d'éliminer les références, les const, et présentent leur résultat sous une forme mutilée (mangled). Si vous voulez mettre des instructions d'affichage de type dans votre code, la meilleure piste est sans doute la bibliothèque Boost \!TypeIndex.

## Débat : un risque de d'utilisation excessive de l'inférence ?

L'utilisation de l'inférence de type apporte beaucoup de bénéfices :

  - une écriture simplifiée,
  - un algorithme plus lisible,
  - une protection contre les fautes de frappes dans les noms de types,
  - une plus grande généricité, donc un remaniement et une évolution plus facile du code.

Devant le succès et l'efficacité d'\`auto\`, le comité C++ a cherché d'élargir autant que possible ses usages possibles. Pour ne citer qu'un exemple, on peut par exemple utiliser `auto` comme type de retour d'une fonction :

``` cpp
auto computeLength()
{
  int length ;
  ...
  return length ;
}
```

Cependant, nous conclurons sur une mise en garde : tout à la joie de ne plus avoir à expliciter tous les types, on peut devenir extrémiste et couvrir son code d' `auto`, même lorsque le type d'une variable est parfaitement connu, déterministe et rapide à taper. Ceci pour adopter un style de programmation ou tous les types sont déduits par le compilateur.

Mais même quand cela est possible, il faut être bien conscient que le code résultant peut devenir moins lisible, car il y est plus difficile de connaître le type d'une variable, donc de trouver et corriger les erreurs de type.

De plus, l'utilisation excessive d'auto peut briser une abstraction, en amenant un compilateur à accepter un objet d'un type incorrect dans l'initialisation d'une variable, ou comme paramètre d'une fonction, pour ne s'en plaindre que plus tard (lorsque le code en fait un usage incohérent).

Enfin, le mot-clé \`auto\` peut apparaitre dans de nombreux contextes, chacun utilisant des règles d'inférence de type différentes et complexes, comme on l'a vu avec l'initialisation par accolades. Il est donc aisé de commettre une erreur dans son utilisation.

Par conséquent, en toute chose, mesure est bonne...

## Exercice

L'auteur du code suivant s'attend à ce que la boucle modifie le contenu du tableau \`valeurs\`. Il n'en est rien. Pourquoi ?

``` cpp
vector<int> valeurs { 1, 2, 3 };
for ( auto val : valeurs )
{
   val = val + 1;
}
```

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont et Hadrien Grasland, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
