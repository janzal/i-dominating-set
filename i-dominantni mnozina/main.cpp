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

struct Graph {

    // pocet uzlu v grafu
    unsigned int size;

    // i-hodnota
    unsigned int iVal;

    // 
    vector<set<unsigned int> > coveredArea;

    // matice incidence
    vector<vector<bool> > matrix;
};

class Solver {
public:
    static Solver* loadGraph(char *fileName, unsigned int iVal) {
        Solver *solver = new Solver();
        solver->graph->iVal = iVal;
        
        clog << "Nacitam graf: " << fileName << endl;
        
        string line;
        ifstream myfile (fileName);
        if (myfile.is_open())
        {
            getline(myfile, line);
            int graphSize = atoi(line.c_str());
            vector< vector<bool> > matrix(graphSize, vector<bool>(graphSize, false));
            int i = 0;
            for (int var = 0; var < graphSize; ++var)
            {
                getline (myfile,line);
                for(int j = 0; j < graphSize; ++j) {
                    if(line[j] == '1')	{
                        matrix[i][j] = true;
                    }
                }
                i++;
            }
            solver->graph->size = graphSize;
            solver->graph->matrix = matrix;

            vector< set<unsigned int> > coveredArea(graphSize);
            solver->graph->coveredArea = coveredArea;
            myfile.close();
            
            clog << "Soubor: " << fileName << endl;
            clog << "Graph size: " << solver->graph->size << endl;
            clog << "i-value: " << iVal << endl << endl;

            return solver;
        }
        return NULL;
    }
    
    void findSolution() {
        this->stack = vector< vector<unsigned int> >(this->graph->size, vector<unsigned int>(1, 0));
        vector<unsigned int> result(graph->size + 1);
        
        //uvodni naplneni zasobniku
        for (unsigned int i = 0; i < graph->size; i++) {
            vector<unsigned int> partialResult(1, i);
            this->stack.push_back(partialResult);
        }
        cout << "Hledani zacalo...\n\n";
        //hlavni cyklus, prochazi zasobnik
        while (!this->stack.empty()) {
            vector<unsigned int> nodes = this->stack.back();
            this->stack.pop_back();
            //otestuj castecne reseni
            if (nodes.size() < result.size() && this->isGraphCoveredBy(nodes)) {
                // ok mame reseni
                result = nodes;
                this->printVector(nodes,"Reseni nalezeno: ");
            } else if (nodes.size() < (result.size() - 1)) {
                //expanduj castecne reseni tehdy ma-li to smysl
                this->expandStack(expandPartialResult(nodes));
                this->printVector(nodes,"res: ");
            }
        }
        
        printVector(result,"\nJedno z nejlepsich reseni: ");
        cout << "Velikost nejlepsiho reseni: " << result.size() << "\n";

    }
    
private:

    vector< vector<unsigned int> > stack;
    

    vector< vector<unsigned int> > expandPartialResult(vector<unsigned int> nodes) {
        unsigned int max = 0;
        //najdi uzel s maximalnim id v nodes
        for (unsigned int i = 0; i < nodes.size(); i++) {
            if (nodes[i] > max) {
                max = nodes[i];
            }
        }

        //vysledek ktery vratime
        vector < vector<unsigned int> > result(this->graph->size - (max + 1));

        //dopln castecne reseni o nove prvky
        for (unsigned int i = max + 1; i < this->graph->size; i++) {
            // urcite to bude pokryvat minimalne vstupni nodes
            vector<unsigned int> tmp = nodes;

            // postupne to tam bude vkladat nody od max do velikosti grafu
            tmp.push_back(i);

            // pridej celej vector do vectoru vystupnich vektoru
            result[i - (max + 1)] = tmp;
        }
        
        return result;
    }


