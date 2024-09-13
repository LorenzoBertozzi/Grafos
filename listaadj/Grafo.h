#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <array>
#include <cstring>
#include <float.h>
#include "Lista.h" 
#include "FPHeapMinIndireto.h"

using namespace std;

class Grafo {
public:
    class Aresta {
    private:
        int v1, v2, peso;
    public: 
        Aresta (int v1, int v2, int peso) {
            this->v1 = v1; this->v2 = v2; this->peso = peso;
        }
        int _peso () { return this->peso; }
        int _v1 () { return this->v1; }
        int _v2 () { return this->v2; }
        ~Aresta(){}
    };

private:    
    class Celula {
        friend class Grafo;
        friend ostream& operator<< (ostream& out, const Celula& celula) {
            out << "vertice:" << celula.vertice << endl;
            out << "peso:"    << celula.peso    << endl;
            return out;
        }    
    private:    
        int vertice, peso;
    public:
        Celula (int v, int p) {
            this->vertice = v; this->peso = p;
        }
        Celula (const Celula& cel) { *this = cel; }      
        bool operator== (const Celula& celula) const {
            return this->vertice == celula.vertice;
        }
        bool operator!= (const Celula& celula) const {
            return this->vertice != celula.vertice;
        }
        const Celula& operator= (const Celula& cel) {     
            this->vertice = cel.vertice; this->peso = cel.peso;
            return *this; 
        }      
        ~Celula () {}        
    }; 

    Lista<Celula> *adj;
    int numVertices;

public:
    Grafo( istream &in );
    Grafo (int numVertices);	  
    Grafo (int numVertices, int numArestas);	  
	  void insereAresta (int v1, int v2, int peso);
	  bool existeAresta (int v1, int v2) const;
	  bool listaAdjVazia (int v) const;
    Aresta *lerAresta ();
	  Aresta *primeiroListaAdj (int v);
	  Aresta *proxAdj (int v);
	  Aresta *retiraAresta (int v1, int v2);
	  void imprime () const ;
	  int _numVertices () const;
	  Grafo *grafoTransposto ();

    bool contemK5OuSubdivisao();
    bool contemK33OuSubdivisao();
    bool ehPlanar();

