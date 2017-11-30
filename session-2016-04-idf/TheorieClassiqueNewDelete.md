# [Variables et tableaux sur le tas](TheorieClassique.md)

\_\_FORCETOC\_\_

En C++, on peut contrôler finement le cycle de vie des objets et gérer la mémoire qui leur est allouée. C'est ce qui permet de réaliser des applications plus performantes qu'avec d'autres langages, mais c'est aussi la source d'erreurs principale du langage. Pointeurs et gestion dynamique de la mémoire : '''attention danger \! '''

## Variables sur le tas

Les variables ordinaires de C++ ont une durée de vie limitée au bloc d'instructions courant, qu'il s'agisse de la fonction en cours, ou bien d'un bloc d'instructions attaché à une instruction `if`, `for`, ... La mémoire qui leur est allouée est située dans une zone appelée "pile", et récupérée automatiquement lorsque l'on sort du bloc en cours.

En C++, on peut également placer explicitement une variable dans une autre zone mémoire appelée "tas". Cela se fait se fait par un appel à l'opérateur **new**, qui réserve la mémoire et renvoie son adresse, pour que l'utilisateur puisse la stocker dans un pointeur.

Le tas est indépendant de la pile et la variable ainsi créée existe tant que l'on appelle pas explicitement l'opérateur **delete**. La création et la destruction de ce type de variable est à la charge du programmeur. Création et destruction peuvent se faire à des endroits très disjoints de votre programme, et le compilateur n'est pas apte à vérifier que vous faites les choses correctement.

``` cpp
int main()
 {
  //...
  int * pi = new int ;
  //...
  *pi = 1 ;
  //...
  std::cout << *pi << std::endl ;
  //...
  delete pi ;
  //...
  return 0
 } 
```

L'intérêt de créer et des variables sur le tas, c'est de pouvoir ensuite faire circuler leur adresse de fonction en fonction sans que cette variable soit détruite. On peut notamment créer une fonction qui a la charge de créer la variable et de l'initialiser correctement. Dans ce cas, par symétrie, on crée en général une autre fonction chargée de la destruction :

``` cpp
int * cree()
 {
  int * pi = new int ;
  //...
  return pi ;
 }

void affiche( int * pi )
 { std::cout << *pi << std::endl ; }          

void detruit( int * pi )
 {
  //...
  delete pi ;
 }

int main()
 {
  int * ii = cree() ;
  *ii = 1 ;
  affiche(ii) ;
  detruit(ii) ;
 } 
```

Une grande partie des erreurs d'exécution C++ sont des erreurs avec les appels à `new` et `delete`. Pour chaque à l'un, il faut exactement un appel à l'autre. Si on l'oublie, on a une fuite de mémoire. Si on détruit deux fois la même adresse, le système d'exploitation trouve cela suspect et interrompt le programme en cours. Pour éviter de se tromper, on évite les manoeuvre disymétriques telles que ci-dessous :

``` cpp
int * cree()
 { return (new int) ; }

void affiche( int * i )
 { std::cout << *i << std::endl ; }          

void detruit( int * i )
 { delete i ; }

int main()
 {
  int * i = cree() ;
  int * j = new int ;
  int * k = new int ;
  *i = 1 ;
  *j = 2 ;
  *k = 3 ;
  affiche(i) ;
  affiche(j) ;
  affiche(k) ;
  delete k ;
  detruit(j) ;
  detruit(i) ;
 } 
```

## Tableaux sur le tas

On peut créer un tableau sur le tas via l'opérateur **new**.

Il est impossible d'initialiser les éléments d'un tel tableau.

Il faut rappeler au compilateur qu'il s'agit d'un tableau lors de la destruction.

``` cpp
int * t = new int[10] ;
int i ;
for ( i=0, i<10 ; i++ )
 { t[i] = 0 ; }
//...
delete [] t
```

  
  
© *CNRS 2016*  
*Assemblé et rédigé par David Chamont, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
