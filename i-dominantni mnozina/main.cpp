#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <set>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string.h>

#define DEBUG 2
#define INFO 1
#define SILENT 0

using namespace std;

unsigned int debugLevel = INFO;

void Log (string msg, unsigned int dbg){
    
    if(dbg <= debugLevel)
        cout << msg << endl;
}

void Log (string msg){
    Log(msg, 0);
}

string Vector2String (vector<unsigned int> & vec){
    stringstream ss;

    ss << "<";
    for (int i = 0; i < vec.size(); i++)
        ss << vec[i] << ", ";
    ss << ">";

    return ss.str();
}

class Graph{
public:
    // pocet uzlu grafu
    unsigned int size;

    // level dominance
    unsigned int level;
    
    // matice sousednosti
    vector<vector<bool>> matrix;

    // mapa sousedu
    vector<set<unsigned int>> neighbourhoodMap;

    // konstruktor
    Graph(unsigned int level){
        this->level = level;
    }

    /*
        Metoda, ktera do nbh vlozi vsechny uzly, ktere jsou v level vzdalenosti od uzlu node
    */
    void exploreNode(set<unsigned int> * nbh, unsigned int node, unsigned int level){
        // hah, zapornej level nebereme
        if (level < 0 ) return;
        // projdi cely graf
        for (unsigned int i = 0; i < this->size; i++){
            // sousedi spolu
            if(this->matrix[node][i]){
                // jako novy uzel pouzij i, level sniz o jedna
                this->exploreNode(nbh, i, level - 1);
            }
        }
        // vloz soucasny uzel do nbh
        nbh->insert(node);
    }

    // Tests if set given as param covers whole graph
    bool isCoveredBy(vector<unsigned int> & set){
        bool ok[this->size];

        for (unsigned int i = 0; i < set.size(); i++){

            // no neighbourhood has been generated?
            if (this->neighbourhoodMap[set[i]].empty()){
                // init set 
                set<unsigned int> neighbourhood;

                // fill set with nodes (with distance <= this->level from set[i])
                this->exploreNode(&neighbourhood, set[i], this->level);

                // set new neighbourhood
                this->neighbourhoodMap = neighbourhood;
            }

            // TODO -> refactor
            set<unsigned int>::iterator it;

            for(it = this->neighbourhoodMap[set[i]].begin(); it = this->neighbourhoodMap[set[i]].end(); it++)
                ok[*it]=true;
        }

        // check coverage of the graph
        for (unsigned int i = 0; i < this->size; i++){
            // first fail -> whole graph isnt covered
            if (ok[i] == false) return false
        }
        return true;
    }
};



class Dominator {
private:
    unsigned int m_level;
    unsigned int m_debugLevel;
    vector<vector<unsigned int>> m_stack;
    vector<unsigned int> m_result;
    
    Graph * m_graph;

    void initStack(){
        // init stack
        // outer vector consists of vectors
        // inner vectors consists of vectors of size 1 and filled with zeroes
        Log("initializing stack...", DEBUG);
        this->m_stack = new vector<vector<unsigned int>>(graph.size, vector<unsigned int>(1, 0));

        // Fill stack with all nodes of the graph
        unsigned int i = 0;
        while (i < this->m_graph.size()){
            vector<unsigned int> singleNode(1,i);
            this->m_stack.push_back(singleNode);
            i++;
        }
        Log("\tdone", DEBUG);
    }

    void initResult() {
        Log("initializing result vector...", DEBUG);
        this->m_results = new vector<unsigned int>(this->m_graph.size() + 1);
        Log("\tdone", DEBUG);
    }

    void analyzeResult(vector<unsigned int> & result){
        if (result.size() < this->m_result.size() && this->coversGraph(result))
    }
public:
    Dominator(unsigned int level, Graph *graph){
        this->m_level = level;
        this->m_graph = graph;

        this->initStack();
        this->initResults();
    }
    
    void start(){
        Log("*** 666 > DOMINATOR HAS BEEN RELEASED < 666 ***", INFO);

        while ( this->m_stack.empty() == false){
            vector<unsigned int> currResult = this->m_stack.back();
            this->m_stack.pop_back();

            this->analyzeResult(currResult);
        }
    }
};


bool loadGraph(char fileName[], Graph *graph)
{
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
        graph->size = graphSize;
        graph->matrix = matrix;
        graph->neighbourhoodMap = neighbourhoodMap;
        myfile.close();
        return true;
    }
    return false;
}





int main(int argc, char *argv[]) {
    if(argc >= 3 && argc <= 4) {
        if(argc == 4){
            if (strcmp(argv[1], "-i") == 1){
                debugLevel = INFO;
            }else if (strcmp(argv[1], "-d") == 1) {
                debugLevel = DEBUG;
            }
        }
    }else{
        Log("[-i,-d] <input_file> <i>");
        return 1;
    }
    
    Log("Debug level: " + debugLevel);

    // get i (level)
    unsigned int level = atoi(argv[argc-1]);
    
    if (level < 0){
        Log("Invalid level, use value > 0.");
        return 1;
    }else{
        Log("Level (i-value) is set to: " + level, INFO);
    }
        
    
    Graph *graph = new Graph(level);

    char* fileName = argv[argc -2];
    
    if ( ! loadGraph(fileName, graph)) {
        Log("Invalid file!");
        return 1;
    }else{
        Log("File ok.", INFO);
    }
    
    
    Dominator dom(level, graph);
    dom.start();
}
