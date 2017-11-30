# [Types prédéfinis](TheorieClassique "wikilink")

\_\_FORCETOC\_\_

## Booléens

Les variables de type \`bool\` peuvent prendre pour valeur \`true\` ou \`false\`.

On notera que que ce type n'existait pas à l'origine, et que les instructions C++ comprenant des conditions n'attendent pas nécessairement des valeurs booléennes, mais plutôt des entiers.

Il y a une forme d'équivalence entre booléens et entiers. Tout entier nul est équivalent à \`false\`, et toute autre valeur est équivalente à \`true\`.

``` cpp
#include <iostream>

int main()
{
  bool a = true, b(false), c{true};
  bool d;  // booléen de valeur NON DEFINIE !! - warning: 'd' is used uninitialized in this function [-Wuninitialized]
  if (d)
    std::cout << "La valeur de d est 'true'" << std::endl;  // Dépend de la valeur qui était présente en mémoire

  bool dix(10), zero(0);
  if (dix)
    std::cout << "La valeur de dix est 'true'" << std::endl;
  if (!zero)
    std::cout << "La valeur de zero est 'false'" << std::endl;

  return 0;
}
```

    #!txt
    [La valeur de d est 'true']
    La valeur de dix est 'true'
    La valeur de zero est 'false'

## Enumérations

#### Les énumérations historiques

Les enumérations historiques de C++ permettent de définir des constantes qui sont traitées comme des entiers, et qui peuvent s'initialiser à partir d'entiers.

``` cpp
enum color { red, green, blue } ;
std::cout << red << " " << green << " " << blue << std::endl;          //  0 1 2

enum shape { circle=10, square, triangle=20 };
std::cout << circle << " " << square << " " << triangle << std::endl;  //  10 11 20
```

#### Nouvelles énumérations

L'équivalence implicite entre énumérations historiques et entiers est une source d'erreurs potentielles :

``` cpp
enum color { red, green, blue } ;
enum shape { circle, square, triangle };
 
color c = red ;
bool flag = (c >= triangle); // erreur de type !
```

C++11 permet de déclarer de nouvelles énumérations constituant chacune un type à part entière, indépendant des entiers, ce qui protège des erreurs précédentes (mais demande un peu plus d'écriture de la part du programmeur) :

``` cpp
enum class Couleur { jaune, rouge, bleu, vert } ;
couleur c ;
.....
c = Couleur::jaune             // OK
c = Couleur::jaune+3 ;         // Interdit (accepté avec un enum ordinaire)
if (c==Couleur::rouge) .....   // OK
if (c==6) .....                // Interdit (accepté avec un enum ordinaire)
if (c>Couleur::rouge) .....    // Interdit, on ne peut pas tout avoir ;)
```

## Types numériques

#### Liste des types numériques

Les correspondances FORTRAN ci-dessous sont données à titre d'exemple. La taille des types numériques du C++ peut varier selon le processeur utilisé. Le standard C++ impose seulement "short\<=int\<=long" et "float\<=double\<=long double". Cela rend ces types prédéfinis non portables. Comme beaucoup de choses en C, et donc en C++, on privilégie la performance sur toute autre considération. Les types entiers et réels par défaut, \`int\`et \`double\`, sont supposés correspondre à la taille des registres du processeur et être les plus rapides (pour plus de détails sur les [types numériques](http://en.cppreference.com/w/cpp/language/types))

||= C++ =||= Fortran =||= Observations =|| || `short` || INTEGER\*2 || Au moins sur 16 bits || || `int` || INTEGER\*4 || Au moins sur 16 bits || || `long` || INTEGER\*8 || Au moins sur 32 bits || || `long long` || INTEGER\*16|| Au moins sur 64 bits || || `float` || REAL\*4 || || || `double` || REAL\*8 || || || `long double` || REAL\*16 || ||

Tous les types entiers (`short`, `int` et `long`) ont également une variante non signée, par exemple `unsigned int`, qui ne prend que des valeurs positives.

Notons également que le type `char` est l'équivalent d'un octet, et selon le contexte sera interprété comme un nombre ou comme un caractère.

Si vous avez besoin d'un type d'entier de taille définie, et ce quelque soit le type de processeur ou de plateforme utilisé, il faut utiliser ceux déjà définis dans <cstdint> pour C++11 (pour plus de détails sur [<cstdint>](http://en.cppreference.com/w/cpp/types/integer))

``` cpp
#include <iostream>
#include <limits>  // pour std::numeric_limits
#include <iomanip>  // pour std::setprecision

