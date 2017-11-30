# [Le titre général](README.md) \[\#license ©\]

\_\_FORCETOC\_\_

## Première section

#### Sous-section

  - [Bonnes pratiques C++03 : bases.](BpBases.md)
  - [Nouveautés C++11 : petites nouveautés pratiques.](Cpp11Sucre.md)

#### Mot réservé `machin`

  - [Nouveautés C++11 : inférence de types.](Cpp11Inference.md) \[\#license ©\]

## Section sur les exemples

Style extrait simple

    #include <iostream>
    int main()
     {
      std::cout<<"Hello world !"<<std::endl ;
      return 0 ;
     }

Style spécifique C++ (et une image à droite) : [Image(debuter-en-cpp-banner.v.png,right,width=71ppx)](Image\(debuter-en-cpp-banner.v.png,right,width=71ppx\).md)

``` cpp
#include <iostream>
int main()
 {
  std::cout<<"Hello world !"<<std::endl ;
  return 0 ;
 }
```

    #!html
    <br clear=right>

Du code et sa sortie dessous, limités en largeur

    #!div style="width:600px"
    
    <source lang="cpp">
    #include <iostream>
    int main()
     {
      std::cout<<"Hello world !"<<std::endl ;
      return 0 ;
     }
    </source>
    
    {{{
    #!html
    <pre  style="margin-left:22px;margin-right:22px;border: 1px solid lightgoldenrodyellow;padding:10px;background-color:lightyellow">
    
    
    
    Hello world !

</pre>

}}}

Sur toute la largeur, avec indentation :

``` cpp
#include <iostream>
int main()
 {
  std::cout<<"Hello world !"<<std::endl ;
  return 0 ;
 }
```

    #!html
    <pre style="margin-left:50px;border: 1px solid palegoldenrod;padding:10px;background-color:lightyellow">
    
    
    
    Hello world !

</pre>

Du code et sa sortie côte à côte :

    #!div style="float:left;width:48%"
    
    <source lang="cpp">
    #include <iostream>
    int main()
     {
      std::cout<<"Hello world !"<<std::endl ;
      return 0 ;
     }
    </source>

    #!div style="float:right;width:48%"
    
    {{{
    #!html
    <pre style="margin-right:20px;border: 1px solid palegoldenrod;padding:4px;background-color:lightyellow">
    
    
    
    Hello world !

</pre>

}}}

    #!html
    <br clear=right>

## [Sous-titre avec lien extérieur](http://www.numpy.org)

Bla bla bla

© *[David Chamont](http://llr.in2p3.fr/spip.php?page=view_person&personID=121), Laboratoire Leprince-Ringuet (LLR) / IN2P3 / CNRS, Ecole polytechnique / Université Paris-Saclay*  
© *Ce(tte) œuvre est mise à disposition selon les termes de la [Licence Creative Commons Attribution - Partage dans les Mêmes Conditions 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/)*
