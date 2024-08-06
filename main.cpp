#include "matrizadj/Grafo.h" 
#include <fstream>
using namespace std;

int main (int argc, char **argv) 
{
    ifstream in ("input.txt");
    Grafo *grafo = new Grafo (in);
    grafo->imprime ();  

    grafo->floydWarshall();

    grafo->floydWarshall(1,4);
}
