# [Variables](TheorieClassique "wikilink")

\_\_FORCETOC\_\_

## Variables ordinaires

#### Déclaration

Pour être utilisable dans un programme C++, une variable doit être préalablement déclarée. Cette déclaration comprend au minimum le type de la variable, suivi de son nom et d'un point-virgule.

``` cpp
int nombre ; // variable de type entier
double pi ;  // variable de type réel
```

#### Initialisation

Bien que cela ne soit pas obligatoire, il est fortement recomandé de donner une valeur initiale à vos variables, sous forme d'une expression entre accolades. Si vous donnez des accolades sans valeurs, une valeur prédéfinie et associée au type est utilisée (en général une forme de 0).

``` cpp
int nb1 { 1 } ;       // variable de type entier
int nb2 {} ;          // équivaut à int nb2 { 0 };
double pi { 3.14 } ;  // variable de type réel
```

C++ supporte en realité de très nombreuses autres formes historiques d'initialisation, que vous croiserez partout, y compris dans ce cours, avec des parenthèses et/ou un signe égal. Il existe des différences subtiles entre chacune... que vous pouvez ignorer la plupart du temps.

``` cpp
int n = 5     ;
int n(5)      ;
int n = { 5 } ;
```

Dans tous les cas, même en présence d'un signe égal, il faut retenir qu'il s'agit d'une initialisation, et non d'une affectation (cela revêtira de l'importance quand nous définirons nos propres types).

#### Affectation

On enregistre une nouvelle valeur dans une variable existante à l'aide de l'opérateur d'affectation =. Le nom de la variable se trouve à gauche. L'expression à droite du signe = est évaluée, et son résultat est affecté à la variable.

``` cpp
int nb1 {}, nb2 {}, nb3 {} ;  // initialisations par défaut de nb1, nb2 et nb3
// Diverses affectations
nb1 = 1 ;
nb2 = nb1 ;
nb3 = nb1+1 ;
nb2 = nb2+1 ;
nb2 += 1 ;
```

## Références

Une référence est une variable qui partage sa valeur avec une autre. C'est une sorte d'alias, un autre nom pour la même valeur.

Lorsqu'on définit une référence, on doit immédiatement l'initialiser en indiquant sur quelle variable elle doit pointer. Ce n'est plus modifiable ensuite.

Toute affectation ultérieure porte sur la valeur partagée.

``` cpp
int a { 2 } ;
int b { a } ;
b = b + 1 ;

std::cout << a << std::endl ; // 2
std::cout << b << std::endl ; // 3

int & c { a } ; // initialisation
c = c + 1 ;     // affectation ultérieure

std::cout << a << std::endl ; // 3
std::cout << c << std::endl ; // 3
```

## Pointeurs

Un pointeur contient l'adresse d'une autre variable. Il se déclare en glissant un caractère \`\*\` avant le nom. On peut l'initialiser ou non avec l'adresse d'une autre variable. Pour extraire explicitement l'adresse de cette autre variable, on utilise la lettre \`&\`.

Ultérieurement, soit on veut modifier l'adresse stockée dans le pointeur, auquel cas on utilise le nom du pointeur directement, soit on veut modifier la variable pointée, auquel cas on préfixe le nom par \`\*\`.

On peut donc voir le pointeur comme une sorte de référence redéfinissable.

``` cpp
int a { 2 }, b { 3 } ;
int * p { &a } ;

std::cout << p  << std::endl ;  // 0xbff6e064
std::cout << &a << std::endl ;  // 0xbff6e064
std::cout << *p << std::endl ;  // 2
std::cout << a  << std::endl ;  // 2

*p = *p + 1 ;

std::cout << p  << std::endl ;  // 0xbff6e064
std::cout << &a << std::endl ;  // 0xbff6e064
std::cout << *p << std::endl ;  // 3
std::cout << a  << std::endl ;  // 3

p = &b ;
*p = *p + 1 ;