    bool isGraphCoveredBy(vector<unsigned int> nodes) {

        // pokud bude na konci pres vsechny prvky true, pak nodes jsou resenim problemu
        bool isset[this->graph->size];
        
        // projdi vsechny uzly v nodes
        for (unsigned int i = 0; i < nodes.size(); i++) {

            // pokud tento uzel jeste nema urceno co pokryva, zjisti co pokryva
            if (this->graph->coveredArea[nodes[i]].empty()) {
                // inicilizace mnoziny
                set<unsigned int> coveredArea;
                // expanduj uzel a tim napln mnozinu
                this->expandNode(&coveredArea, nodes[i], this->graph  ->iVal);
                // uloz tuto mnozinu pokryti, at ji znova nemusime pocitat
                this->graph->coveredArea[nodes[i]] = coveredArea;
            }
            set<unsigned int>::iterator it;

            // projdi celou mnozinu pokryti
            for(it = graph->coveredArea[nodes[i]].begin();
                it != graph->coveredArea[nodes[i]].end();
                it++ ) {
                // a nastav true do kontrolniho pole
                isset[*it] = true;
            }
        }
        
        // projdi cele kontrolni pole a zjisti, zda nodes pokryvaji reseni
        for (unsigned int k = 0; k < graph->size; ++k) {
            if (!isset[k]) return false;
        }

        return true;
    }

    /*
        Vlozime do stacku dalsi mozne reseni
    */
    void expandStack(vector< vector<unsigned int> > expandedPartialResult) {

        clog << "  Exanding stack from:" << endl;
        this->printStack("    ");
        for (unsigned int i = 0; i < expandedPartialResult.size(); i++) {
            this->stack.push_back(expandedPartialResult[i]);
        }

        clog << "  To:" << endl;
        this->printStack("    ");
    }

    void printStack(string prefix){
        clog << prefix <<"Stack:" << endl;

        vector< vector<unsigned int> >::iterator it;
        for(it = this->stack.begin();
            it != this->stack.end();
            it++ ) {
            
            printVector(*it, "      ");
        }
    }
    
    /*
        Metoda, ktera do nbh vlozi vsechny uzly, ktere jsou v currentVal vzdalenosti od uzlu node
    */
    void expandNode(set<unsigned int> *nbh, unsigned int node, unsigned int currentVal) {

        if (currentVal > 1) {
            // zavolej znova expandNode na vsechny sousedici uzly s aktualnim uzlem
            unsigned int i = 0;
            while ( i < this->graph->size){
                // sousedi spolu? pokud ano -> pokracuj sousedem
                if (this->graph->matrix[node][i])
                {
                    // sniz uroven dominance o 1
                    this->expandNode(nbh, i, currentVal - 1);
                }
                i++;
            }
        }

        // pridej node do mnoziny sousedu
        nbh->insert(node);
    }
    
    // vytiskne vektor
    void printVector(vector<unsigned int> print, string message) {
        clog << message << "";
        for (unsigned int i = 0; i < print.size() - 1; i++) {
            clog << print[i] << ", ";
        }
        clog << print[print.size()-1] << "" << endl;
    }


    // privatni konstruktor
    Solver() {
        this->graph = new Graph();
    }
    
    Graph *graph = NULL;
};


int main(int argc, char *argv[]) {
    cout << "Úloha DOM: i-dominující množina grafu" << endl;
    cout << "-------------------------------------" << endl << endl;
    
    
    if (argc >= 3 &&  argc <= 4)
    {  
        Solver *solver = NULL;
        
        char* fileName = argv[argc - 2];
        
        unsigned int iVal = atoi(argv[argc - 1]);

        if (iVal > 0) {
            solver = Solver::loadGraph(fileName, iVal);
        } else {
            cerr << "i neni platne, musi to byt cele cislo > 0" << endl;
            return 1;
        }
        
        if (!solver) {
            cerr << "Graf neni validni, zkontrolujte format souboru" << endl;
            return 1;
        }
        
        // zahaj vypocet
        solver->findSolution();
        return 0;
        
    }
    cout << "Pouziti:" << endl << "<graph_file> <i>" << endl;
    return 0;
}
