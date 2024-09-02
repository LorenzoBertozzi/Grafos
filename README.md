# Identificação de Planaridade
A ativide conciste em elaborar formas de se identificar sobreposicoes de arestas em circuitos impressos de eletronica.

# Teorema de Kuratowski e Teste de Planaridade
## Introdução
O Teorema de Kuratowski é um resultado fundamental na teoria dos grafos, que afirma que um grafo é planar se e somente se não contém um subgrafo que seja uma subdivisão de K5 (grafo completo em 5 vértices) ou K3,3 (grafo bipartido completo em 3 +3 vértices). Este teorema fornece uma ferramenta poderosa para testar a planaridade de um gráfico.

## Codigo
O código é uma implementação do Teorema de Kuratowski em C++. Consiste em uma classe Grafo, que representa um gráfico, e duas funções : isPlanar() e isHomeomorphicToK5() e isHomeomorphicToK33().

### Como o codigo funciona
* 1 - isPlanar(): Esta função testa se um gráfico é plano ou não. Ele faz isso encontrando todos os subgrafos de Kuratowski (subgrafos K4) e depois verificando se algum desses subgrafos é homeomórfico a K5 ou K3,3. Se for encontrado um subgrafo homeomórfico, a função retorna falso, indicando que o gráfico não é planar. Caso contrário, ele retorna verdadeiro.

* 2 - isHomeomorphicToK5() e isHomeomorphicToK33(): Estas funções verificam se um determinado subgrafo é homeomorfo a K5 ou K3,3, respectivamente. Eles fazem isso verificando o número de vértices e arestas no subgrafo, bem como a conectividade e ausência de ciclos de comprimento 3.

### conceito chave
  * Teorema de Kuratowski: Um gráfico é planar se e somente se não contém um subgrafo que seja uma subdivisão de K5 ou K3,3.
  * Subdivisão: Uma subdivisão de um gráfico é um gráfico obtido substituindo cada aresta do gráfico original por um caminho de comprimento pelo menos 1.
  * Homeomorfismo: Dois gráficos são homeomórficos se podem ser transformados um no outro por uma série de contrações de arestas e identificações de vértices.
  * Planaridade: Um gráfico é planar se puder ser desenhado no plano sem cruzamentos de arestas.

## Exemplo de Uso

```
  Grafo g; // create a graph
  // add vertices and edges to the graph
  bool isPlanar = g.isPlanar(); // test planarity
  if (isPlanar) {
      cout << "The graph is planar." << endl;
  } else {
      cout << "The graph is not planar." << endl;
  }
```

## Conclusão
O código fornecido é uma implementação do Teorema de Kuratowski, que fornece uma ferramenta poderosa para testar a planaridade de um gráfico. Ao compreender os conceitos do Teorema de Kuratowski e como o código funciona, você pode usar esta implementação para testar a planaridade dos gráficos em seus próprios aplicativos.