std::cout << p  << std::endl ;  // 0xbff6e060
std::cout << &b << std::endl ;  // 0xbff6e060
std::cout << *p << std::endl ;  // 4
std::cout << b  << std::endl ;  // 4
```

Un pointeur peut aussi ne désigner aucune variable, si on lui affecte la valeur spéciale \`nullptr\`. C'est alors une erreur d'essayer d'accéder à sa valeur pointée. Tout programme acceptant des pointeurs en paramètre doit gérer ce cas, ce qui augmente sa complexité. C'est une des raisons pour lesquelles il est préférable d'utiliser des références plutôt que des pointeurs lorsque c'est possible.

## Variables et pointeurs constants

Lors de la déclaration d'une variable de type prédéfini, on peut lui donner une valeur initiale et déclarer que la variable est constante. Le compilateur s'assurera qu'aucune instruction ultérieure n'essaie de modifier la valeur de la variable. Les **constantes** sont donc des variables qui peuvent être initialisées mais jamais modifiées.

``` cpp
double const pi { 3.1415 } ; // initialisation de la constante 
pi = 3.1416 ;              // ERREUR DE COMPILATION
```

Dans le cas d'un pointeur, on peut déclarer constant le pointeur lui-même, et/ou la valeur pointée selon que l'on place le mot-clé `const` avant ou après le nom de type (dans ce cas il s'applique à la valeur pointée) ou après la caractère `*` (dans ce cas il s'applique au pointeur lui-même).

``` cpp
int a { 2 }, b { 3 };

int * p1 { &a };             // Pointeur modifiable, permettant de modifier la valeur pointée
const int * p2 { &a };       // Pointeur modifiable, ne permettant pas de modifier la valeur pointée
int * const p3 { &a };       // Pointeur non modifiable, permettant de modifier la valeur pointée
const int * const p4 { &a }; // Pointeur non modifiable, ne permettant pas de modifier la valeur pointée

p1 = &b; // Autorisé
p2 = &b; // Autorisé
p3 = &b; // ERREUR DE COMPILATION
p4 = &b; // ERREUR DE COMPILATION

*p1 = 4; // Autorisé
*p2 = 4; // ERREUR DE COMPILATION
*p3 = 4; // Autorisé
*p4 = 4; // ERREUR DE COMPILATION
```

IMPORTANT : Même déclarée `const`, la valeur pointée n'est pas intrinsèquement constante. Elle ne peut juste pas être modifiée à travers ce pointeur précis.

Si d'autres variables référencent la même zone mémoire et ne sont pas const, elles permettent de modifier la valeur.

En revanche, le langage interdit, sans recours à des fonctionnalités spéciales, de créer un pointeur permettant de modifier une constante.

``` cpp
int a { 2 };
const int b { 3 };

const int * p_a { &a };  // Autorisé, ne permet pas de modifier a. Mais a reste modifiable par d'autres moyens.
int * p_b { &b };        // ERREUR DE COMPILATION. b est une constante, on ne peut pas déclarer un pointeur permettant de la modifier.
```

## Tableaux

L'opérateur `[]` permet de déclarer et d'utiliser des tableaux. L'initialisation se fait à l'aide de blocs de nombres.

``` cpp
int i[10] ;
int j[3] = { 1, 2, 3 } ;
int k[2][2] = { { 1, 2 }, { 3, 4 } } ;
std::cout << i[2] << std::endl ;     //  4196496 - N'a pas été initialisé !
std::cout << i[10] << std::endl ;    //  0
std::cout << j[1] << std::endl ;     //  2
std::cout << k[0][0] << std::endl ;  //  1
std::cout << k[0][1] << std::endl ;  //  2
std::cout << k[1][0] << std::endl ;  //  3
std::cout << k[1][1] << std::endl ;  //  4
```

## Tableaux et pointeurs

La variable désignant un tableau est assimilable à un pointeur constant qui pointe au début du tableau. Incrémenter ce pointeur revient à se déplacer dans le tableau.

``` cpp
int i[2] = { 10, 20 } ;
std::cout << *i << std::endl ;                                //  10
std::cout << *i+1 << std::endl ;                              //  11
std::cout << *(i+1) << std::endl ;                            //  20
i++ ; // ERREUR DE COMPILATION
int * j = i ;
j++ ; // correct
std::cout << *j << std::endl ;                                //  20
std::cout << "Valeur du pointeur de i=" << &i << std::endl ;  //  Valeur du pointeur de i=0x7fffbc0e5e60
std::cout << "Valeur du pointeur de j=" << &j << std::endl ;  //  Valeur du pointeur de j=0x7fffbc0e5e68
```

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
