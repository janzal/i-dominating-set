/**
 
 Úloha DOM: i-dominující množina grafu
 =====================================
 
    Vstupní data:

    G(V,E) = jednoduchý souvislý neorientovaný neohodnocený graf o n uzlech a m hranách
    n = přirozené číslo představující počet uzlů grafu G, n >= 5
    k = přirozené číslo řádu jednotek představující průměrný stupeň uzlu grafu G, n >= k >= 3
    i = přirozené číslo, 1 ⇐ i ⇐ 6
    Doporučení pro algoritmus generování G:

    Použijte generátor grafu s volbou typu grafu „-t AD“, který vygeneruje souvislý neorientovaný neohodnocený graf.

    Definice:

    Je-li dáno přirozené číslo i >= 0 a uzel u grafu G, pak i-okolí uzlu u je množina všech uzlů G ve vzdálenosti nejvýše i od u, včetně uzlu u samotného. Pak i-dominující množina grafu G je každa množina uzlů takových, že sjednocení jejich i-okolí obsahuje všechny uzly G.

    Úkol:

    Nalezněte minimální i-dominující množinu W grafu G.

    Výstup algoritmu:

    Výpis uzlů nejmenší i-dominující množiny W.

    Sekvenční algoritmus:

    Řešení musí existovat. Pokud i >= poloměr grafu G, pak je řešením kterýkoli uzel v centru grafu, čili dolní mez na |W| je 1. Proto uvažujme pouze případ, že i < poloměr grafu, kdy není známa dolní mez. Pak se jedná o NP-těžkou úlohu a použijeme sekvenční algoritmus typu BB-DFS s hloubkou omezenou na horní mez. Přípustný mezistav je podmnožina uzlů W, která je i-dominující množinou. Cena, kterou minimalizujeme, je |W|.

    Těsná dolní mez je horní_celá_část(průměr(G)/(2i+1)).

    Holní mez je |V|/(2i+1).

    Paralelní algoritmus

    Paralelní algoritmus je typu L-PBB-DFS-D.

 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <stdlib.h>
#include <stdio.h>

using namespace std;



int main(int argc, char *argv[]) {
    return 0;
}
