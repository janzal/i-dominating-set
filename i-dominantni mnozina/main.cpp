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
    unsigned int size;
    unsigned int level;
    vector<vector<bool> > matrix;
    vector<set<unsigned int> > nbhMap;
};

class Solver {
public:
    static Solver* loadGraph(char *fileName, unsigned int level) {
        Solver *solver = new Solver();
        solver->graph->level = level;
        
        clog << "Nacitam graf: " << fileName << endl;
        
        string line;
        ifstream myfile (fileName);
        if (myfile.is_open())
        {
            getline(myfile, line);
            int graphSize = atoi(line.c_str());
            vector< vector<bool> > matrix(graphSize, vector<bool>(graphSize, false));
            vector< set<unsigned int> > neighbourhoodMap(graphSize);
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
            solver->graph->nbhMap = neighbourhoodMap;
            myfile.close();
            
            clog << "Graf nacten ze souboru: " << fileName << endl;
            clog << "Velikost grafu: " << solver->graph->size << endl;
            clog << "i = " << level << endl << endl;

            return solver;
        }
        return NULL;
    }
    
    void findSolution() {
        vector< vector<unsigned int> > stack(this->graph->size, vector<unsigned int>(1, 0));
        vector<unsigned int> result(graph->size + 1);
        
        //uvodni naplneni zasobniku
        for (unsigned int i = 0; i < graph->size; i++) {
            vector<unsigned int> partialResult(1, i);
            stack.push_back(partialResult);
        }
        cout << "Hledani zacalo...\n\n";
        //hlavni cyklus, prochazi zasobnik
        while (!stack.empty()) {
            vector<unsigned int> partialResult = stack.back();
            stack.pop_back();
            //otestuj castecne reseni
            if (partialResult.size() < result.size() && this->testPartialResult(partialResult)) {
                result = partialResult;
                this->printVector(partialResult,"Reseni nalezeno: ");
            } else if (partialResult.size() < (result.size() - 1)) {
                //expanduj castecne reseni tehdy ma-li to smysl
                this->expandStack(expandPartialResult(partialResult), stack);
                this->printVector(partialResult,"Zkousim: ");
            }
        }
        
        printVector(result,"\nJedno z nejlepsich reseni: ");
        cout << "Velikost nejlepsiho reseni: " << result.size() << "\n";

    }
    
private:
    
    /**
     * Funkce expanduje castecne reseni(vraci potomky tohoto reseni).
     * Nove castecne reseni se sklada z castecneho reseni,
     * ktere je doplnene o nove prvky, ktere puvodni neobsahovalo.
     *
     * @param partialResult Castecne reseni, ktere rozsirujeme o nove prvky
     * @param graph Definice grafu
     * @return Vraci nova castecna reseni
     */
    vector< vector<unsigned int> > expandPartialResult(vector<unsigned int> partialResult) {
        unsigned int max = 0;
        //najdi maximalni prvek v castecnem reseni
        for (unsigned int i = 0; i < partialResult.size(); i++) {
            if (partialResult[i] > max) {
                max = partialResult[i];
            }
        }
        //vysledek
        vector < vector<unsigned int> > expandedResults(this->graph->size - (max + 1));
        //dopln castecne reseni o nove prvky
        for (unsigned int i = max + 1; i < this->graph->size; i++) {
            //cout << "roziruji o:" << i << endl;
            vector<unsigned int> expandedPartialResult = partialResult;
            expandedPartialResult.push_back(i);
            expandedResults[i - (max + 1)] = expandedPartialResult;
        }
        
        return expandedResults;
    }

    /**
     * Rozsiri zasobnik o nova castecna reseni.
     *
     * @param expandedPartialResult Rozsirene castecne reseni
     * @param stack Reference na globalni zasobnik
     */
    void expandStack(vector< vector<unsigned int> > expandedPartialResult, vector< vector<unsigned int> >& stack) {
        for (unsigned int i = 0; i < expandedPartialResult.size(); i++) {
            stack.push_back(expandedPartialResult[i]);
        }
    }

    bool testPartialResult(vector<unsigned int> partialResult) {
        bool ok[this->graph->size];
        for (unsigned int i = 0; i < partialResult.size(); ++i) {
            if (this->graph->nbhMap[partialResult[i]].empty()) {
                set<unsigned int> neighbourhood;
                this->expandNode(&neighbourhood, partialResult[i], this->graph  ->level);
                this->graph->nbhMap[partialResult[i]] = neighbourhood;
            }
            set<unsigned int>::iterator theIterator;
            for(theIterator = graph->nbhMap[partialResult[i]].begin(); theIterator != graph->nbhMap[partialResult[i]].end(); theIterator++ ) {
                ok[*theIterator] = true;
            }
        }
        for (unsigned int k = 0; k < graph->size; ++k) {
            if (!ok[k]) return false;
        }
        return true;
    }
    
    void expandNode(set<unsigned int> *nbh, unsigned int node, unsigned int currentLevel) {
        if (currentLevel > 1) {
            for (unsigned int var = 0; var < this->graph->size; ++var) {
                if (this->graph->matrix[node][var])
                {
                    this->expandNode(nbh, var, currentLevel - 1);
                }
            }
        }
        nbh->insert(node);
    }
    
    void printVector(vector<unsigned int> print, string message) {
        clog << message << "(";
        for (unsigned int i = 0; i < print.size() - 1; i++) {
            clog << print[i] << ", ";
        }
        clog << print[print.size()-1] << ")" << endl;
    }

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
        
        unsigned int level = atoi(argv[argc - 1]);
        if (level > 0) {
            solver = Solver::loadGraph(fileName, level);
        } else {
            cerr << "i neni platne" << endl;
            return 1;
        }
        
        if (!solver) {
            cerr << "Graf neni validni" << endl;
            return 1;
        }
        
        solver->findSolution();
        return 0;
        
    }
    cout << "Pouziti:" << endl << "<graph_file> <i>" << endl;
    return 0;
}
