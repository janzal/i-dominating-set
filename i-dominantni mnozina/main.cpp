#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <set>
#include <stdlib.h>
#include <stdio.h>

#define DEBUG 2
#define INFO 1
#define SILENT 0

using namespace std;

unsigned int debugLevel = INFO;

void Log (string & msg, unsigned int dbg){
    
    if(dbg <= debugLevel)
        cout << setfill(' ') << setw(dbg) << msg << endl;
}

void Log (string & msg){
    Log(msg, 0);
}

struct Graph{
    /** @var pocet uzlu grafu */
    unsigned int size;
    /** @var matice sousednosti */
    vector< vector<bool> > matrix;
    vector< set<unsigned int> > neighbourhoodMap;
};



class Dominator {
private:
    unsigned int m_level;
    unsigned int m_debugLevel;
    
    Graph * graph;
public:
    Dominator(unsigned int level, Graph & graph){
        this->m_level = level;
        this->m_graph = graph;
    }
    
    start(){
        Log("Started", INFO);
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
            }elseif (strcmp(argv[1]), "-d") == 1 {
                debugLevel = DEBUG;
            }
        }
    }else{
        Log("[-i,-d] <input_file> <i>");
        return 1;
    }
    
    Log("Debug level: " + debugLevel)
    
    Graph graph;
    
    char* fileName = argv[argc -2];
    
    if ( ! loadGraph(fileName, &graph)) {
        Log("File " + fileName + " isnt valid or doesnt exist.")
        return 1;
    }else{
        Log("File" + fileName + " has been loaded.", INFO);
    }
    
    unsigned int level = atoi(argv[argc-1]);
    
    if (level < 0){
        Log("Invalid level, use value > 0.");
        return 1;
    }else{
        Log("Level (i-value) is set to: " + level, INFO);
    }
    
    Dominator dom(level, graph);
    dom.start();
}
