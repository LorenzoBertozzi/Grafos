#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <array>
#include <cstring>
#include <float.h>
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
	    ~Aresta () {}
	  };
	private:
	  int **mat; // @{\it pesos do tipo inteiro}@
    int numVertices;
    int *pos; // @{\it posi\c{c}\~ao atual ao se percorrer os adjs de um v\'ertice v}@

  public:
    Grafo (int numVertices);
    Grafo(istream &in);
    Grafo(int numVertices, int numArestas);
    void insereAresta(int v1, int v2, int peso);
    bool existeAresta(int v1, int v2) const;
    bool listaAdjVazia(int v) const;
    Aresta *lerAresta();
    Aresta *primeiroListaAdj(int v);
    Aresta *proxAdj(int v);
    Aresta *retiraAresta(int v1, int v2);
    void imprime() const;
    int _numVertices() const;
    Grafo *grafoTransposto();
    Grafo *grafoNaoDirecionado();
    ~Grafo ();	  

    void floydWarshall(int start, int end);
    void floydWarshall();
	};

  Grafo::Grafo( istream &in )
  {
   int v1, v2, peso;
   in >> this->numVertices;
   this->mat = new int*[numVertices];
    for (int i = 0; i < numVertices; i++) 
      this->mat[i] = new int[numVertices];
    this->pos = new int[numVertices]; 
    
    for (int i = 0; i < this->numVertices; i++) {
      for (int j = 0; j < this->numVertices; j++) this->mat[i][j] = 0;
      this->pos[i] = -1; 
    }

    while (in>>v1>>v2>>peso) {
      Grafo::Aresta *a = new Grafo::Aresta (v1, v2, peso);
      this->insereAresta (a->_v1 (), a->_v2 (), a->_peso ()); 
      delete a;
    }
  }

  Grafo::Grafo (int numVertices) {
    this->mat = new int*[numVertices];
    for (int i = 0; i < numVertices; i++) 
      this->mat[i] = new int[numVertices];
    this->pos = new int[numVertices]; 
    this->numVertices = numVertices; 
    for (int i = 0; i < this->numVertices; i++) {
      for (int j = 0; j < this->numVertices; j++) this->mat[i][j] = 0;
      this->pos[i] = -1; 
    }
  }	  
  
  Grafo::Grafo (int numVertices, int numArestas) {
    this->mat = new int*[numVertices];
    for (int i = 0; i < numVertices; i++) 
      this->mat[i] = new int[numVertices];
    this->pos = new int[numVertices]; 
    this->numVertices = numVertices; 
    for (int i = 0; i < this->numVertices; i++) {
      for (int j = 0; j < this->numVertices; j++) this->mat[i][j] = 0;
      this->pos[i] = -1; 
    }
  }	  

  void Grafo::insereAresta (int v1, int v2, int peso) {
    this->mat[v1][v2] = peso; 
  }
  bool Grafo::existeAresta (int v1, int v2) const {
    return (this->mat[v1][v2] > 0);
  }
  bool Grafo::listaAdjVazia (int v) const {
    for (int i = 0; i < this->numVertices; i++)
      if (this->mat[v][i] > 0) return false;
    return true;
  }

  Grafo::Aresta *Grafo::lerAresta()
  {
    cout << "Aresta:" << endl;
    cout << "  V1:"; int v1 = 0;
    cin >> v1;
    cout << "  V2:"; int v2 = 0;
    cin >> v2;
    cout << "  Peso:"; int peso = 0;
    cin >> peso;
    return new Aresta (v1, v2, peso);
  }

  Grafo::Aresta *Grafo::primeiroListaAdj (int v) {
    // @{\it Retorna a primeira aresta que o v\'ertice v participa ou}@ 
    // @{\it {\bf NULL} se a lista de adjac\^encia de v for vazia}@
    this->pos[v] = -1; return this->proxAdj (v);
  }
  
  Grafo::Aresta *Grafo::proxAdj (int v) {
    // @{\it Retorna a pr\'oxima aresta que o v\'ertice v participa ou}@ 
    // @{\it {\bf NULL} se a lista de adjac\^encia de v estiver no fim}@ 
    this->pos[v] ++;
    while ((this->pos[v] < this->numVertices) && 
           (this->mat[v][this->pos[v]] == 0)) this->pos[v]++;
    if (this->pos[v] == this->numVertices) return NULL;
    else return new Aresta (v, this->pos[v], this->mat[v][this->pos[v]]);
  }
  
  Grafo::Aresta *Grafo::retiraAresta (int v1, int v2) {
    if (this->mat[v1][v2] == 0) return NULL; // @{\it Aresta n\~ao existe}@
    else {
      Aresta *aresta = new Aresta (v1, v2, this->mat[v1][v2]);
      this->mat[v1][v2] = 0; return aresta;
    }
  }
  
  void Grafo::imprime () const {
    cout << "   ";
    for (int i = 0; i < this->numVertices; i++) 
      cout << i << "   "; 
    cout << endl;
    for (int i = 0; i < this->numVertices; i++) { 
      cout << i <<  "  ";
      for (int j = 0; j < this->numVertices; j++)
        cout << this->mat[i][j] << "   ";
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

  Grafo *Grafo::grafoNaoDirecionado () {
    Grafo *grafoND = new Grafo (this->numVertices); 
    //Ex5: Implementar o grafo não direcionado
    return grafoND;
  }

  Grafo::~Grafo()
  {
    for (int i = 0; i < numVertices; i++) 
      delete [] this->mat[i];
    delete [] this->mat;
    delete [] this->pos;
  }

  void Grafo::floydWarshall(int start, int end) {
    vector<vector<int>> dist(numVertices, vector<int>(numVertices, INT8_MAX));
    vector<vector<int>> pred(numVertices, vector<int>(numVertices, -1));//

    for (int i = 0; i < numVertices; ++i) {
      for (int j = 0; j < numVertices; ++j) {
        if (i == j) {
          dist[i][j] = 0;
        } else if (mat[i][j] != 0) {
          dist[i][j] = mat[i][j];
          pred[i][j] = i;//
        }
      }
    }

    for (int k = 0; k < numVertices; ++k) {
      for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
          if (dist[i][k] != INT8_MAX && dist[k][j] != INT8_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
            dist[i][j] = dist[i][k] + dist[k][j];
            pred[i][j] = pred[k][j];//
          }
        }
      }
    }

    cout << "Matriz de distancia Floyd-Warshall:" << endl;
    for (int i = 0; i < numVertices; ++i) {
      for (int j = 0; j < numVertices; ++j) {
      if (dist[i][j] == INT8_MAX) {
        cout << "INF ";
        } else {
          cout << dist[i][j] << " ";
      }
      }
      cout << endl;
    }


    auto imprimirCaminho = [&](int start , int end){
      if(dist[start][end] == INT8_MAX){
        cout << "Não existe caminho entre " << start << " e " << end << endl;
        return;
      }
      vector<int> path;
        for (int v = end; v != -1; v = pred[start][v]) {
            path.push_back(v);
        }
      reverse(path.begin(), path.end());
      cout << "\nCaminho minimo de " << start << " para " << end << " : " << endl;
      for(int v : path){
        cout << v << " ";
      }
      cout << endl;
    };

    //imprimir caminho
    imprimirCaminho(start, end);
  }

  void Grafo::floydWarshall() {
    vector<vector<int>> dist(numVertices, vector<int>(numVertices, INT8_MAX));

    for (int i = 0; i < numVertices; ++i) {
      for (int j = 0; j < numVertices; ++j) {
        if (i == j) {
          dist[i][j] = 0;
        } else if (mat[i][j] != 0) {
          dist[i][j] = mat[i][j];
        }
      }
    }

    for (int k = 0; k < numVertices; ++k) {
      for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
          if (dist[i][k] != INT8_MAX && dist[k][j] != INT8_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
            dist[i][j] = dist[i][k] + dist[k][j];
          }
        }
      }
    }

    cout << "Matriz de distancia Floyd-Warshall:" << endl;
    for (int i = 0; i < numVertices; ++i) {
      for (int j = 0; j < numVertices; ++j) {
      if (dist[i][j] == INT8_MAX) {
        cout << "INF ";
        } else {
          cout << dist[i][j] << " ";
      }
      }
      cout << endl;
    }
  }

		
