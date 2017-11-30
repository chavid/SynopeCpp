# [C++11/14 : Références temporaires et déplacement de données internes](TheorieBonnesPratiquesBiblio.md)

## Motivation : éliminer les copies profondes inutiles

Dans de nombreuses situations, on est amené à réaliser des copies profondes inutiles. Dans l'exemple ci-dessous, lors de l'échange entre les deux instances de classe Texte, on est amené à faire 3 désallocations de mémoire, 3 allocations, 3 boucles de copies de caractères... là ou 3 copies de pointeurs suffiraient.

``` cpp
#include <cstring>

class Texte
 {
  public :
    Texte( const char * str )
     {
      taille_ = std::strlen(str)+1 ;
      donnees_ = new char [taille_] ;
      std::copy(str,str+taille_,donnees_) ;
     }
    Texte( const Texte & t )
     : taille_(t.taille_), donnees_(new char [t.taille_])
     { std::copy(t.donnees_,t.donnees_+taille_,donnees_) ; }
    Texte & operator=( const Texte & t )
     {
      if (this == &t) return *this ;
      delete [] donnees_ ;
      taille_ = t.taille_ ;
      donnees_ = new char [t.taille_] ;
      std::copy(t.donnees_,t.donnees_+taille_,donnees_) ;
      return *this ;
     }
    ~Texte()
     { delete [] donnees_ ; }
    friend std::ostream & operator<<( std::ostream & os, Texte const & t )
     { return os<<t.donnees_ ; }
  private :
    unsigned int taille_{0} ;
    char * donnees_{nullptr} ;
 } ;

int main()
 {
  Texte t1("world") ;
  Texte t2("hello") ;
  
  // échange de valeurs
  Texte tmp = t1 ;
  t1 = t2 ;
  t2 = tmp ;
  
  return 0 ;
 }
```

## Une réponse traditionnelle : permettre l'échange des données internes

En permettant à deux textes d'échanger (swap) leurs données internes, on peut réécrire notre programme de façon beaucoup plus économe en temps d'exécution :

``` cpp
class Texte
 {
  public :
    ...
    void echange( Texte & t )
     { 
      unsigned int taille_tmp = taille_ ;
      char * donnees_tmp = donnees_ ;
      taille_ = t.taille_ ;
      donnees_ = t.donnees_ ;
      t.taille_ = taille_tmp ;
      t.donnees_ = donnees_tmp ;
     }
    ...
 } ;

int main()
 {
  Texte t1("world") ;
  Texte t2("hello") ;
  t1.echange(t2) ;
  return 0 ;
 }
```

Voyons à présent comment C++11 introduit des notions nouvelles permettant de résoudre ce problème (et de nombreux autres) de façon plus élégante.

## Rappel sur les références en C++03

Les références C++ permettent d'attacher un nouveau nom à pour un objet existant de la pile ou du tas. Tous les accès et les modifications faites à travers la référence affectent l'objet original.

``` cpp
int var=42;
int& ref=var; // Create a reference to var
ref=99;
assert(var==99); // Original updated because of assignment to reference
```

Une référence ne peut être attachées qu'à une valeur stable (left value), susceptible de recevoir une nouvelle valeur ultérieurement, et jamais à une valeur temporaire (right value) telle qu'une expression littérale ou un objet temporaire créé par conversion implicite.

``` cpp
int & i=42 ; // Won’t compile
void print( std::string & s ) ; 
print("hello") ; // Won't compile
```

Exception notable : une référence **constante** (abus de langage désignant une référence vers une valeur constante) peut être attachée à une valeur temporaire, notamment pour faciliter les conversions implicites :

``` cpp
int const & i=42 ; // OK
void print( std::string const & s ) ; 
print("hello") ; // OK
```

## C++11/14 : les références temporaires

Pour aller plus loin, C++11 introduit le concept de référence temporaire (right value reference), qui ne peut désigner que des valeurs temporaires, et se déclare à l'aide d'un double &.

``` cpp
int&& i=42;
int j=42;
int&& k=j; // Won’t compile
```

On peut maintenant surdéfinir (overload) une fonction pour différencier le traitement à effectuer selon qu'on lui fournit une valeur stable ou une valeur temporaire. Ci-dessous, la fonction f est fournie en trois variantes :

``` cpp
void f( T & )       ; // I : applicable seulement à une valeur stable
void f( const T & ) ; // II : applicable à toute valeur, mais ne peut pas la modifier
void f( T && )      ; // III : applicable seulement à une valeur temporaire
```

En cas d'appel de f avec une valeur temporaire, c'est maintenant la forme III qui sera invoquée, si elle est définie. Cela constitue la pierre angulaire de la notion de déplacement.

