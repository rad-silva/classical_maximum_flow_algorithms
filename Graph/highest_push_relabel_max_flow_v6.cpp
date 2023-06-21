// Uma implementação do algoritmo Highest Push-Relabel para o problema de fluxo máximo.
// Proposto por Ahuja e Orlin aprimorado por Cheriyan e Mehlhorn
// Tempo de pior caso: O(n^2 * sqrt(m))

// Sobre a versão 6:
//
// Nesta versão acrescentamos a heurística Global Relebeling que realiza um busca em
// largura a partir do terminal e atualiza o rótulo de altura de cada nó i que pode
// alcançar o terminal (utilizando arcos com capacidade residual positiva) para a
// distância real entre i e t


#include "graph.h"

#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>


class highest_push_relabel_max_flow_v6 {
  ListGraph Gf;
  int src, sink;
  int num_nodes;

  int d = 0;
  long max_flow = 0;
  long num_pushs = 0;
  long num_relabels = 0;
  long num_gaps = 0;
  long num_global_relabels = 0;
  long time_execution = 0;
  
  std::vector<int> node_excess;
  std::vector<int> node_height;
  std::vector<int> num_nodes_height;
  std::vector<std::queue<int>> bucket;

  void initializePreflow();
  void pushFlow(int u, LinkedListImplementation::edge *e, int flow);
  void relabel(int u);
  bool isAdmissible(int u, LinkedListImplementation::edge *e);
  void discharge(int u);
  bool gap(int k);
  void global_relabeling();

public:
  // ctor
  highest_push_relabel_max_flow_v6(ListGraph &G);

  int solve(int s, int t);

  int get_max_flow() { return max_flow; };
  long get_num_pushs() { return num_pushs; };
  long get_num_relabels() { return num_relabels; };
  long get_num_gaps() { return num_gaps; };
  long get_num_globals() { return num_global_relabels; };
  long get_time_execution() { return time_execution; };
};


highest_push_relabel_max_flow_v6::highest_push_relabel_max_flow_v6(ListGraph &G) 
  : Gf(G)
{
  num_nodes = Gf.num_vertices();
  node_excess.resize(num_nodes+1, 0);
  node_height.resize(num_nodes+1, 0);
  num_nodes_height.resize(num_nodes+1, 0);
  bucket.resize(2 * (num_nodes+1), std::queue<int>());
}


// Satura todos os arcos que saem da fonte no grafo Residual -> f(s,v) = u(s,v)
void highest_push_relabel_max_flow_v6::initializePreflow()
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
void highest_push_relabel_max_flow_v6::pushFlow(int u, LinkedListImplementation::edge *e, int flow)
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


// Se existir algum valor k < n tal que existam nós ativos i com rótulo de distância h(i) > k,
// mas nenhum nó j com rótulo de distância h(j) = k (includo os não ativos),
// então definimos h(i) = n para cada nó i com k < h(i) < n.
bool highest_push_relabel_max_flow_v6::gap(int k)
{
  if (num_nodes_height[k] == 0) {
    for (int u = 0; u < num_nodes + 1; u++) {
      if (node_height[u] > k and node_height[u] < num_nodes + 1) {
        // Atualiza a altura dos nós acima do gap
        num_nodes_height[node_height[u]]--;
        node_height[u] = num_nodes;
        num_nodes_height[node_height[u]]++;
      }
    }

    num_gaps++;
    return true;
  }

  return false;
}


// Realiza uma busca em largura a partir do terminal
// Seja v um nó já encontrado (um nó que alcança t). Percorre-se a lista de todos os 
// arcos que entram em v. Assim, para todo arco (u,v) com capacidade residual positiva,
// verifica se o nó u de origem desse arco ainda não foi encontrado, caso verdadeiro
// atualiza a altura de u para ser uma unidade maior que a altura de v.
void highest_push_relabel_max_flow_v6::global_relabeling()
{
  int u, v;
  LinkedListImplementation::edge *e;
  std::vector<std::queue<int>> new_bucket;
  new_bucket.resize(2 * (num_nodes+1), std::queue<int>());

  std::queue<int> fila;
  std::vector<bool> visited(num_nodes+1);

  for (int i = 0; i < num_nodes+1; i++) {
    visited[i] = false;
  }

  visited[sink] = true;
  fila.push(sink);
  
  while (!fila.empty()) {
    v = fila.front(); fila.pop();

    for (e = Gf.get_edge(v); e != nullptr; e = e->next) {
      u = e->destNode;

      if (visited[u] == false and e->reverse->capacity > 0) {
        visited[u] = true;
        fila.push(u);

        node_height[u] = node_height[v] + 1;
        
        if(node_height[u] > d) {
          d = node_height[u];
        }

        new_bucket[node_height[u]].push(u);
      }
    }
  }

  bucket = new_bucket;
  num_global_relabels++;
}



void highest_push_relabel_max_flow_v6::relabel(int u)
{
  num_nodes_height[node_height[u]]--;

  // Verifica se essa operação de relabel cria um Gap
  if (gap(node_height[u])) {
    node_height[u] = num_nodes;
  }

  // Percorre a lista de todos os arcos que saem de u:
  // Dentre os arcos (u,v) com capacidade residual positiva,
  // encontra o vértice v com menor rótulo de altura e atualiza
  // h(u) para h(v)+1, tornando (u,v) um arco admissível.
  else {
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
    
    if (num_relabels % num_nodes == 0) {
      global_relabeling();
    }
  }

  num_nodes_height[node_height[u]]++;
}


bool highest_push_relabel_max_flow_v6::isAdmissible(int u, LinkedListImplementation::edge *e)
{
  return (e->capacity > 0 and node_height[u] == node_height[e->destNode] + 1);
}


// Enquanto um nó u tiver excesso positivo realiza operações de push e de relabel
void highest_push_relabel_max_flow_v6::discharge(int u)
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


int highest_push_relabel_max_flow_v6::solve(int s, int t)
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