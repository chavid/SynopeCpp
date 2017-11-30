# [Factoriser le code indépendant des paramètres](TheorieGenerique.md)

Les patrons constituent un outil puissant, mais en simplifiant et au automatisant le copier-coller, il peuvent favoriser une duplication invisible mais foisonnante de portions de code et de données, et conduire à des fichiers exécutables de très/trop grande taille. On parle alors de "code bloat".

Les programmeurs passent une grande partie de leur temps à repérer, à extraire et à factoriser du code commun : au sein des fonctions, au sein des classes, et à l'aide de patrons. La difficulté, dans le cas des patrons, est que la duplication de code est implicite et que le repérage du code à factoriser demande un peu d'entrainement. Prenons l'exemple d'un patron pour des matrices carrées de taille fixe, supportant l'inversion :

``` cpp
template< typename T, std::size_t n >
class SquareMatrix
 {
  public :
    .....
    void invert() ;
 } ;
```

Ce patron a deux paramètres, `T` et `n`, de nature très différente. `T` est un type, `n` est une valeur. On qualifie `n` de "paramètre expression". Ce dernier genre de paramètre est moins fréquemment utilisé que les types, mais ils sont parfaitement valides, et assez naturels dans le cas présent. Observons ce code :

``` cpp
SquareMatrix<double, 5> sm1 ;
...
sm1.invert() ;                  // call SquareMatrix<double, 5>::invert

SquareMatrix<double, 10> sm2 ;
...
sm2.invert();                  // call SquareMatrix<double, 10>::invert
```

Deux copies de la méthode `invert` seront générées ici, identique à ceci près que la première travaille sur une matrice 5x5 et la seconde sur une matrice 10x10. C'est un cas typique de gonflement de code induit par les patrons. En présence de deux codes de ce type, vous créeriez une fonction commune, prenant en argument la taille de la matrice ? C'est exactement ce qu'il faut faire :

``` cpp
template<typename T>
class SquareMatrixBase
 {
  protected :
    .....
    void invert( std::size_t matrixSize ) ;
    .....
 } ;

template< typename T, std::size_t n >
class SquareMatrix : private SquareMatrixBase<T>
 {
  private :
    using SquareMatrixBase<T>::invert ;
  public :
    .....
    void invert() { this->invert(n) ; } 
 } ;
```

La méthode `invert` prenant en argument la taille de matrice a été placée dans une classe de base, destinée à recevoir toutes les méthodes partagées. Ces dernières sont protégées, car uniquement utilisées dans les classes dérivées, et l'héritage est privé. L'appel supplémentaire de la méthode `invert` de la classe de base ne devrait rien couter, car implicitement en ligne. On notera également l'utilisation simultanée de `using`, pour que la méthode `invert` de la classe dérivée ne cache pas celle de la classe de base, et de `this->`, pour accéder à la méthode `invert` de la classe de base paramétrée.

Nous avons laissé de côté une question délicate : comment transmettre les données à inverser ? On peut imaginer un argument supplémentaire à `SquareMatrixBase`<T>`::invert`, par exemple un pointeur vers un segment de mémoire contenant les eléments. Mais en présence de multiples fonctions à factoriser ainsi, qui auront toutes besoin d'accéder aux éléments de la matrice, on sent bien que ce n'est pas très satisfaisant. On pourrait placer notre pointeur dans la classe de base, et tant qu'à faire, y ajouter la taille :

``` cpp
template<typename T>
class SquareMatrixBase
 {
  protected :
    SquareMatrixBase( std::size_t n, T * pMem )
     : size(n), pData(pMem) {}
    void setDataPtr( T * ptr) { pData = ptr ; }
    .....
  private :
    std::size_t size ;
    T * pData ;
 } ;

template< typename T, std::size_t n >
class SquareMatrix: private SquareMatrixBase<T>
 {
  public :
    SquareMatrix()
     : SquareMatrixBase<T>(n, data) {}
    .....
  private :
   T data[n*n] ;
 } ;
```

On a ici nul besoin d'allocation dynamique, mais c'est aussi une possibilité :

``` cpp
template<typename T, std::size_t n>
class SquareMatrix: private SquareMatrixBase<T> {
public:
  SquareMatrix()                          // set base class data ptr to null,
  : SquareMatrixBase<T>(n, 0),            // allocate memory for matrix
    pData(new T[n*n])                     // values, save a ptr to the
  { this->setDataPtr(pData.get()); }      // memory, and give a copy of it
  ...                                     // to the base class

private:
  boost::scoped_array<T> pData;
};                             
```

A présent, la plupart si ce n'est toutes les méthodes de `SquareMatrix` sont de simples appels en ligne aux méthodes de quelques classes de base (une par type), indépendemment de leur taille. En même temps, nous avons de vrais types indépendants tels que `SquareMatrix<double, 5>` et `SquareMatrix<double, 10>`, et il est impossible de donner une matrice de taille 5 à une fonction qui attendrait une matrice de taille 10.

Il y a tout de même quelques couts cachés : par exemple, certaines optimisations du compilateur sont plus faciles sur le code initial ou les tailles sont des constantes, plutôt que sur le code final ou la taille est devenue un argument de fonction. Par contre la plus petite taille du code final peut améliorer la localité et les performances des caches. Quel effet va dominer ? Seule façon de le savoir : essayer les deux solutions avec votre code, votre matériel et vos données...

Il y a d'autres effets de bord à surveiller :

  - l'accroissement de taille des objets qui résulte de l'ajout de données dans la classe de base,
  - la perte d'encapsulation liée au pointeur vers les éléments de matrice,
  - les questionnements sur la bonne façon de libérer, ou pas, les données de pointeur.

On en vient à penser qu'un peu de duplication de code n'est toujours un si grand mal... à vous de doser.

Pour finir, notons que le gonflement de code vient aussi des paramètres de type. Par exemple, sur les plateformes ou `int` et `long` ont la même taille, des types tels que `vector`<int> et `vector`<long> sont parfaitement identiques. Tous les éditeurs de liens ne sont pas capables de détecter et de nettoyer automatiquement de tels doublons. De même, de nombreux patrons manipulant des pointeurs typés (e.g., `list<int*>`, `list<const int*>`, `list<SquareMatrix<long, 3>*>`, etc.) gagneraient à exploiter des méthodes partagées manipulant des pointeurs neutres (`void *`). Certaines implémentations de la librairie standard applique cette technique à des conteneurs tels que `vector`, `deque` et `list`. Si vous êtes concernés par le "code bloat", vous gagneriez à faire de même.

## A retenir

  - Les patrons génèrent de multiples classes et fonctions, et tout le code qui ne dépend pas des paramètres est source potentielle de gonglement.
  - Le gonflement lié aux paramètres expressions peut souvent être éliminé en remplacant les paramètres du patron par des arguments de fonction ou des variables membres.
  - Le gonflement lié aux paramètres de type peut être réduit en partageant les implémentations pour les types qui ont la même représentation binaire.

## Exercice

#### Fichiers à récupérer

  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef9.cpp>
  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef9.ref>

#### Enoncé

Factoriser dans une classe de base \!CoefImpl tout ce qui peut l'être de Coef<U>.

#### Solution

  - <https://llr-training-git.in2p3.fr/chamont/CppGenerique/raw/master/Exercices/coef10.cpp>