## Notion de déplacement

Lorsqu'on sait qu'une valeur est temporaire, on doit pouvoir s'en resservir, ou "piller" son contenu sans conséquence néfaste. Le "déplacer" au lieu de le copier. Lorsqu'on manipule des grandes structures de données dynamiques, cela peut économiser de nombreuses operations couteuses.

Prenons une fonction qui recoit un vecteur d'entiers et le duplique pour le modifier. L'ancienne façon de faire serait la suivante :

``` cpp
void traitement(std::vector<int> const& vec)
{
  std::vector<int> copie(vec);
  ... // operations sur copie
}
```

Si l'objet original est temporaire, le copier est inutile. On peut en tirer partie grâce à cette surdéfinition de la fonction :

``` cpp
void traitement(std::vector<int> && vec)
{
  ... // operations directement sur vec
}
```

Maintenant, si la fonction est un constructeur, et que la classe possède des données internes allouées dynamiquement, on peut s'éviter de couteuses allocations, désallocations et copies :

``` cpp
class X
{
private:
  int* data_;
public:
  X(): data_(new int[1000000])
   {}
  ~X() 
   { delete [] data_; }
  X(const X& other): data_(new int[1000000])
   { std::copy(other.data_,other.data_+1000000,data_); }
  X(X&& other): data(other.data_)
   { other.data_=nullptr; }
};
```

Le second constructeur est appelé "constructeur par déplacement". Quand un type d'objet doit être "déplaçable", on dote sa classe d'un constructeur de déplacement et d'un opérateur d'affectation par déplacement.

``` cpp
A( const A & )             ; // constructeur de recopie
A( A && )                  ; // constructeur par déplacement
A & operator=( const A & ) ; // affectation par copie
A & operator=( A && )      ; // affectation par déplacement
```

## std::move

On peut vouloir dire explicitement qu'un objet ne servira plus et qu'il est déplaçable. Cela se fait en utilisant static\_cast\<X&&\> ou en appelant std::move().

``` cpp
X x1;
X x2=std::move(x1);
X x3=static_cast<X&&>(x2);
```

Notez bien que l'appel à std::move ne déplace pas x1 en soi. Il en fait seulement une valeur temporaire aux yeux du compilateur, donc un objet "éventuellement" déplaçable si le contexte s'y prête. Dans l'exemple ci-dessus, x1 est déplacé dans x2, au lieu d'être copié, si et seulement si la classe X est dotée du constructeur par déplacement adéquat. Même chose si on utilise le static\_cast\<X&&\>.

Le déplacement est par exemple utilisable pour rendre beaucoup plus efficace l'échange entre deux chaines de caractères. Si l'on reprend notre exemple initial de classe Texte :

``` cpp
class Texte
 {
  public :
    ...
    Texte( const Texte & t )
     : taille_(t.taille_), donnees_(new char [t.taille_])
     { std::copy(t.donnees_,t.donnees_+taille_,donnees_) ; }
    Texte & operator=( const Texte & t )
     {
      if (this == &t) return *this ;
      delete [] donnees_ ;
      taille_ = t.taille_ ;
      donnees_ = new char [t.taille_] ;
      std::copy(t.donnees_,t.donnees_+taille_,donnees_) ;
      return *this ;
     }
    Texte( Texte && t )
     : taille_(t.taille_), donnees_(t.donnees_)
     {
      t.taille_ = 0 ;
      t.donnees_ = nullptr ;
     }
    Texte & operator=( Texte && t )
     {
      if (this == &t) return *this ;
      delete [] donnees_ ;
      taille_ = t.taille_ ;
      donnees_ = t.donnees_ ;
      t.taille_ = 0 ;
      t.donnees_ = nullptr ;
      return *this ;
     }
    ~Texte()
     { delete [] donnees_ ; }
    ...
  private :
    unsigned int taille_{0} ;
    char * donnees_{nullptr} ;
 } ;

int main()
 {
  ...
  // échange de valeurs
  Texte tmp = std::move(t1) ;
  t1 = std::move(t2) ;
  t2 = std::move(tmp) ;
  ...
 }
```

Sans l'utilisation conjointe des références temporaires et du déplacement, on aurait du procéder à trois copies, avec leurs lots d'allocations/désallocations et de copies profondes. La classe std::string de la bibliothèque standard est évidemment "déplaçable", et on peut donc écrire avec succès :

``` cpp
void echange( std::string & a, std::string & b )
 {
  std::string tmp = std::move(a) ;
  a = std::move(b) ;
  b = std::move(tmp) ;
 }
```

