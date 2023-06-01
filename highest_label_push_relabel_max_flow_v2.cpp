// Uma implementação do algoritmo push-relabel para o problema de fluxo máximo.
// Tempo de pior caso: O(n^2 * sqrt(m))
//
// Considere um grafo G = (V,A), onde V denota o conjunto de nós (vértices) no grafo
// e A denota o conjunto de arcos (arestas), e um par de vértices distintos s e t
// em G chamados fonte e terminal. n = |V| denota o número de nós no grafo, e m = |A|
// denota o número de arcos no grafo.
//
// Para um fluxo ser válido, ele deve atender as seguintes restrições:
//
//  a) f(v,w) <= c(v,w) para todo (v,w) em E (restrição de capacidade)
//
//  b) f(v,w) = -f(w,v) para todo (v,w) em E (restrição de antissimetria)
//
//  c) soma em todo u em V, f(v,w) = 0  (restrição de conservacao de fluxo)


#include "graph.cpp"
#include "./dependencies/quickSort.cpp"
#include "./dependencies/insertionSort.cpp"

#include <queue>
#include <vector>
#include <algorithm>

using namespace std;


auto totGf_dprv2= (p - p).count();
auto totsort_dprv2= (p - p).count();
auto totpush_dprv2= (p - p).count();
auto totrelabel_dprv2= (p - p).count();
auto totdischarge_dprv2= (p - p).count();

/* Algoritmo de Ahuja e Orlin aprimorado por Cheriyan e Mehlhorn: highest label version of the push-relabel algorithm */

namespace operations_highest_label_push_relabel_max_flow_v2 {

  void preflowInitialize(VectorGraph &Gf, vector<int> &excesso, vector<int> &altura, vector<std::queue<int>> &b, int s, int t) {
    // satura todos os arcos que saem da fonte no grafo Original -> f(s,v) = u(s,v)
    vector<vectorImplementation::edge> &edgeList = Gf.get_edge_list(s);
    int sizeList = edgeList.size();
    
    for (int i = 0; i < sizeList; i++) {
      vectorImplementation::edge *e = &edgeList[i];

      // Atualiza o excesso do nó que recebeu fluxo e o insere na fila de prioridade de altura
      if (e->destNode != t and e->capacity > 0) {
        excesso[e->destNode] = e->capacity;
        b[0].push(e->destNode);
      }

      // Atualiza as capacidades residuais dos arcos diretos e reversos
      Gf.add_capacity(e->destNode, s, +e->capacity);
      e->capacity -= e->capacity;
    } 

    altura[s] = Gf.num_vertices();
    
    return;
  }


  void pushFlow(VectorGraph &Gf, int u, vectorImplementation::edge *e, vector<int> &excesso, vector<int> &altura, \
  vector<std::queue<int>> &b, int s, int t) {
    
    // Empurra fluxo no arco (u,v)
    int delta = min(excesso[u], e->capacity);
    
    // Atualiza a capacidade dos arcos (u,v) e (v,u) no grafo residual
    e->capacity -= delta;
    Gf.add_capacity(e->destNode, u, delta);

    // Insere o nó que recebeu excesso na fila se ele é diferente de s e t, e não possuia excesso antes da operação
    if (excesso[e->destNode] == 0 and (e->destNode != s and e->destNode != t)) {
      b[altura[e->destNode]].push(e->destNode);
    }

    // Atualiza o excesso de u
    excesso[u] -= delta;

    // Atualiza o excesso recebido por v
    if (e->destNode != s and e->destNode != t) {
      excesso[e->destNode] += delta; 
    }

    return;
  }


  void relabel(int u, vectorImplementation::edge e, vector<int> &altura, int *d) {
    altura[u] = altura[e.destNode] + 1;
    if (altura[u] > *d) {
      *d = altura[u];
    }

    return;
  }


  void discharge(int u, VectorGraph &Gf, vector<int> &excesso, vector<int> &altura,\
  vector<queue<int>> &b, int *d, int s, int t) {

    vector<vectorImplementation::edge> &edgeList = Gf.get_edge_list(u);

    auto startsort_dprv2= chrono::steady_clock::now();
    insertionSort(edgeList, altura);
    // quickSort(edgeList, 0, sizeList - 1, altura);
    // quickSortRSemiIter(edgeList, 0, sizeList - 1, altura);
    
    auto endsort_dprv2= chrono::steady_clock::now();
    totsort_dprv2 += (endsort_dprv2 - startsort_dprv2).count();
    
    auto startdischarge_dprv2= chrono::steady_clock::now();

    // Percorre a lista de arcos que saem de u
    for (int i = 0; excesso[u] > 0 and i < edgeList.size(); i++) {
      vectorImplementation::edge e = edgeList[i];

      if (e.capacity > 0) {
        // Relabel: ao encontrar um nó com altura igual ou maior que u, atualiza a altura de u
        // e faz com que a próxima iteração inicie nesse arco novamente
        if (altura[e.destNode] >= altura[u]) {
          auto startrelabel_dprv2= chrono::steady_clock::now();
          relabel(u, e, altura, d);
          auto endrelabel_dprv2= chrono::steady_clock::now();
          totrelabel_dprv2 += (endrelabel_dprv2 - startrelabel_dprv2).count();
        }
        
        
        // Push: ao encontrar um arco (u,v) em que v está um nível abaixo de u
        if (altura[u] == altura[e.destNode] + 1) {
          auto startpush_dprv2= chrono::steady_clock::now();
          pushFlow(Gf, u, &edgeList[i], excesso, altura, b, s, t);
          auto endpush_dprv2= chrono::steady_clock::now();
          totpush_dprv2 += (endpush_dprv2 - startpush_dprv2).count();
        }
        
      }
    }

    auto enddischarge_dprv2= chrono::steady_clock::now();
    totdischarge_dprv2 += (enddischarge_dprv2 - startdischarge_dprv2).count();
  }
}


int highest_label_push_relabel_max_flow_v2(VectorGraph &G, int s, int t) {
  auto startGf_dprv2= chrono::steady_clock::now();
  VectorGraph Gf(G);
  auto endGf_dprv2= chrono::steady_clock::now();
  totGf_dprv2 += (endGf_dprv2 - startGf_dprv2).count();

  int flow = 0;
  int d = 0;
  int n = G.num_vertices();
  vector<int> excesso(n + 1);  // armazena a quantidade de excesso presente em cada nó
  vector<int>  altura(n + 1);  // armazena o valor do rótulo de altura de cada nó
  vector<queue<int>> b(2 * n); // armazena na posição k do vetor, o rótulo dos nós ativos i cuja altura d(i) = k

  operations_highest_label_push_relabel_max_flow_v2::preflowInitialize(Gf, excesso, altura, b, s, t);
  
  while (d >= 0) {
    if (!b[d].empty()) {
      int u = b[d].front(); b[d].pop();
      operations_highest_label_push_relabel_max_flow_v2::discharge(u, Gf, excesso, altura, b, &d, s, t);
    } 
    else {
      d = d - 1;
    }
  }

  // Calcula o fluxo que sai fonte
  vector<vectorImplementation::edge> &edgeList = G.get_edge_list(s);
  for (int i = 0; i < edgeList.size(); i++) {
    vectorImplementation::edge *e_original = &edgeList[i];
    vectorImplementation::edge *e_residual = Gf.get_edge_pointer(s, e_original->destNode);
    flow += e_original->capacity - e_residual->capacity;
  }
  
  return flow;
}