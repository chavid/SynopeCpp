# [Les fonctions lambdas](TheorieFonctionnelleConcurrente.md)

Les fonctions lambda (ou anonymes), introduites en C++11, peuvent être définies au vol n'importe où dans le code, là ou l'on en a besoin. Cela permet de se passer d'une inutile déclaration préalable de la fonction, relativement inutile quand la fonction ne doit servir qu'une fois. Par ailleurs, la mécanique de capture des variables locales permet d'éviter la lourdeur d'un foncteur (objet appelable à travers l'opérateur ()). Dans les cas simples, le code produit est plus concis et plus clair. Si on pousse le mécanisme dans ses retranchements avec des fonctions lambda très compliquées, c'est moins évident... à vous de juger.

## Motivation : la définition au vol de fonctions simples

Le C++ permet d'utiliser une fonction comme argument d'une autre fonction. Parfois, cette fonction est triviale, ne va servir qu'une fois, et il est pénible et "encombrant" de devoir la déclarer comme n'importe quelle autre.

``` cpp
#include <iostream>
#include <vector>
#include <list>

template< typename Collection >
void applique( Collection & col, void (*f)( typename Collection::value_type ) )
 {
  auto itr = col.begin() ;
  while (itr!=col.end())
   { f(*itr) ; ++itr ; }
 }

void affiche( int i )
 {
  std::cout<<i<<std::endl ;
 }

int main()
 {
  std::vector<int> v {1,2,3,4,5} ;
  applique(v,affiche) ;
  std::list<int> l {1,2,3,4,5} ;
  applique(l,affiche) ;
 }
```

C++11 offre la possibilité d'écrire directement la fonction là ou elle est appelée, en remplacant son nom par "\[\]", ce qui permet d'alléger le code et de le rendre plus expressif, lorsque cette définition est brève. Ces fonctions sans nom sont appelées **fonctions lambdas**.

``` cpp
#include <iostream>
#include <vector>

template< typename Collection >
void applique( Collection & col, void (*f)( typename Collection::value_type ) )
 {
  auto itr = col.begin() ;
  while (itr!=col.end())
   { f(*itr) ; ++itr ; }
 }

int main()
 {
  std::vector<int> v{1,2,3,4,5} ;
  applique(v,[](int i){ std::cout<<i<<std::endl ; }) ;
 }
```

## Retourner un résultat

Si votre fonction lambbda doit retourner un résultat, c'est à peu prêt aussi facile. Si le corps de la fonction est fait d'une seule instruction return, le type de retour de la fonction est déduit de celui de l'expression retournée.

``` cpp
std::condition_variable cond;
bool data_ready;
std::mutex m;

void wait_for_data()
{
  std::unique_lock<std::mutex> lk(m);
  cond.wait(lk,[]{return data_ready;}); // (1)
}
```

Ci-dessus, le type de retour de la lambda est celui de la variable data\_ready, donc bool. Si jamais le corps de la lambda est plus complexe qu'une instruction return, il faut préciser explicitement ce type de retour, à l'aide d'une flèche (-\>) suivie du type, placée entre la spécification des arguments et le corps de fonction. En reprenant l'exemple ci-dessus :

``` cpp
cond.wait(lk,[]()->bool{return data_ready;});
```

Et puisque le type de retour est maintenant explicite, on peut complexifier à loisir le corps de la fonction :

``` cpp
cond.wait(lk,[]()->bool
{
  if(data_ready)
  {
    std::cout<<”Data ready”<<std::endl;
    return true;
  }
  else
  {
    std::cout<<”Data not ready, resuming wait”<<std::endl;
    return false;
  }
});
```

Ces premiers exemples montrent déjà des utilisations pratiques des lambdas. Mais là où elles deviennent réellement redoutables, c'est dans leur capacité à capturer des variables locales.

## Capturer une variable par valeur

On veut parfois doter une fonction de certains paramètres qui sont fixés lors de sa création, puis réutilisés, sans avoir à les redonner en argument à chaque appel. La façon "objet" de le faire consister à créer un objet-fonction, ou "foncteur", instance d'une classe dotée d'un opérator (), telle que la classe Mult ci-dessous.

``` cpp
#include <iostream>
#include <vector>
#include <functional>

template< typename Collection >
void applique( Collection & col, std::function<void(typename Collection::value_type)> f )
 {
  auto itr = col.begin() ;
  while (itr!=col.end())
   { f(*itr) ; ++itr ; }
 }

class Mult
 {
  public :
    Mult( int m ) : m_{m} {}
    void operator() ( int i ) { std::cout<<" "<<m_*i ; }
  private :
    int m_ ;
 } ;

template< typename Collection >
int f( Collection & col, int m )
 {
  std::cout<<"*"<<m<<" :" ;
  applique(col,Mult(m)) ;
  std::cout<<std::endl ;
 }

int main()
 {
  std::vector<int> v{1,2,3,4,5} ;
  f(v,1) ;
  f(v,2) ;
  f(v,3) ;
 }
```

Une fonction lambda introduite par "\[\]" n'a accès qu'à ses propres arguments, où aux variables globales, comme n'importe quelle fonction. Mais ces crochets permettent aussi de "capturer" la valeur d'une variable disponible là ou notre lambda est créée. Dans notre exemple précédent, on peut ainsi capturer `m` et se passer facilement du foncteur :

``` cpp
...
template< typename Collection >
int f( Collection & col, int m )
 {
  std::cout<<"*"<<m<<" :" ;
  applique(col, [m](int i){ std::cout<<" "<<m*i ; } ) ;
  std::cout<<std::endl ;
 }
...
```

D'un point de vue interne, vous pouvez imaginez qu'une classe équivalente à notre ancienne `Mult` a été créée au vol.

Ci-dessous, pour démontrer que la variable capturée est bien copiée, on stocke une lambda dans une enveloppe de fonction `std::function<>`, afin de pouvoir la réappeler après avoir modifier la variable originale, et on constatera que notre lambda renvoit toujours le même résultat, indépendamment de la valeur courante de m.

``` cpp
#include <functional>

int main()
 {
  int i{1} ;
  std::function<void(void)> f = [i](){ std::cout<<i<<std::endl ; } ;
  f() ;
  i = 2 ;
  f() ;
  i = 3 ;
  f() ;
 }
```

## Capture de variable par référence

La capture par valeur est la plus sûre, parce que les variables sont copiées, mais on peut également capturer par référence, en ajoutant yb "&". Dans ce cas, comme pour n'importe quelle référence, le comportement est indéfini si jamais la variable originale vient à disparaitre avant la fonction lambda. Dans notre exemple précédent, une capture par référence permet d'obtenir une lambda qui prend en compte la valeur courante de m :

``` cpp
#include <functional>

int main()
 {
  int i{1} ;
  auto f = [&i](){ std::cout<<i<<std::endl ; } ;
  f() ;
  i = 2 ;
  f() ;
  i = 3 ;
  f() ;
 }
```

## Captures multiples

Vous n'êtes pas limité à capturer une seule variable. Les crochets peuvent inclure autant de variables que vous le désirez, séparées par des virgules, en choississant individuellement pour chaque variable si elle doit être capturée par valeur ou par référence.

``` cpp
#include <functional>

int main()
 {
  int i{100}, j{10}, k{1} ;
  auto f = [i,&j,k](){ std::cout<<(i+j+k)<<std::endl ; } ;
  f() ;
  i = 200, j = 20, k = 2 ;
  f() ;
  i = 300, j = 30, k = 2 ;
  f() ;
 }
```

Si on raisonne en terme d'objets-fonctions, comme nous le faisions plus tôt avec notre classe `Mult`, on peut imaginer une classe qui capture i, j et k, ce qu'enterme savant on appelera une fermeture ou une clôture (closure) :

``` cpp
class Cloture
 {
  public :
    Cloture( int i, int & j, int k ) : i_{i}, j_{j}, k_{k} {}
    void operator() () { std::cout<<(i_+j_+k_)<<std::endl ; }
  private :
    int i_, & j_, k_ ;
 } ;
```

Vous pouvez également décider de capturer tout ce qui peut l'être sans discernement, à l'aide de la notation "\[=\]" pour le faire par valeur, ou "\[&\]" pour le faire par référence.

Enfin, vous pouvez utilisez cette dernière notation comme capture par défaut, et spécifier explicitement le mode de capture de certaines variables échappant au mode par défaut.

``` cpp
#include <functional>

int main()
 {
  int i{100}, j{10}, k{1} ;
  auto f = [=,&j](){ std::cout<<(i+j+k)<<std::endl ; } ;
  f() ;
  i = 200, j = 20, k = 2 ;
  f() ;
  i = 300, j = 30, k = 2 ;
  f() ;
 }
```

## Au sein d'une fonction membre

Pour accéder aux variables membres de l'objet courant, il faut capturer this.

``` cpp
#include <iostream>
#include <functional>
#include <vector>

template< typename Collection >
void applique( Collection & col, std::function<void(typename Collection::value_type)> f )
 {
  auto itr = col.begin() ;
  while (itr!=col.end())
   { f(*itr) ; ++itr ; }
 }

class Mult
 {
  public :
    Mult( int m ) : m_{m} {}
    template< typename Collection >
    void parcourt( Collection & col )
     {
      std::cout<<"*"<<m_<<" :" ;
      applique(col,[this]( int i ){ std::cout<<" "<<m_*i ; }) ;
      std::cout<<std::endl ;
     }
  private :
    int m_ ;
 } ;

int main()
 {
  std::vector<int> v{1,2,3,4,5} ;
  Mult mult(3) ;
  mult.parcourt(v) ;
 }
```

En programmation concurrente, les lambdas sont particulièrement utiles pour servir de prédicats dans `condition_variable::wait()`, avec`std::packaged_task<>` ou pour regrouper des petites tâches dans un "thread pool". Elles peuvent aussi être confiées à un constructeur de `std::thread` ou à un algorithme parallèle tel que `parallel_for_each()`.

## Lambdas polymorphes

Revenons à notre exemple initial, et imaginons qu'on cherche à traiter des collections de nature différente :

``` cpp
#include <iostream>
#include <vector>

template< typename Collection >
void applique( Collection & col, void (*f)( typename Collection::value_type ) )
 {
  auto itr = col.begin() ;
  while (itr!=col.end())
   { f(*itr) ; ++itr ; }
 }

int main()
 {
  std::vector<int> vi{1,2,3,4,5} ;
  applique(vi,[](int i){ std::cout<<i<<std::endl ; }) ;

  std::vector<double> vd{1.1,2.2,3.3,4.4} ;
  applique(vd,[](double i){ std::cout<<i<<std::endl ; }) ;

  ...
 }
```

On aurait naturellement envie de créer un patron de lambda, ou quelque chose qui y ressemble et qui apporte une forme de polymorphisme, plutot que de redéclarer notre lambda à chaque fois.

C++14 répond à cette envie, en nous permettant de remplacer les types de notre choix par `auto` :

``` cpp
...
int main()
 {
  auto f = [](auto i)
   { std::cout<<i<<std::endl ; } ;
 
  std::vector<int> vi{1,2,3,4,5} ;
  applique(vi,f) ;

  std::vector<double> vd{1.1,2.2,3.3,4.4} ;
  applique(vd,f) ;

  ...
 }
```

Si l'on recherche la clôture équivalente, cela revient à transformer en patron l'opérateur (), et notre objet `f` serait une instance de :

``` cpp
class Cloture
 {
  public :
    template<typename Value>
    void operator() (Value v)
     { std::cout<<v<<std::endl ; }
 } ;
```

Modifiez `lbd_complexe.cpp` pour utiliser un maximum de lambdas.

</pre>

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