int main()
{
  int i = 10, j(9), k{7};   // entier normal
  std::cout << "i = " << i << " - j = " << j << " - k = " << k << std::endl;
  std::cout << "int [min, max] = [" << std::numeric_limits<int>::min() << ", " << std::numeric_limits<int>::max() << "]" << std::endl;

  short s = -32010;
  std::cout << "s = " << s << " - short [min, max] = [" << std::numeric_limits<short>::min() << ", " << std::numeric_limits<short>::max() << "]" << std::endl;
  unsigned short us = 67010;  // 67010 > std::numeric_limits<unsigned short>::max() - warning: large integer implicitly truncated to unsigned type
  std::cout << "s = " << s << " - us = " << us << std::endl;

  long lo = -3000000000; unsigned long ulo = 3000000000;
  std::cout << "lo = " << lo << " - ulo = " << ulo << std::endl;

  float f = 5.3f; float g = 1e-3;
  double d = 3.141592653589793; long double ld = std::numeric_limits<long double>::max();
  std::cout << "f = " << f << " - g = " << g << " - d = " << d << " - ld = " << ld << std::endl;
  std::cout << "d avec une précision de 10 = " << std::setprecision(10) << d << std::endl;

  return 0;
}
```

    #!txt
    i = 10 - j = 9 - k = 7
    int [min, max] = [-2147483648, 2147483647]
    s = -32010 - short [min, max] = [-32768, 32767]
    s = -32010 - us = 1474
    lo = -3000000000 - ulo = 3000000000
    f = 5.3 - g = 0.001 - d = 3.14159 - ld = 1.18973e+4932
    d avec une précision de 10 = 3.141592654

#### Les conversions entre types numériques

En C++11, il y a une différence de comportement du compilateur entre l'initialisation et l'affectation. Les pertes de précision sont autorisées lors d'une affectation, mais pas lors d'une initialisation entre accolades.

``` cpp
  float f = 1.12345678901234567890;
  double d = 2.12345678901234567890;
  float f_d(d);
  float f_dd = d;
  float f_ddd{d};  // warning: narrowing conversion of 'd' from 'double' to 'float' inside { } [-Wnarrowing]
  float f_tab[] = {d};  // warning: narrowing conversion of 'd' from 'double' to 'float' inside { } [-Wnarrowing]
```

Les pertes de précision sont détectées lors d'une conversion :

  - d'un type à virgule flottante (`long double`, `double` et `float`) vers un type entier.
  - d'un `long double` vers un `double` ou un `float`, sauf si la source est constante et que sa valeur va tenir dans le type de la destination
  - d'un `double` vers un `float`, sauf si la source est constante et que sa valeur va tenir dans le type de la destination
  - d'un type entier vers un énuméré ou un type à virgule flottante, sauf si la source est constante et que sa valeur va tenir dans le type de la destination
  - d'un type entier vers un énuméré ou un autre type entier, sauf si la source est constante et que sa valeur va tenir dans le type de la destination

#### Les conversions implicites

Les conversions implicites entre les différents types numériques sont assurées en C++. Ce mécanisme est hérité du C. Dans ce cas là, le programmeur laisse le compilateur gérer la conversion.

``` cpp
#include <iostream>
#include <climits>  // pour INT_MAX
#include <limits>  // pour std::numeric_limits

int main()
{
  unsigned short i = 42000;
  short j = i;
  unsigned short k = j;
  std::cout << "i = " << i << " - j = " << j << " - k = " << k << std::endl;
  int l = INT_MAX;
  k = l;
  short m = std::numeric_limits<unsigned short>::max();
  std::cout << "k = " << k << " - l = " << l << " - m = " << m << std::endl;
  
  float a = std::numeric_limits<double>::max();
  double b = 3e38;
  long double c = b;
  std::cout << "a = " << a << " - b = " << b << " - c = " << c << std::endl;
  
  double d = 3.25;
  int e = d;
  std::cout << "d = " << d << " - e = " << e << std::endl;
}
```

    #!txt
    i = 42000 - j = -23536 - k = 42000
    k = 65535 - l = 2147483647 - m = -1
    a = inf - b = 3e+38 - c = 3e+38
    d = 3.25 - e = 3

#### Les conversions explicites héritées du C

Dans le cas d'une conversion explicite, le programmeur dit explicitement quelle conversion utiliser. Le C++ hérite du mécanisme de forçage de type du C :

``` cpp
unsigned short i = 42000 ;
short j = short(i) ;
unsigned short k = (unsigned short)(j) ;
```

#### Les conversions explicites par static\_cast

C++ a également redéfini une famille de forçages de type, plus verbeux mais plus précis. Le type de forçage à utiliser par défaut, qui entend remplacer celui du C, est le `static_cast` :

``` cpp
unsigned short i = 42000;
short j = static_cast<short>(i);
unsigned short k = static_cast<unsigned short>(j);
```

#### Les conversions explicites par const\_cast

Ce type de forçage est le seul autorisé pour enlever la constance d'une valeur pointée ou référencée. Il ne doit être utilisé qu'avec parcimonie. Il est par ailleurs plus visible et plus facile à retrouver qu'un forçage façon C.

``` cpp
#include <iostream>

