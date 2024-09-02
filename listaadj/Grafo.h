#include "Lista.h" 
#include "FPHeapMinIndireto.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <array>
#include <cstring>
#include <float.h>
#include <limits>
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
        return *this; // @{\it permite atribui\c{c}\~oes encadeadas}@
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
    ~Grafo ();	 

    int fordFulkerson(int source, int sink);
    void imprimirGrafoResidual() const;
    void imprimirFluxo() const;
    void imprimirCaminhosAumentantes() const;
  
  private:
    std::vector<std::vector<int>> capacity; // Matriz de capacidades
    std::vector<std::vector<int>> flow;     // Matriz de fluxo
    std::vector<std::vector<int>> parentPath; // Armazena o caminho de aumento
    bool bfs(int source, int sink, std::vector<int>& parent);
    void printPath(int source, int sink, const std::vector<int>& parent) const;
    void initCapacityAndFlow();
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
  int Grafo::_numVertices () const { 
    return this->numVertices;
  }
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

  // Método para inicializar as matrizes de capacidade e fluxo
  // Método para inicializar as matrizes de capacidade e fluxo
  void Grafo::initCapacityAndFlow() {
    int n = this->numVertices;
    capacity.assign(n, std::vector<int>(n, 0));
    flow.assign(n, std::vector<int>(n, 0));

    // Preencher a matriz de capacidades
    for (int u = 0; u < n; ++u) {
      Celula* item = this->adj[u]._primeiro();
      while (item != NULL) {
        capacity[u][item->vertice] = item->peso;
        item = this->adj[u].proximo();
      }
    }
  }

  //busca em largura
  bool Grafo::bfs(int source, int sink, std::vector<int>& parent) {
    int n = this->numVertices;
    std::vector<bool> visited(n, false);
    std::queue<int> q;

    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
      int u = q.front();
      q.pop();

      for (int v = 0; v < n; ++v) {
        if (!visited[v] && capacity[u][v] - flow[u][v] > 0) {
          q.push(v);
          visited[v] = true;
          parent[v] = u;
          if (v == sink) return true;
        }
      }
    }
    return false;
  }

  int Grafo::fordFulkerson(int source, int sink) {
    initCapacityAndFlow();

    std::vector<int> parent(numVertices);
    int maxFlow = 0;

    while (bfs(source, sink, parent)) {
      int pathFlow = std::numeric_limits<int>::max();
      int s = sink;

      while (s != source) {
        int u = parent[s];
        pathFlow = std::min(pathFlow, capacity[u][s] - flow[u][s]);
        s = u;
      }

      s = sink;
      while (s != source) {
        int u = parent[s];
        flow[u][s] += pathFlow;
        flow[s][u] -= pathFlow;
        s = u;
      }

      maxFlow += pathFlow;
      cout << "Caminho de aumento encontrado com fluxo: " << pathFlow << endl;
      printPath(source, sink, parent);
    }

    cout << "Fluxo Máximo: " << maxFlow << endl;
    return maxFlow;
  }

  void Grafo::printPath(int source, int sink, const std::vector<int>& parent) const {
    if (parent[sink] == -1) {
      cout << "Nenhum caminho de aumento encontrado." << endl;
      return;
    }
    cout << "Caminho de aumento: ";
    int v = sink;
    vector<int> path;
    while (v != source) {
      path.push_back(v);
      v = parent[v];
    }
    path.push_back(source);
    reverse(path.begin(), path.end());

    for (size_t i = 0; i < path.size(); ++i) {
      cout << path[i];
      if (i < path.size() - 1) cout << " -> ";
    }
    cout << endl;
  }

  void Grafo::imprimirGrafoResidual() const {
    cout << "Grafo Residual:" << endl;
    for (int u = 0; u < numVertices; ++u) {
      cout << "Vertice " << u << ":";
      for (int v = 0; v < numVertices; ++v) {
        if (capacity[u][v] - flow[u][v] > 0) {
          cout << " -> " << v << " (Capacidade Residual: " << capacity[u][v] - flow[u][v] << ")";
        }
      }
      cout << endl;
    }
  }

  void Grafo::imprimirFluxo() const {
    cout << "Fluxo no Grafo:" << endl;
    for (int u = 0; u < numVertices; ++u) {
      cout << "Vertice " << u << ":";
      for (int v = 0; v < numVertices; ++v) {
        if (flow[u][v] > 0) {
          cout << " -> " << v << " (Fluxo: " << flow[u][v] << ")";
        }
      }
      cout << endl;
    }
  }