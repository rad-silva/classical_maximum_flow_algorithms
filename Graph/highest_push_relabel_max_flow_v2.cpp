// Uma implementação do algoritmo Highest Push-Relabel para o problema de fluxo máximo.
// Proposto por 
// Tempo de pior caso: O(n^2 * sqrt(m))

// Sobre a versão:
// Adaptamos a forma de arazenamento de arcos do grafo para que possamos
// realizar a operação de discharge de um modo não muito convencional.
// Trocamos as listas de adjascência por um vetor de adjascência para cada nó
// (difere da matriz de adjascência, uma posição no vetor não corresponde ao nó de destino)
// Sempre que realizamos a operação de discharge sobre um nó u, ordenamos o vetor de adjascência de
// Essa ideia visto que o grau média de adjascência por nó eram na média de 5 arcos


#include <queue>
#include <vector>
#include <algorithm>

#include "graph.h"
#include "./utils/insertionSort.cpp"

class highest_push_relabel_max_flow_v2 {
  VectorGraph Gf;
  int src, sink;
  int num_nodes;

  int d = 0;
  int max_flow = 0;
  long num_pushs = 0;
  long num_relabels = 0;
  long time_execution = 0;
  
  std::vector<int> node_excess;
  std::vector<int> node_height;
  std::vector<std::queue<int>> bucket;

  void initializePreflow();
  void pushFlow(int u, vectorImplementation::edge *e, int flow);
  void relabel(int u, vectorImplementation::edge e);
  void discharge(int u);

public:
  // ctor
  highest_push_relabel_max_flow_v2(VectorGraph &G);

  int solve(int s, int t);

  int get_max_flow() { return max_flow; };
  long get_num_pushs() { return num_pushs; };
  long get_num_relabels() { return num_relabels; };
  long get_time_execution() { return time_execution; };
};


highest_push_relabel_max_flow_v2::highest_push_relabel_max_flow_v2(VectorGraph &G) : Gf(G) {
  num_nodes = Gf.num_vertices();
  node_excess.resize(num_nodes+1, 0);
  node_height.resize(2*(num_nodes+1), 0);
  bucket.resize(2*(num_nodes+1), std::queue<int>());
}


void highest_push_relabel_max_flow_v2::initializePreflow() {
  std::vector<vectorImplementation::edge> &edgeList = Gf.get_edge_list(src);
  int sizeList = edgeList.size();
    
  // satura todos os arcos que saem da fonte no grafo Original -> f(s,v) = u(s,v)
  for (int i = 0; i < sizeList; i++) {
    if (edgeList[i].capacity > 0){
      pushFlow(src, &edgeList[i], edgeList[i].capacity);
    }
  }

  node_height[src] = num_nodes;
}


void highest_push_relabel_max_flow_v2::pushFlow(int u, vectorImplementation::edge *e, int flow) {
  // Atualiza o fluxo e capacidade dos arcos (u,v) e (v,u) no grafo residual
  e->flow = e->flow + flow;
  e->capacity = e->capacity - flow;

  Gf.update_flow_capacity(e->destNode, u, -flow);


  // Insere o nó que recebeu excesso na fila se ele é diferente de s e t, e não possuia excesso antes da operação
  if (node_excess[e->destNode] == 0 and (e->destNode != src and e->destNode != sink)) {
    bucket[node_height[e->destNode]].push(e->destNode);
  }

  // Atualiza o excesso de u e de v
  node_excess[u] -= flow;
  node_excess[e->destNode] += flow; 

  num_pushs++;
}


void highest_push_relabel_max_flow_v2::relabel(int u, vectorImplementation::edge e) {
  node_height[u] = node_height[e.destNode] + 1;
    
  if (node_height[u] > d) {
    d = node_height[u];
  }

  num_relabels++;
}


void highest_push_relabel_max_flow_v2::discharge(int u) {

  // Obtém a lista de arcos que saem de u
  std::vector<vectorImplementation::edge> &edgeList = Gf.get_edge_list(u);

  // Ordena a lista de arcos que saem de u pela altura de cada destNode
  insertionSort(edgeList, node_height);

  // Percorre a lista de arcos que saem de u
  for (int i = 0; node_excess[u] > 0 and i < edgeList.size(); i++) {
    vectorImplementation::edge e = edgeList[i];

    if (e.capacity > 0) {
      // Relabel: ao encontrar um nó com node_height igual ou maior que u, atualiza a node_height de u
      // e faz com que a próxima iteração inicie nesse arco novamente
      if (node_height[e.destNode] >= node_height[u]) {
        relabel(u, e);
      }
      
      // Push: ao encontrar um arco (u,v) em que v está um nível abaixo de u
      if (node_height[u] == node_height[e.destNode] + 1) {
        int delta = std::min(node_excess[u], e.capacity);
        pushFlow(u, &edgeList[i], delta);
      }
    }
  }
}


int highest_push_relabel_max_flow_v2::solve(int s, int t) {
  std::chrono::time_point start = std::chrono::steady_clock::now();

  src = s;
  sink = t;
  d = 0;

  initializePreflow();

  while (d >= 0) {
    if (!bucket[d].empty()) {
      int u = bucket[d].front(); bucket[d].pop();
      discharge(u);
    } 
    else {
      d = d - 1;
    }
  }

  // Calcula o fluxo que sai fonte
  std::vector<vectorImplementation::edge> &edgeList = Gf.get_edge_list(src);
  
  for (int i = 0; i < edgeList.size(); i++) {
    max_flow += edgeList[i].flow;
  }

  std::chrono::time_point end = std::chrono::steady_clock::now();
  time_execution = (end - start).count();

  return max_flow;
}