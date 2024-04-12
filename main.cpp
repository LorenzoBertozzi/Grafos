#include "listaadj/Grafo.h" 
#include <fstream>
using namespace std;

int main (int argc, char **argv) 
{
    ifstream in ("input.txt");
    Grafo *grafo = new Grafo (in);
    grafo->imprime ();  

    //grafo->buscaEmLargura();
    //grafo->buscaEmProfundidade();

    //1- Verificar se o grafo possui ciclos.
    if(grafo->temCiclo()){
        cout << "ciclico" << endl;
    }
    else{
        cout << "nao tem ciclo" << endl;
    }

    //2- Verificar a quantidade de componentes do grafo.
    int numComponentes = grafo->contarComponentes();
    cout << "O grafo possui " << numComponentes << " componentes." << endl;

    //grafo->imprimeCaminho(0,6);

    grafo->ordenacaoTopologica();

    grafo->kruskal();

}