    ~Grafo();
};

  Grafo::Grafo( istream &in )
  {
   int v1, v2, peso, numVertices;
   in >> numVertices;
  
   this->adj = new Lista<Celula>[numVertices]; 
   this->numVertices = numVertices; 	  	  	
   
    while (in>>v1>>v2>>peso) {
      Grafo::Aresta *a = new Grafo::Aresta (v1, v2, peso);
      this->insereAresta (a->_v1 (), a->_v2 (), a->_peso ()); 
      delete a;
    }
  }

  Grafo::Grafo (int numVertices) {
  	this->adj = new Lista<Celula>[numVertices]; 
  	this->numVertices = numVertices; 	  	
  }	  
  Grafo::Grafo (int numVertices, int numArestas) {
  	this->adj = new Lista<Celula>[numVertices]; 
  	this->numVertices = numVertices; 	  	
  }	  

  Grafo::Aresta *lerAresta () {
    cout << "Aresta:" << endl;
    cout << "  V1:"; int v1 = 0;
    cin >> v1;
    cout << "  V2:"; int v2 = 0;
    cin >> v2;
    cout << "  Peso:"; int peso = 0;
    cin >> peso;
    return new Grafo::Aresta (v1, v2, peso);
  }

  void Grafo::insereAresta (int v1, int v2, int peso) {
    Celula item (v2, peso); 
    this->adj[v1].insere (item); 
  }
  bool Grafo::existeAresta (int v1, int v2) const {
    Celula item (v2, 0);
    return (this->adj[v1].pesquisa (item) != NULL);
  }
  bool Grafo::listaAdjVazia (int v) const {
		return this->adj[v].vazia ();  
  }	  
  Grafo::Aresta *Grafo::primeiroListaAdj (int v) {
    // @{\it Retorna a primeira aresta que o v\'ertice v participa ou}@
    // @{\it {\bf NULL} se a lista de adjac\^encia de v for vazia}@ 
    Celula *item = this->adj[v]._primeiro ();    
    return item != NULL ? new Aresta(v,item->vertice,item->peso) : NULL;
  }
  Grafo::Aresta *Grafo::proxAdj (int v) {
    // @{\it Retorna a pr\'oxima aresta que o v\'ertice v participa ou}@
    // @{\it {\bf NULL} se a lista de adjac\^encia de v estiver no fim}@
    Celula *item = this->adj[v].proximo ();    
    return item != NULL ? new Aresta(v,item->vertice,item->peso) : NULL;
  }
  Grafo::Aresta *Grafo::retiraAresta (int v1, int v2) {
    Celula chave (v2, 0);
    Celula *item = this->adj[v1].retira (chave);
    Aresta *aresta = item != NULL ? new Aresta(v1,v2,item->peso) : NULL;
    delete item; return aresta;
  }
  void Grafo::imprime () const {
    for (int i = 0; i < this->numVertices; i++) { 
      cout << i << ":";
      Celula *item = this->adj[i]._primeiro ();
      while (item != NULL) {
        cout << "  " << item->vertice << " (" <<item->peso<< ")";
        item = this->adj[i].proximo ();
      }
      cout << endl;
    }
  }
  int Grafo::_numVertices () const { return this->numVertices; }
  Grafo *Grafo::grafoTransposto () {  	
    Grafo *grafoT = new Grafo (this->numVertices); 
    for (int v = 0; v < this->numVertices; v++)
      if (!this->listaAdjVazia (v)) {
        Aresta *adj = this->primeiroListaAdj (v);
        while (adj != NULL) {
          grafoT->insereAresta (adj->_v2 (), adj->_v1 (), adj->_peso ());
          delete adj;
          adj = this->proxAdj (v);
        }
      }
    return grafoT;
  }
  Grafo::~Grafo () {
    delete [] this->adj;
  }	  

  bool Grafo::contemK5OuSubdivisao() {
    // Verificar todos os subconjuntos de 5 vértices
    if (this->numVertices < 5) return false; // Não pode conter K5 se não tem ao menos 5 vértices
    
    vector<int> vertices(this->numVertices);
    for (int i = 0; i < this->numVertices; ++i) vertices[i] = i;

    // Gerar combinações de 5 vértices e verificar se formam K5
    do {
        int v1 = vertices[0], v2 = vertices[1], v3 = vertices[2], v4 = vertices[3], v5 = vertices[4];
        // Verificar se existe uma aresta entre todos os pares
        if (existeAresta(v1, v2) && existeAresta(v1, v3) && existeAresta(v1, v4) && existeAresta(v1, v5) &&
            existeAresta(v2, v3) && existeAresta(v2, v4) && existeAresta(v2, v5) &&
            existeAresta(v3, v4) && existeAresta(v3, v5) && existeAresta(v4, v5)) {
            return true; // Encontrado subgrafo isomórfico a K5
        }
    } while (next_permutation(vertices.begin(), vertices.end()));

    return false;
}

bool Grafo::contemK33OuSubdivisao() {
    if (this->numVertices < 6) return false; // Não pode conter K33 se não tem ao menos 6 vértices

    vector<int> vertices(this->numVertices);
    for (int i = 0; i < this->numVertices; ++i) vertices[i] = i;

    // Gerar combinações de 6 vértices e verificar se formam K33
    do {
        int a1 = vertices[0], a2 = vertices[1], a3 = vertices[2];
        int b1 = vertices[3], b2 = vertices[4], b3 = vertices[5];

        // Verificar se existe uma aresta entre todos os pares de {a1, a2, a3} com {b1, b2, b3}
        if (existeAresta(a1, b1) && existeAresta(a1, b2) && existeAresta(a1, b3) &&
            existeAresta(a2, b1) && existeAresta(a2, b2) && existeAresta(a2, b3) &&
            existeAresta(a3, b1) && existeAresta(a3, b2) && existeAresta(a3, b3)) {
            return true; // Encontrado subgrafo isomórfico a K33
        }
    } while (next_permutation(vertices.begin(), vertices.end()));

    return false;
}

bool Grafo::ehPlanar() {
    // Passo 1: Verificar a condição de Euler para planaridade
    int m = 0; // Número de arestas
    for (int v = 0; v < this->numVertices; ++v) {
        Aresta *adj = this->primeiroListaAdj(v);
        while (adj != NULL) {
            m++;
            adj = this->proxAdj(v);
        }
    }
    m /= 2; // Como contamos duas vezes as arestas

    if (m > 3 * this->numVertices - 6) return false; // Não planar se m > 3n - 6

    // Passo 2: Verificar se contém K5 ou K33
    if (contemK5OuSubdivisao() || contemK33OuSubdivisao()) return false;

    return true; // Se passou nas verificações, é planar
}
