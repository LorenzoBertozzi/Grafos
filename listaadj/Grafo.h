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
  //BUSCA EM PORFUNDIDADE
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
  /*
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
  //CRIR UMA ARVORE GERADORA MINIMA USANDO ALGORITIMO DE KRUSKAL
  void Grafo::kruskal(){
    vector<Grafo::Aresta*> arestasAGM;
    criaConjunto();

    vector<pair<int, pair<int, int>>> arestaOrdenas;;
    for(int u = 0; u < numVertices; u++){
      Grafo::Aresta* adj = primeiroListaAdj(u);
      while (adj != nullptr){
        int v = adj->_v2();
        int peso = adj->_peso();
        arestaOrdenas.push_back({peso, {u,v}});
        adj = proxAdj(u);
      }
    }
    sort(arestaOrdenas.begin(), arestaOrdenas.end());

    for (auto & aresta : arestaOrdenas){
      int u = aresta.second.first;
      int v = aresta.second.second;
      if(encontreConjunto(u) != encontreConjunto(v)){
        arestasAGM.push_back(new Grafo::Aresta(u, v, aresta.first));
        unirConjunto(u,v);
      }
    }
    
    cout << "Árvore Geradora Mínima (AGM) encontrada por Kruskal:" << endl;
    for(auto& aresta : arestasAGM){
              cout << "(" << aresta->_v1() << ", " << aresta->_v2() << ") - Peso: " << aresta->_peso() << endl;
    }
  }

  void Grafo::criaConjunto(){
    for (int v = 0; v < numVertices; ++v) {
        conjunto[v] = -1;
    }
  }
  
  int Grafo::encontreConjunto(int x){
    if(conjunto[x] == -1){
      return x;
    }
    return encontreConjunto(conjunto[x]);
  }

  void Grafo::unirConjunto(int x, int y){
    int conjuntoX = encontreConjunto(x);
    int conjuntoY = encontreConjunto(y);
    conjunto[conjuntoX] = conjuntoY;
  }
  */

//CRIA UM ARVORE GERADORA MINIMA ULTILIZANDO ALGORITMO PRIM
  void Grafo::prim(int raiz) {
    const int INF = INT8_MAX; // Definindo infinito como o maior valor possível
    double* peso = new double[numVertices]; // Array para armazenar os pesos dos vértices
    int* antecessor = new int[numVertices]; // Array para armazenar os antecessores dos vértices na AGM
    bool* itensHeap = new bool[numVertices]; // Array para rastrear se um vértice está no heap ou não
    int* vs = new int[numVertices + 1]; // Array para heap indireto

    for (int u = 0; u < numVertices; ++u) {
        peso[u] = INF; // Inicializa os pesos como infinito
        antecessor[u] = -1; // Inicializa os antecessores como -1 (indicando nenhum antecessor)
        itensHeap[u] = true; // Inicializa todos os vértices como presentes no heap
        vs[u + 1] = u; // Inicializa o heap indireto
    }

    peso[raiz] = 0; // Define o peso da raiz como 0

    FPHeapMinIndireto<double> Q(peso, vs, numVertices); // Criação do heap

    while (!Q.vazio()) {
        int u = Q.retiraMin(); // Retira o vértice de menor peso do heap
        itensHeap[u] = false; // Marca o vértice como retirado do heap

        Aresta* adj = primeiroListaAdj(u); // Obtém as arestas adjacentes ao vértice u
        while (adj != nullptr) {
            int v = adj->_v2(); // Obtém o segundo vértice da aresta (o vértice adjacente)
            int pesoAresta = adj->_peso(); // Obtém o peso da aresta

            if (itensHeap[v] && pesoAresta < peso[v]) {
                antecessor[v] = u; // Define o vértice u como antecessor de v
                Q.diminuiChave(v, pesoAresta); // Atualiza o peso de v no heap
            }

            adj = proxAdj(u); // Passa para a próxima aresta adjacente
        }
    }

    // Imprime a Árvore Geradora Mínima encontrada por Prim
    cout << "Árvore Geradora Mínima (AGM) encontrada por Prim:" << endl;
    for (int v = 0; v < numVertices; ++v) {
        if (antecessor[v] != -1) { // Se o vértice tem um antecessor na AGM
            cout << "(" << antecessor[v] << ", " << v << ")" << endl; // Imprime a aresta (antecessor[v], v)
        }
    }

    // Libera memória alocada dinamicamente
    delete[] peso;
    delete[] antecessor;
    delete[] itensHeap;
    delete[] vs;
}



