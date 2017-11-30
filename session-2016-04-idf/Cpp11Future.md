# [Récupérer les résultats d'un processus léger : future & promise](TheorieFonctionnelleConcurrente.md)

C++11 offre quelques moyens de transmettre des informations entre threads sans avoir à passer par des données partagées et un mécanisme de synchronisation manuel.

## Utilisation de la fonction `async`

Si l'on souhaite exécuter une fonction dans un autre thread, puis en obtenir le résultat ultérieurement, on peut utiliser \`std::async\`.

``` cpp
int f()
 { .....}
.....
std::future<int> res = std::async(std::launch::async, f) ;
.....
int n = res.get() ;
```

Avec ce mécanisme, si dans l'exemple ci-dessus la fonction \`f\` lance une exception, elle sera transmise à l'appelant au moment de la récupération du résultat par \`res.get()\`. \`async\` permet donc de lancer l'exécution d'une fonction en tâche de fond, pour ne récupérer le résultat que plus tard.

Le paramètre \`std::launch::async\` ci-dessus peut sembler redondant. Il s'agit en réalité d'un degré de liberté supplémentaire pour \`std::async\`, car async permet aussi de faire de l'évaluation paresseuse, c'est à dire de n'exécuter une fonction que lorsque l'on est sûr que son résultat est nécessaire, au moment de l'appel à \`res.get()\`. Dans ce cas, on utilisera \`std::launch::deferred\`.

Il est aussi possible de laisser le choix à l'implémentation d'utiliser une option ou l'autre avec \`std::launch::async | std::launch::deferred\`. C'est ce que fait std::async par défaut si ce premier paramètre n'est pas précisé. Cependant, il n'est pas recommandé de faire cela, puisque le comportement du programme devient alors moins prévisible quand on passe d'une implémentation de C++11 à une autre.

## Démarche plus générale

De façon plus générale, on peut organiser la récupération d'un résultat en connectant un objet de type `future` dans le code client, et un objet de type `promise` confié au processus fournisseur :

``` cpp
// fonction qui sera lancée par un processus léger
void f( std::promise<int> pi )
 {
  // ...
  pi.set_value(...) ;
 }

// dans le code lancant le thread, on trouvera :
std::promise<int> pi ;
std::future<int> fi = pi.get_future() ;
std::thread t(f,std::move(pi)) ;
// .....
int n = fi.get() ;
```

## Futur partagé

De même qu'une instance de `std::thread`, une instance de `std::future` est non copiable, et seulement déplaçable. Si on souhaite avoir plusieurs processus dans l'attente d'un même calcul asynchrone, on utilisera plutôt une instance de `std::shared_future`, qui est copiable. Chaque processus client disposera de sa propre copie du résultat.

  
  
© *CNRS 2015*  
*Assemblé et rédigé par David Chamont et Hadrien Grasland, cette œuvre est mise à disposition selon les termes de la [Licence Creative Commons - Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-nc-sa/4.0/)*
