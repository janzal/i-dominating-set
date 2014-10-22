/*
 ============================================================================
 Name        : ParallelDominatingSub.c
 Author      : Štěpán Ort a Michal Havlíček
 Version     : 0.1
 ============================================================================
 */

#include "mpi.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <set>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

unsigned int level;

struct Graph{
    /** @var pocet uzlu grafu */
    unsigned int size;
    /** @var matice sousednosti */
    vector< vector<bool> > matrix;
    vector< set<unsigned int> > neighbourhoodMap;
};

/**
 * Funkce expanduje castecne reseni(vraci potomky tohoto reseni).
 * Nove castecne reseni se sklada z castecneho reseni,
 * ktere je doplnene o nove prvky, ktere puvodni neobsahovalo.
 *
 * @param partialResult Castecne reseni, ktere rozsirujeme o nove prvky
 * @param graph Definice grafu
 * @return Vraci nova castecna reseni
 */
vector< vector<unsigned int> > expandPartialResult(vector<unsigned int> partialResult, Graph *graph) {
    unsigned int max = 0;
    //najdi maximalni prvek v castecnem reseni
    for (unsigned int i = 0; i < partialResult.size(); i++) {
        if (partialResult[i] > max) {
            max = partialResult[i];
        }
    }
    //vysledek
    vector < vector<unsigned int> > expandedResults(graph->size - (max + 1));
    //dopln castecne reseni o nove prvky
    for (unsigned int i = max + 1; i < graph->size; i++) {
        //cout << "roziruji o:" << i << endl;

        // zkopiruj partialResult (param)
        vector<unsigned int> expandedPartialResult = partialResult;

        // pridaj novej uzel do mozneho reseni
        expandedPartialResult.push_back(i);

        // pridej vektor uzlu moznych reseni do vektoru expandedResults
        expandedResults[i - (max + 1)] = expandedPartialResult;
    }

    // vrati to vsechny mozny rozsireni castecnyho reseni o dalsi uzly
    return expandedResults;
}

/**
 * Rozsiri zasobnik o nova castecna reseni.
 *
 * @param expandedPartialResult Rozsirene castecne reseni
 * @param stack Reference na globalni zasobnik
 */
void expandStack(vector< vector<unsigned int> > expandedPartialResult, vector< vector<unsigned int> >& stack) {
    // prida nova reseni na stack
    for (unsigned int i = 0; i < expandedPartialResult.size(); i++) {
        stack.push_back(expandedPartialResult[i]);
    }
}

void exploreNeighbourhood(set<unsigned int> *neighbourhood, Graph *graph, unsigned int node, unsigned int currentLevel) {
﻿  if (currentLevel > 1) {
﻿  ﻿  for (unsigned int var = 0; var < graph->size; ++var) {
﻿  ﻿  ﻿  if (graph->matrix[node][var])
﻿  ﻿  ﻿  {
﻿  ﻿  ﻿  ﻿  exploreNeighbourhood(neighbourhood, graph, var, currentLevel - 1);
﻿  ﻿  ﻿  }
﻿  ﻿  }
﻿  }
﻿  neighbourhood->insert(node);
}

// ostestuje cast grafu (treba 2 uzly) na to, zda je to reseni nebo neni 
bool testPartialResult(vector<unsigned int> partialResult, Graph *graph) {
﻿  bool ok[graph->size];
﻿  for (unsigned int i = 0; i < partialResult.size(); ++i) {
﻿  ﻿  if (graph->neighbourhoodMap[partialResult[i]].empty()) {

      // init
﻿  ﻿  ﻿  set<unsigned int> neighbourhood;

      // rozbali uzly do neighbourhoodu, nigga
﻿  ﻿  ﻿  exploreNeighbourhood(&neighbourhood, graph, partialResult[i], level);

      // pro node i je okoli takovyhle
﻿  ﻿  ﻿  graph->neighbourhoodMap[partialResult[i]] = neighbourhood;
﻿  ﻿  }
﻿  ﻿  set<unsigned int>::iterator theIterator;

    // projde vsechny fellas ze sousedstvi
﻿  ﻿  for( theIterator = graph->neighbourhoodMap[partialResult[i]].begin(); theIterator != graph->neighbourhoodMap[partialResult[i]].end(); theIterator++ ) {
﻿  ﻿  ﻿  // nastav jim true ty kurvo
      ok[*theIterator] = true;
﻿  ﻿  }
﻿  }
  // a vsechny musi bejt true, jinak to neni reseni
﻿  for (unsigned int k = 0; k < graph->size; ++k) {
﻿  ﻿  if (!ok[k]) return false;
﻿  }
﻿  return true;
}

void printVector(vector<unsigned int> print, string message) {
    cout << message << "(";
﻿  for (unsigned int i = 0; i < print.size() - 1; i++) {
        cout << print[i] << ", ";
    }
    cout << print[print.size()-1] << ")\n";
}

bool loadGraph(char fileName[], Graph *graph)
{
﻿  string line;
﻿  ifstream myfile (fileName);
﻿  if (myfile.is_open())
﻿  {
﻿  ﻿  getline(myfile, line);
    
    // pocet radek
﻿  ﻿  int graphSize = atoi(line.c_str());
    
    // vyplni matici falsem
﻿  ﻿  vector< vector<bool> > matrix(graphSize, vector<bool>(graphSize, false));
    
    // init
﻿  ﻿  vector< set<unsigned int> > neighbourhoodMap(graphSize);

﻿  ﻿  int i = 0;
﻿  ﻿  for (int var = 0; var < graphSize; ++var)
﻿  ﻿  {
﻿  ﻿  ﻿  getline (myfile,line);
﻿  ﻿  ﻿  for(int j = 0; j < graphSize; ++j) {
﻿  ﻿  ﻿  ﻿  if(line[j] == '1')﻿  {
﻿  ﻿  ﻿  ﻿  ﻿  matrix[i][j] = true;
﻿  ﻿  ﻿  ﻿  }
﻿  ﻿  ﻿  }
﻿  ﻿  ﻿  i++;
﻿  ﻿  }
﻿  ﻿  graph->size = graphSize;
﻿  ﻿  graph->matrix = matrix;
﻿  ﻿  graph->neighbourhoodMap = neighbourhoodMap;
﻿  ﻿  myfile.close();
﻿  ﻿  return true;
﻿  }
﻿  return false;
}

