// Uma implementação do algoritmo Highest Push-Relabel para o problema de fluxo máximo.
// Proposto por 
// Tempo de pior caso: O(n^2 * sqrt(m))

// Sobre a versão 4:
// Nesta versão a definição de nó ativo deixa de ser exclusivamente relacionado ao excesso
// de um nó. Agora um nó é ativo se ef(i) > 0 e h(i) >= n para todo i /in V - {s, t}
// Nós com altura maior que n não conseguem alncaçar o terminal, e portanto não podem
// contribuir para o aumento de fluxo recebido por ele.


#include "graph.h"

#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>


class highest_push_relabel_max_flow_v4 {
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
  highest_push_relabel_max_flow_v4(ListGraph &G);

  int solve(int s, int t);

  int get_max_flow() { return max_flow; };
  long get_num_pushs() { return num_pushs; };
  long get_num_relabels() { return num_relabels; };
  long get_time_execution() { return time_execution; };
};


highest_push_relabel_max_flow_v4::highest_push_relabel_max_flow_v4(ListGraph &G) 
  : Gf(G), num_nodes(Gf.num_vertices())
{
  num_nodes = Gf.num_vertices();
  node_excess.resize(num_nodes+1, 0);
  node_height.resize(2*(num_nodes+1), 0);
  bucket.resize(2*(num_nodes+1), std::queue<int>());
}


// Satura todos os arcos que saem da fonte no grafo Residual -> f(s,v) = u(s,v)
void highest_push_relabel_max_flow_v4::initializePreflow()
{
  for (LinkedListImplementation::edge *e = Gf.get_edge(src); e != NULL; e = e->next) {
    if (e->capacity > 0) {
      pushFlow(src, e, e->capacity);
    }
  }

  node_height[src] = num_nodes;
}


// Empurra excesso contido no nó u para o nó v
// Note: flow deve ser igual a min(node_excess[u], e->capacity);
void highest_push_relabel_max_flow_v4::pushFlow(int u, LinkedListImplementation::edge *e, int flow)
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


// Percorre a lista de todos os arcos que saem de u:
// Dentre os arcos (u,v) com capacidade residual positiva,
// encontra o vértice v com menor rótulo de altura e atualiza
// h(u) para h(v)+1, tornando (u,v) um arco admissível.
void highest_push_relabel_max_flow_v4::relabel(int u)
{
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

  // Como no algoritmo highest label Push-Relabel pegamos sempre
  // o nó com maior node_height, d* sempre aumenta ao fazer relabel
  if (node_height[u] > d) {
    d = node_height[u];
  }

  num_relabels++;
}


bool highest_push_relabel_max_flow_v4::isAdmissible(int u, LinkedListImplementation::edge *e)
{
  return (e->capacity > 0 and node_height[u] == node_height[e->destNode] + 1);
}


// Enquanto um nó u tiver excesso positivo realiza operações de push e de relabel
void highest_push_relabel_max_flow_v4::discharge(int u)
{
  LinkedListImplementation::edge *e;

  while (node_excess[u] > 0 and node_height[u] < num_nodes) {
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


int highest_push_relabel_max_flow_v4::solve(int s, int t)
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

  //
  max_flow = node_excess[t];

  std::chrono::time_point end = std::chrono::steady_clock::now();
  time_execution = (end - start).count();

  return max_flow;
}