Un point crucial maintenant : si une fonction reçoit un argument de type référence temporaire (qui ne peut donc être attaché qu'à une valeur temporaire), au sein de la fonction cet argument est considéré comme une variable tout ce qu'il y a de stable (on peut parfaitement le mettre à gauche d'un = et lui réaffecter une nouvelle valeur). Si la fonction ne pille pas elle-même le contenu de la variable, et qu'elle le transmet à une autre fonction (ou constructeur ou opérateur), elle ne peut réactiver son caractère temporaire qu'à l'aide d'un appel à std::move.

``` cpp
void do_stuff(X&& x)
{
  X a(x);            // Copies
  X b(std::move(x)); // Moves
}
do_stuff(X()); // OK, rvalue binds to rvalue reference
X x;
do_stuff(x);   // Error, lvalue can’t bind to rvalue reference
```

Tous les conteneurs de la bibliothèque standard sont maintenant enrichis de constructeurs et d'opérateurs d'affectation par déplacement.

Par ailleurs, la notion de déplacement ne sert pas uniquement à améliorer les performances. Il y a des classes pour lesquelles il est illogique que les objets soient copiables, mais ou il est nécessaire qu'ils soient déplacables. Dans ce cas, la classe possède un constructeur par déplacement, et pas de constructeur par copie. C'est notamment utilisé pour la gestion des threads ou les pointeurs améliorés.

Un objet qui a été "vidé" à la suite d'un déplacement n'est plus supposé servir à rien, si ce n'est être détruit, mais il est tout de même recommandé, quand vous implémenter ce type de classe et de déplacement, de laisser l'objet vidé dans un état "valide".

Si on l'appelle en lui passant une valeur temporaire, alors `T` est déduit du même type que la valeur :

    #!cpp
    foo(42);            // Calls foo<int>(int&&)
    foo(3.14159);       // Calls foo<double>(double&&)
    foo(std::string()); // Calls foo<std::string>(std::string&&)
    </source>
    
    Par contre, si on l'appelle avec une valeur non temporaire, alors <code>T</code>
    et le type de l'argument sont considérés comme des références classiques :
    
    <source lang="cpp">
    int i=42;
    foo(i); // Calls foo<int&>(int&)
    </source>
    
    Cela permet d'avoir un seul patron de fonction acceptant à la fois les
    valeurs temporaires et non temporaires, et capable de réaliser un
    déplacement plutôt qu'une copie quand c'est possible.
    
    
    == std::forward ==
    
    Dans le cas d'une fonction appelant une autre fonction, on fait de nouveau
    face au problème de préservation du caractère temporaire d'une valeur.
    Pour les fonctions ordinaires, std::move faisait l'affaire. Mais dans le
    cadre d'un patron de fonction et d'une référence universelle, on voudrait
    pouvoir préserver la propriété temporaire des variables temporaires,
    mais ne pas ajouter cette propriété aux variables stables. C'est
    précisemment
    le rôle de la fonction <code>std::forward</code>.
    
    <source lang="cpp">
    template<typename T>
    void do_stuff(T&& t)
    {
     ...
      do_other_stuff(std::forward<T>(t));
    }
    </source>
    
    On parle alors de '''suivi parfait''' (perfect forwarding), car la fonction
    do_other_stuff recoit un argument qui est exactement du même type que la
    valeur initialement passée à do_stuff.
    
    
    == Exercice ==
    
    Fichiers à récupérer
    *https://llr-training-git.in2p3.fr/chamont/CppNouvellesPratiques/raw/master/Exercices/coef4.cpp
    
    Dans notre code pour les exercices, on peut remarquer que la construction
    d'une instance de la classe Echec, quand on utilise lui donne un commentaire
    sous forme d'une chaine de caractères littérale de type <code>const char *</code>, perd
    son temps à faire une double copie : d'abord pour transformer le littéral en
    instance de std::string, puis en faisant une construction par copie. C'est
    un cas de figure évident pour tirer profit de la nouvelle notion de
    déplacement.
    #Dotez la classe Echec d'un constructeur et d'un opérateur d'affectation par déplacement.
    #Essayez d'observer le bénéfice de performance, en mesurant le temps nécessaire pour construire une instance d'Echec à partir d'une chaine de caractères littérale de type <code>const char *</code> d'une part (valeur temporaire), d'une instance de <code>std::string</code> d'autre part (valeur stable). Le code de <code>Boucle</code> montre commetn utiliser la classe standard <code>chrono</code>.
    #Essayer de remplacer toutes les utilisations de <code>std::string</code> dans le code par la classe <code>Texte</code> naive décrite ci-dessus. Quelle différence avec std::string au niveau des performances ?
    
    Solution
    *https://llr-training-git.in2p3.fr/chamont/CppNouvellesPratiques/raw/master/Exercices/coef5.cpp

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