int main(int argc, char *argv[]) {
﻿  if (argc >= 3 &&  argc <= 4)
﻿  ﻿  {
﻿  ﻿  ﻿  bool verbose = false;
﻿  ﻿  ﻿  if (argc == 4) {
﻿  ﻿  ﻿  ﻿  if (0 == strcmp(argv[1], "-v")) {
﻿  ﻿  ﻿  ﻿  ﻿  verbose = true;
﻿  ﻿  ﻿  ﻿  }
﻿  ﻿  ﻿  ﻿  else {
﻿  ﻿  ﻿  ﻿  ﻿  cout << "[-v] <graph_file> <i>\n";
﻿  ﻿  ﻿  ﻿  ﻿  return -1;
﻿  ﻿  ﻿  ﻿  }
﻿  ﻿  ﻿  }
﻿  ﻿  ﻿  Graph graph;
﻿  ﻿  ﻿  char* fileName = argv[argc - 2];
﻿  ﻿  ﻿  if (!loadGraph(fileName, &graph)) {
﻿  ﻿  ﻿  ﻿  std::cout << "Graph file is not valid!\n";
﻿  ﻿  ﻿  ﻿  exit(-1);
﻿  ﻿  ﻿  }

      // i - dominating bla -> i == level
﻿  ﻿  ﻿  unsigned int l = atoi(argv[argc - 1]);
﻿  ﻿  ﻿  if (l > 0) {
﻿  ﻿  ﻿  ﻿  level = l;
﻿  ﻿  ﻿  } else {
﻿  ﻿  ﻿  ﻿  std::cout << "i-value is not valid!\n";
﻿  ﻿  ﻿  ﻿  exit(-1);
﻿  ﻿  ﻿  }



﻿  ﻿  ﻿  cout << "Graph loaded from file: " << fileName << "\n";
﻿  ﻿  ﻿  cout << "Graph size: " << graph.size << "\n";
﻿  ﻿  ﻿  cout << "i = " << level << "\n\n";

      // stack -> velikost: graph.size, hodnota [ 0 ], kde [] je vektor :D
﻿  ﻿  ﻿  vector< vector<unsigned int> > stack(graph.size, vector<unsigned int>(1, 0));

      // [] vysledku
﻿  ﻿  ﻿  vector<unsigned int> result(graph.size + 1);

﻿  ﻿  ﻿  //uvodni naplneni zasobniku
﻿  ﻿  ﻿  for (unsigned int i = 0; i < graph.size; i++) {

        // [] castecnych vysledku, obsahuje vsechny uzly
﻿  ﻿  ﻿  ﻿  vector<unsigned int> partialResult(1, i);

        // pushne uzel na zasobnik (vsechny postupne)
﻿  ﻿  ﻿  ﻿  stack.push_back(partialResult);
﻿  ﻿  ﻿  }

      // vypis <3
﻿  ﻿  ﻿  cout << "Search started...\n\n";

﻿  ﻿  ﻿  //hlavni cyklus, prochazi zasobnik
﻿  ﻿  ﻿  while (!stack.empty()) {

﻿  ﻿  ﻿  ﻿  vector<unsigned int> partialResult = stack.back();
﻿  ﻿  ﻿  ﻿  stack.pop_back();

﻿  ﻿  ﻿  ﻿  //otestuj castecne reseni
﻿  ﻿  ﻿  ﻿  if (partialResult.size() < result.size() && testPartialResult(partialResult, &graph)) {
          // testPartialResult rika -> ok pro tenhle graf je to reseni
          // partialResult.size() < result.size() -> tohle reseni potrebuje min uzlu nez predesly
﻿  ﻿  ﻿  ﻿  ﻿  result = partialResult;
﻿  ﻿  ﻿  ﻿  ﻿  printVector(partialResult,"Solution found : ");

﻿  ﻿  ﻿  ﻿  } else if (partialResult.size() < (result.size() - 1)) {
          // tak reseni to neni -> rozsirime partialResult o dalsi uzly
﻿  ﻿  ﻿  ﻿  ﻿  //expanduj castecne reseni tehdy ma-li to smysl
﻿  ﻿  ﻿  ﻿  ﻿  expandStack(expandPartialResult(partialResult, &graph), stack);
﻿  ﻿  ﻿  ﻿  ﻿  if (verbose) {
﻿  ﻿  ﻿  ﻿  ﻿  ﻿  printVector(partialResult,"Trying: ");
﻿  ﻿  ﻿  ﻿  ﻿  }
﻿  ﻿  ﻿  ﻿  }
﻿  ﻿  ﻿  }

﻿  ﻿  ﻿  printVector(result,"\nOne of the best solutions: ");
﻿  ﻿  ﻿  cout << "Size of best solution: " << result.size() << "\n";
﻿  ﻿  ﻿  return 0;

﻿  ﻿  }
﻿  ﻿  cout << "[-v] <graph_file> <i>\n";
﻿  ﻿  return -1;
}

