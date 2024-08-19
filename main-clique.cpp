#include "matrizadj/Grafo.h" 
#include <fstream>
using namespace std;

int main (int argc, char **argv) 
{
    ifstream in ("input.txt");
    Grafo *grafo = new Grafo (in);
    grafo->imprime ();  

    vector<int> clique = grafo->maxClique();
    cout << "max clique :";
    for (int i : clique)
    {
        cout << i << " ";
    }
    cout << endl;

    return 0;    
}
