#include "Lista.h" 
#include "FPHeapMinIndireto.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <array>
#include <cstring>
#include <float.h>
#include <stack>
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
    vector <int> conjunto;

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

    void buscaEmLargura ();
    void visitaBfs (int u, vector <string>& cor, vector <int>&dist, vector <int>&antecessor);
    void buscaEmProfundidade();
    void visitaDFS(int u, vector<string>& cor, vector<int>& antecessor);
    bool temCiclo(); // verifica se o grafo tem cilho 
    bool visitaDFSComCiclo(int u, vector<string>& cor);//busca ciclo do grafo
    int contarComponentes(); //retorna quantidade de vertices
    void visitaDfsTopologica(int u, vector<string>& cor, stack<int>& pilha);
    void ordenacaoTopologica();//Mostrar uma ordenação topológica
    void imprimeCaminho (int u, int v);

    void kruskal();
    void criaConjunto();
    int encontreConjunto(int x);
    void unirConjunto(int x, int y);

    void prim(int raiz);
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
  //BUSCA EM LARGURA
  void Grafo::buscaEmLargura(){
    vector <string> cor (numVertices, "branco");
    vector <int> dist (numVertices, INT8_MAX);
    vector <int> antecessor(numVertices, -1);

    for (int u = 0; u < numVertices; u++){
      if(cor[u] == "branco"){
        visitaBfs(u, cor, dist, antecessor);
      }
    }
  }	
  void Grafo::visitaBfs(int u, vector <string>& cor, vector <int>&dist, vector <int>&antecessor){
    dist[u] = 0;
    cor[u] = "cinza";

    //cout << "-" << u << " cor: " << cor[u] << " dist: " << dist[u] << " ant: " << antecessor[u] << endl;

    queue<int> fila;
    fila.push(u);

    while (!fila.empty()){
      u = fila.front();
      fila.pop();
      Aresta* adj = primeiroListaAdj(u);
      while (adj != NULL){
        int v= adj->_v2();
        if(cor[v] == "branco"){
          cor[v] = "cinza";
          dist[v] = dist[u] + 1;
          antecessor[v] = u;
          fila.push(v);

          //cout << "-" << v << " cor: " << cor[v] << " dist: " << dist[v] << " ant: " << antecessor[v] << endl;
        }
        adj = proxAdj(u);
      }
      cor[u] = "preto";

      //cout << "-" << u << " cor: " << cor[u] << " dist: " << dist[u] << " ant: " << antecessor[u]  << "\n" << endl;
    }
  }
  //BUSCA EM PROFUNDIDADE
  void Grafo::buscaEmProfundidade() {
    vector<string> cor(numVertices, "branco");
    vector<int> antecessor(numVertices, -1);

    for (int u = 0; u < numVertices; u++) {
        if (cor[u] == "branco") {
          //cout << u << " cor : " << cor[u] << " antecessor : " << antecessor[u] << endl;
          visitaDFS(u, cor, antecessor);
        }
    }
  }
  void Grafo::visitaDFS(int u, vector<string>& cor, vector<int>& antecessor) {
    cor[u] = "cinza";
    Aresta* adj = primeiroListaAdj(u);
    //cout << u << " cor : " << cor[u] << " antecessor : " << antecessor[u] << endl;
    while (adj != NULL) {
        int v = adj->_v2();
        if (cor[v] == "branco") {
          antecessor[v] = u;          
          visitaDFS(v, cor, antecessor);
          //cout << v << " cor : " << cor[v] << " antecessor : " << antecessor[v] << endl;
        }
        adj = proxAdj(u);
    }

    cor[u] = "preto";
  }
  //VERIFICA SE O GRAFO POSSUI CICLO ULTILIZANDO DFS
  bool Grafo::temCiclo() {
    vector<string> cor(numVertices, "branco");

    for (int u = 0; u < numVertices; u++) {
        if (cor[u] == "branco") {
            if (visitaDFSComCiclo(u, cor))
                return true; // Se encontrou um ciclo, retorna verdadeiro
        }
    }

    return false; // Se não encontrou ciclos, retorna falso
  }
  bool Grafo::visitaDFSComCiclo(int u, vector<string>& cor) {
    cor[u] = "cinza";
    Aresta* adj = primeiroListaAdj(u);

    while (adj != NULL) {
        int v = adj->_v2();
        if (cor[v] == "cinza")
            return true; // Se encontrou um vértice cinza, significa que há um ciclo

        if (cor[v] == "branco" && visitaDFSComCiclo(v, cor))
            return true; // Se encontrou um ciclo em uma chamada recursiva, retorna verdadeiro

        adj = proxAdj(u);
    }

    cor[u] = "preto";
    return false; // Não encontrou ciclo a partir deste vértice
  }
  //VERIFICAR QUANTIDADE DE COMPONETES DO GRAFO (QUANTIDADE DE GRAFOS)
  int Grafo::contarComponentes() {
    vector<string> cor(numVertices, "branco");
    vector<int> antecessor(numVertices, -1);
    int numComponentes = 0;
    for (int u = 0; u < numVertices; u++) {
        if (cor[u] == "branco") {
          numComponentes++;
          visitaDFS(u, cor, antecessor);
        }
    }
    return numComponentes;
  } 
  //MOSTRA O CAMINHO MAIS CURTO ENTRE DOIS VERTICES
  void Grafo::imprimeCaminho(int u, int v){
    vector <string> cor (numVertices, "branco");
    vector <int> dist (numVertices, INT8_MAX);
    vector <int> antecessor(numVertices, -1);

    visitaBfs(u, cor, dist, antecessor);

    if (u == v) {
        cout << v << " " ; 
    } else if (antecessor[v] == -1) {
        cout << "Não existe caminho de " << u << " para " << v << endl;
    } else {
        imprimeCaminho(u, antecessor[v]); 
        cout << v << " "; 
    }
  }
  //MOSTRAR UMA ORDENAÇÃO TOPOLOGICA PARA O GRAFO
  void Grafo::visitaDfsTopologica(int u, vector<string>& cor, stack<int>& pilha) {
    cor[u] = "cinza";

    Aresta* adj = primeiroListaAdj(u);
    while (adj != NULL) {
        int v = adj->_v2();
        if (cor[v] == "branco") {
            visitaDfsTopologica(v, cor, pilha);
        }
        adj = proxAdj(u);
    }

    cor[u] = "preto";
    pilha.push(u);
}
  void Grafo::ordenacaoTopologica(){
    vector<string> cor(numVertices, "branco");
    stack <int> pilha;

    for(int i = 0; i < numVertices; i++){
      if(cor[i] == "branco"){
        visitaDfsTopologica(i , cor, pilha);
      }
    }

    //imprimir ordenação topologica
    cout << "Ordenação topologica : ";
    while (!pilha.empty()){
      cout << pilha.top() << " ";
      pilha.pop();
    }
    cout << endl;
  }
  