void sortie_const (const char* chaine_const) {
  std::cout << chaine_const << std::endl;
}
void sortie (char* chaine) {
  std::cout << chaine << std::endl;
  chaine[0] = 'a'; // Vu que l'argument n'est plus constant, on y a accès. Le comportement n'est pas garanti.
}

int main () {
  char bonjour[] = "bonjour";
  sortie_const ( const_cast<const char *> (bonjour) );
  
  const char* const_bonjour = "bonjour";
  sortie ( const_cast<char *> (const_bonjour) );
  
  std::cout << const_bonjour << std::endl;
  
  return 0;
}
```

    #!txt
    bonjour
    bonjour
    bash: line 7: 27543 Segmentation fault      (core dumped) ./a.out

#### Les conversions explicites par reinterpret\_cast

Encore plus dangereux que le précédent, ce forçage de type ne modifie en rien ce qui est codé en mémoire, mais se contente de changer le type officiel rattaché à la zone mémoire. C'est un dernier recours que nous ne détaillerons pas ici.

</source>

## Caractères et chaînes de caractères

#### Chaînes historiques

``` cpp
#include <iostream>
#include <cstring>  // Pour strlen, strcpy

int main()
{
  // Version statique
  char bonjour[] = {'b','o','n','j','o','u','r','\0'};
  char coucou[] = "coucou";
  char* salut = "salut";  // A éviter en C++
  char copie[10] = {0}; // = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
  
  strcpy(copie, bonjour);
  std::cout << "la chaine '" << copie << "' est de taille " << strlen(copie) << std::endl;
  
  strncpy(copie, coucou, strlen(coucou));
  copie[strlen(coucou)] = '\0';  // Ne pas oublier de terminer la chaine
  std::cout << "la chaine '" << copie << "' est de taille " << strlen(copie) << std::endl;

  // Version dynamique en C
  char* dynamique = (char*)malloc(10*sizeof(char));
  strcpy(dynamique, "dynamique");
  dynamique = (char *)realloc(dynamique,25*sizeof(char));  // Re-allocation dynamique
  strcpy(dynamique, "dynamique et flexible");
  std::cout << "la chaine '" << dynamique << "' est de taille " << strlen(dynamique) << std::endl;
  
  free(dynamique);

  // Version dynamique en C++
  char* dynamique_cpp = new char[15];
  strcpy(dynamique_cpp, "version C++");
  delete[] dynamique_cpp;
  dynamique_cpp = new char[50];
  strcpy(dynamique_cpp, "dynamique version C++ et flexible");
  std::cout << "la chaine '" << dynamique_cpp << "' est de taille " << strlen(dynamique_cpp) << std::endl;

  return 0;
}
```

    #!txt
    main.cpp:9:17: warning: ISO C++ forbids converting a string constant to 'char*' [-Wpedantic]
       char* salut = "salut";  // A éviter en C++
    
    la chaine 'bonjour' est de taille 7
    la chaine 'coucou' est de taille 6
    la chaine 'dynamique et flexible' est de taille 21
    la chaine 'dynamique version C++ et flexible' est de taille 33

Il existe tout un tas de fonctionnalités pour travailler à partir des chaînes de caractères historiques. Pour plus d'informations, n'hésitez pas à consulter [Cpluplus Reference sur les cstring](http://www.cplusplus.com/reference/cstring/).

#### Nouvelles chaînes

C++11 introduit de nouveaux types de caractères étendus, et les chaînes qui vont avec Il existe à présent :

  - `char` : doit maintenant supporter UTF-8 ;
  - `wchar_t` : caractère multi-octets (nombre d'octets variable), existant déjà en C++03 ;
  - `char16_t` : caractère unicode UTF-16 ;
  - `char32_t` : caractère unicode UTF-32.

Les différentes façons de définir des chaines se notent maintenant :

``` cpp
"bonjour"   // tableau constant de char, terminé par 0
L"bonjour"  // tableau constant de wchar_t, terminé par 0
u8"bonjour" // tableau constant de char, codés suivant UTF-8, terminé par 0
u"bonjour"  // tableau constant de char16_t, codés suivant UTF-16, terminé par 0
U"bonjour"  // tableau constant de char32_t, codés suivant UTF-32, terminé par 0
```

#### std::string

En C++ moderne, plutôt que de s'embarrasser des tableaux de caractères qui nous viennent du langage C, on utilisera le type std::string, fourni par le biais de la biliothèque standard du langage. Pour pouvoir utiliser ce type, il faut inclure le fichier d'entête <string> à l'aide de la commande préprocesseur `#include` :

