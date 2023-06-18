// Uma implementação do algoritmo Highest Push-Relabel para o problema de fluxo máximo.
// Proposto por 
// Tempo de pior caso: O(n^2 * sqrt(m))

// Sobre a versão:
// Incluímos a operação de discharge que sempre realiza pushs e relabels sobre
// o nó ativo com maior rótulo de altura até que seu excesso seja zerado.

#include "graph.h"

#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>


class highest_push_relabel_max_flow_v1 {
  ListGraph Gf;
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
  void pushFlow(int u, LinkedListImplementation::edge *e, int flow);
  void relabel(int u);
  bool isAdmissible(int u, LinkedListImplementation::edge *e);
  void discharge(int u);

public:
  // ctor
  highest_push_relabel_max_flow_v1(ListGraph &G);

  int solve(int s, int t);

  int get_max_flow() { return max_flow; };
  long get_num_pushs() { return num_pushs; };
  long get_num_relabels() { return num_relabels; };
  long get_time_execution() { return time_execution; };
};


highest_push_relabel_max_flow_v1::highest_push_relabel_max_flow_v1(ListGraph &G)
  : Gf(G)
{
  num_nodes = Gf.num_vertices();
  node_excess.resize(num_nodes+1, 0);
  node_height.resize(num_nodes+1, 0);
  bucket.resize(2 * (num_nodes+1), std::queue<int>());
}


void highest_push_relabel_max_flow_v1::initializePreflow()
{
  // satura todos os arcos que saem da fonte no grafo Original -> f(s,v) = u(s,v)
  for (LinkedListImplementation::edge *e = Gf.get_edge(src); e != NULL; e = e->next) {
    if (e->capacity > 0) {
      pushFlow(src, e, e->capacity);
    }
  }

  node_height[src] = num_nodes;
}


void highest_push_relabel_max_flow_v1::pushFlow(int u, LinkedListImplementation::edge *e, int flow)
{
  // Atualiza o fluxo e capacidade dos arcos (u,v) e (v,u) no grafo residual
  e->flow += flow;
  e->reverse->flow -= flow;

  e->capacity -= flow;
  e->reverse->capacity += flow;

  // Insere o nó que recebeu excesso na fila se ele é diferente de s e t, e não possuia excesso antes da operação
  if (node_excess[e->destNode] == 0 and (e->destNode != src and e->destNode != sink)) {
    bucket[node_height[e->destNode]].push(e->destNode);
  }

  // Atualiza o excesso de u e de v
  node_excess[u] -= flow;
  node_excess[e->destNode] += flow; 

  num_pushs++;
}


void highest_push_relabel_max_flow_v1::relabel(int u)
{
  // Para todos os arcos (u,v) com capacidade residual positiva,
  // encontra o vértice v com menor rótulo de altura e atualiza
  // a altura de u para que (u,v) se torne um arco admissível.

  int h_min = 2 * num_nodes;

  LinkedListImplementation::edge *e;
  for (e = Gf.get_edge(u); e != NULL; e = e->next) {
    if (e->capacity > 0) {
      if (node_height[e->destNode] < h_min) {
        h_min = node_height[e->destNode];
      }
    }
  }

  node_height[u] = h_min + 1;

  // Como no algoritmo highest label Push-Relabel pegamos sempre o nó com maior node_height, d* sempre aumenta ao fazer relabel
  if (node_height[u] > d) {
    d = node_height[u];
  }

  num_relabels++;
}


bool highest_push_relabel_max_flow_v1::isAdmissible(int u, LinkedListImplementation::edge *e)
{
  return (e->capacity > 0 and node_height[u] == node_height[e->destNode] + 1);
}


void highest_push_relabel_max_flow_v1::discharge(int u)
{
  LinkedListImplementation::edge *e;

  while (node_excess[u] > 0) {
    for (e = Gf.get_edge(u); node_excess[u] > 0 and e != NULL; e = e->next) {
      if (isAdmissible(u, e)) {
        int delta = std::min(node_excess[u], e->capacity);
        pushFlow(u, e, delta);
      }
    }

    if (node_excess[u] > 0) {
      relabel(u);
    }
  }
}


int highest_push_relabel_max_flow_v1::solve(int s, int t)
{
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
  LinkedListImplementation::edge *e;
  for (e = Gf.get_edge(src); e != NULL; e = e->next) {
    max_flow += e->flow;
  }

  std::chrono::time_point end = std::chrono::steady_clock::now();
  time_execution = (end - start).count();

  return max_flow;
}