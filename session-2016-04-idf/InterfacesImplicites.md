# [Interfaces implicites et le polymorphisme de compilation](TheorieGenerique "wikilink")

Le monde de la programmation orientée-objet tourne autour d'interfaces explicites (classes abstraites) et d'un polymorphisme dynamique (au moment de l'exécution). Par exemple, étant donné cette classe :

``` cpp
class Widget
 {
  public:
    Widget() ;
    virtual ~Widget() ;
    virtual std::size_t size() const ;
    virtual void normalize() ;
    void swap( Widget & other ) ;
    ...
 } ;
```

et cette fonction :

``` cpp
void doProcessing( Widget & w )
 {
  if (w.size() > 10 && w != someNastyWidget)
   {
      Widget temp(w) ;
      temp.normalize() ;
      temp.swap(w) ;
   }
 }
```

on peut dire de `w` dans `doProcessing` :

  - Parce que `w` est de type `Widget`, `w` doit respecter l'interface de `Widget`. Les détails de cette interface sont regroupés dans la définition de classe, consultable dans le fichier d'entête. On parle d'interface explicite.
  - Parce que certaines méthodes de `w` sont virtuelles, leur appel dans `doProcessing` peut conduire à l'exécution de méthodes réelles différentes, selon la classe réelle de `w` au moment de l'exécution. On parle de polymorphisme dynamique.

Le monde de la programmation générique (patrons) est fondamentalement différent, et même si les interfaces explicites et le polymorphisme dynamique sont toujours là, ils laissent la vedette aux interfaces implicites et au polymorphisme statique (à la compilation). Transformons `doProcessing` en patron de fonction :

``` cpp
template<typename T>
void doProcessing( T & w )
{
  if (w.size() > 10 && w != someNastyWidget) {
     T temp(w);
     temp.normalize();
     temp.swap(w);
  }
}
```

Que peut-on dire à présent de `w` ?

  - Ce que le type de `w` doit respecter est maintenant indirectement défini par les opérations qui sont appliquées à `w` dans le corps de la fonction, sans quoi la compilation échoue. Dans cet exemple, il s'agit notamment des fonctions membres `size`, `normalize` et `swap` ; du constructeur de copie (pour créer temp), et du comparateur d'inégalité (pour la comparaison avec `someNastyWidget`). L'interface est définie implicitement par l'ensemble des expressions faisant intervenir `w`, qui doivent être valides.
  - Les appels à certaines fonctions portant sur `w`, telles que les opérateurs `>` et `!=`, peuvent impliquer l'instanciation de patrons. Cette instanciation intervient à la compilation, et parce que des paramètres de patrons différents peuvent conduire à l'appel de fonctions différentes, on parle de polymorphisme statique (i.e. à la compilation).

## Interfaces implicites

Une interface explicite de classe est constituée des fonctions membres et de leurs signatures (nom de la fonction, types des paramètres, type de retour, constance). Elle comprend également les fonctions générées implicitement par le compilateur, et elle peut comprendre des données si vous avez la mauvaise idée d'en placer dans la zone publique.

``` cpp
class Widget
 {
  public :
    Widget() ;
    virtual ~Widget() ;
    virtual std::size_t size() const ;
    virtual void normalize() ;
    void swap( Widget & other) ;
 } ;
```

Au contraire, l'interface implicite d'un patron n'est pas faite de signatures de fonctions, mais d'une liste d'expressions qui doivent être valides. Examinons à nouveau le début de `doProcessing` :

    template<typename T>
    void doProcessing( T & w )
     {
      if (w.size() > 10 && w != someNastyWidget) {
      ...

On pourrait croire que l'interface implicite de `T` (type de `w`) doit répondre aux contraintes suivantes :

  - fournir une méthode `size` qui retourne un entier,
  - fournir un operateur `!=` permettant de comparer deux `T` (en faisant l'hypothèse que `someNastyWidget` est de type `T`).

En réalité, aucun de ces contraintes n'est obligatoire :

  - `T` doit bien avoir une méthode `size`, mais elle peut très bien être héritée d'une classe de base, et elle n'a pas besoin de retourner un entier, mais un objet de type `X` que le compilateur est capable de transformer en objet de type `Y` qu'un opérateur `>` est capable de comparer à un `int`...
  - De même, il suffit de disposer d'un opérateur `!=` capable de comparer un `X` et un `Y`, du moment que le compilateur est capable de transformer implicitement `w` en `X` et `someNastyWidget` en `Y` (sans parler de la possibilité que soit redéfini et impose des types de retour exotiques sur les expressions à sa gauche et à sa droite).

## A retenir

  - Classes et patrons ont tous deux des notions d'interface et de polymorphisme.
  - Pour les classes, les interfaces sont explicites et centrées sur les signatures de fonctions. Le polymorphisme s'exprime à l'exécution, à travers les méthodes virtuelles.
  - Pour les patrons, les interfaces sont implicites et centrées sur des expressions valides. Le polymorphisme s'exprime à la compilation, à travers l'instanciation des templates et la résolution de la surcharge.