``` cpp
#include <iostream>
#include <string>  // Pour std::string

void printchar(const char* chaine_de_caractere) {
  printf("%s\n", chaine_de_caractere);
}

int main()
{
  std::string espace;
  espace = " ";
  std::string bonjour = "Bonjour !";
  std::string commentcava("Comment ça va ?");
    
  std::string phrase = bonjour + " " + commentcava;
  std::cout << phrase << std::endl;
  std::cout << "La phrase contient " << phrase.size() << " caractères." << std::endl;
  
  phrase.resize(9);
  if (phrase == bonjour)
    std::cout << "C'est la même phrase." << std::endl;

  phrase.append(espace);
  phrase.append(commentcava);
  std::cout << phrase.substr(phrase.find("Comment")) << std::endl;

  int reponse = 42;
  std::string reponse_str = "La réponse à la grande question sur la vie est : " + std::to_string(reponse);
  printchar(reponse_str.data());
  
  std::string verlan(bonjour.rbegin(), bonjour.rend());
  std::cout << "endroit = '" << bonjour << "' - envers = '" << verlan << "'" << std::endl;

  return 0;
}
```

    #!txt
    Bonjour ! Comment ça va ?
    La phrase contient 26 caractères.
    C'est la même phrase.
    Comment ça va ?
    La réponse à la grande question sur la vie est : 42
    endroit = 'Bonjour !' - envers = '! ruojnoB'

En C++, on utilise les `std::string` plutôt que les chaines de caractère historiques du C, car elles garantissent qu'il n'y aura ni fuite mémoire, ni dépassement en mémoire. De plus, les `std::string` sont données pour être au moins aussi performantes que les chaines de caractère historiques du C, voire plus rapide pour certains cas. Il existe tout un tas de fonctionnalités pour les `std::string` déjà définies (cf. [std::string Cplusplus Reference](http://www.cplusplus.com/reference/string/string/))

## Le pointeur nul

En C++, les pointeurs et les adresses de variables sont considérés comme des entiers. Comme pour les booléens ou les énumérations historiques, cela peut conduire à des ambiguités ou des comportements incorrects, dans les contextes où il faut différencier entier et pointeur. En particulier quand un pointeur ne pointe sur aucune variable, ce que l'on signifiait en lui donnant la valeur entière 0.

Pour améliorer le traitement des pointeurs nuls, C++11 introduit une valeur spécifique, `nullptr`, de type `nullptr_t`. Cette nouvelle valeur ne peut pas être convertie implicitement en entier (par contre, elle peut l'être en booléen).

``` cpp
#include <cstddef>
#include <iostream>

void fonction(int i) {
  std::cout << "Surcharge de la fontion pour un entier\n";
}
void fonction(void* pi) {
  std::cout << "Surcharge de la fontion pour un pointeur void\n";
}
void fonction(int* pi) {
  std::cout << "Surcharge de la fontion pour un pointeur sur un entier\n";
}
void fonction(std::nullptr_t nullp) {
  std::cout << "Surcharge de la fontion pour un pointeur null\n";
}

int main()
{
  fonction(0); // appelle fonction(int), mais 0 peut aussi être un pointeur null
  fonction(static_cast <int*>(0)); // appelle fonction(int *) comme espéré
  int* pi;
  fonction(pi);
  fonction(static_cast <void*>(pi)); // appelle fonction(void *) comme espéré
  fonction(nullptr); // Cet appel serait ambigu sans la définition de void fonction(nullptr_t)
  '' fonction(NULL); '' error: call of overloaded 'fonction(NULL)' is ambiguous
}
```

    #!txt
    Surcharge de la fontion pour un entier
    Surcharge de la fontion pour un pointeur sur un entier
    Surcharge de la fontion pour un pointeur sur un entier
    Surcharge de la fontion pour un pointeur void
    Surcharge de la fontion pour un pointeur null

## Renommage de types

Il peut être pratique de définir un alias (nom alternatif) pour un type, par exemple pour pouvoir changer très facilement la précision de calcul d'un code. La syntaxe historique pour le faire passe par le mot clef `typedef`.

``` cpp
#include <iostream>
#include <iomanip>

typedef float reel ;

reel circular_area(reel r)
 {
  return 3.1415926535897932385 * r * r ;
 }

int main()
 {
  std::cout
   <<"double : "
   <<std::setprecision(15)
   <<circular_area(1)
   <<std::endl ;
 }
```

En C++ moderne, on utilisera plutôt la syntaxe qui s'appuie sur le mot-clef `using`, qui a de multiples autres usages que nous verrons ultérieurement :

``` cpp
#include <iostream>
#include <iomanip>

using reel = double ;

reel circular_area(reel r)
 {
  return 3.1415926535897932385 * r * r ;
 }

int main()
 {
  std::cout
   <<"double : "
   <<std::setprecision(15)
   <<circular_area(1)
   <<std::endl ;
 }
```

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
