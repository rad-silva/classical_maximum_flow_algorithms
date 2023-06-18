// Uma implementação do algoritmo Highest Push-Relabel para o problema de fluxo máximo.
// Proposto por Goldberg e Tarjan
// Tempo de pior caso: O(n^2 * sqrt(m))

// Sobre a versão:
// Incluímos a operação de discharge que sempre realiza pushs e relabels sobre
// o nó ativo com maior rótulo de altura até que seu excesso seja zerado.

#include "graph.h"
#include "./utils/heapNode.cpp"

#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>


class push_relabel_max_flow {
  ListGraph Gf;
  BinaryHeap pq;
  int src, sink;
  int num_nodes;

  int max_flow = 0;
  long num_pushs = 0;
  long num_relabels = 0;
  long time_execution = 0;
  
  std::vector<int> node_excess;
  std::vector<int> node_height;

  void initializePreflow();
  void pushFlow(int u, LinkedListImplementation::edge *e, int flow);
  void relabel(int u);
  LinkedListImplementation::edge *existAdmissibleEdge(int u);

public:
  // ctor
  push_relabel_max_flow(ListGraph &G);

  int solve(int s, int t);

  int get_max_flow() { return max_flow; };
  long get_num_pushs() { return num_pushs; };
  long get_num_relabels() { return num_relabels; };
  long get_time_execution() { return time_execution; };
};


push_relabel_max_flow::push_relabel_max_flow(ListGraph &G)
  : Gf(G), pq(G.num_vertices()+1) 
{
  num_nodes = Gf.num_vertices();
  node_excess.resize(num_nodes+1, 0);
  node_height.resize(2*(num_nodes+1), 0);
}


// satura todos os arcos que saem da fonte no grafo Original -> f(s,v) = u(s,v)
void push_relabel_max_flow::initializePreflow()
{
  for (LinkedListImplementation::edge *e = Gf.get_edge(src); e != NULL; e = e->next) {
    if (e->capacity > 0) {
      pushFlow(src, e, e->capacity);
    }
  }
  
  node_height[src] = num_nodes;

  return;
}


void push_relabel_max_flow::pushFlow(int u, LinkedListImplementation::edge *e, int flow)
{
  // Atualiza o fluxo e capacidade dos arcos (u,v) e (v,u) no grafo residual
  e->flow += flow;
  e->reverse->flow -= flow;

  e->capacity -= flow;
  e->reverse->capacity += flow;

  // Insere o nó que recebeu excesso na fila se ele é diferente de s e t, e não possuia excesso antes da operação
  if (node_excess[e->destNode] == 0 and (e->destNode != src and e->destNode != sink)) {
    pq.insert( Node {e->destNode, node_height[e->destNode]} );
  }

  // Atualiza o excesso de u e de v
  node_excess[u] -= flow;
  node_excess[e->destNode] += flow;

  // Se u perde todo seu excesso ele é removido da lista
  if (node_excess[u] == 0) {
    pq.extractMax();
  }

  num_pushs++;
}


void push_relabel_max_flow::relabel(int u) {
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

  return;
}


LinkedListImplementation::edge * push_relabel_max_flow::existAdmissibleEdge(int u) {

  // busca um arco admissível saindo de u
  LinkedListImplementation::edge *e;
  for (e = Gf.get_edge(u); e != NULL; e = e->next) {
    if (node_height[u] == node_height[e->destNode] + 1 and e->capacity > 0) {
      return e; // retorna o destino do arco admissível
    }
  }

  return NULL; // retorna indicador de que não existe arco admssível com origem em u
}



int push_relabel_max_flow::solve(int s, int t) {
  std::chrono::time_point start = std::chrono::steady_clock::now();

  src = s;
  sink = t;

  initializePreflow();

  // Empura fluxo nos arcos admissíveis enquanto existir um vértice com execesso positivo
  // Vértice com execesso positivo == ter um vértice na fila de prioridades
  while (!pq.empty()) {
    
    Node u = pq.getMax();
    LinkedListImplementation::edge *e = existAdmissibleEdge(u.id);
   
    if (e != NULL) {
      pushFlow(u.id, e, e->capacity);
    } 
    else {
      relabel(u.id);

      u.priority = node_height[u.id];

      pq.changePriority(pq.getIndexNode(u.id), u);
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



// /* Algoritmo de Goldberg e Tarjan: Push Relabel */

// void preflowInitialize(ListGraph &G, ListGraph &Gf, int altura[], int excesso[], BinaryHeap &pq, int s, int t) {
  
  
// }

// void PushFlow(ListGraph &G, ListGraph &Gf, int u, LinkedListImplementation::edge *aux, \
// int altura[], int excesso[], BinaryHeap &pq, int s, int t) {
  
//   // Calcula a quantidade de excesso que é possível enviar
//   int delta = std::min(excesso[u], aux->capacity);
  
//   // G.addFlowEdge(u, aux->destNode, +delta);
//   // G.addFlowEdge(aux->destNode, u, -delta);
  
//   // Atualiza a capacidade dos arcos (u,v) e (v,u) no grafo residual
//   aux->capacity -= delta;
//   Gf.add_capacity(aux->destNode, u, delta);

//   // Insere o nó que recebeu excesso na fila se ele é diferente de s e t, e não possuia excesso antes da operação
//   if (excesso[aux->destNode] == 0 and (aux->destNode != s and aux->destNode != t)) {
//     pq.insert( Node {aux->destNode, altura[aux->destNode]} );
//   }

//   // Atualiza o excesso de u
//   excesso[u] -= delta;

//   // Se u perde todo seu excesso ele é removido da lista
//   if (excesso[u] == 0) {
//     pq.extractMax();
    
//   }
  
//   // Atualiza o excesso v somente se v != s,t
//   if (aux->destNode != s and aux->destNode != t) {
//     excesso[aux->destNode] += delta;
//   }

//   return;
// }

// void Relabel(ListGraph &Gf, int u, int altura[]) {
//   // Para todos os arcos (u,v) com capacidade residual positiva,
//   // encontra o vértice v com menor rótulo de altura e atualiza
//   // a altura de u para (u,v) se torne um arco admissível.
  
//   int h_min = INT_MAX;
//   for (LinkedListImplementation::edge *aux = Gf.get_edge(u); aux != NULL; aux = aux->next) {
//     if (aux->capacity > 0) {
//       h_min = std::min(h_min, altura[aux->destNode] + 1);
//     }
//   }

//   altura[u] = h_min;

//   return;
// }




// int push_relabel_max_flow(ListGraph &G, int s, int t) {
//   int flow = 0;
//   int len = G.num_vertices() + 1;
//   int excesso[len] = {}; // armazena o excesso presente em cada nó
//   int altura[len] = {};  // armazena o rótulo de altura de cada nó

//   ListGraph Gf(G);           // Cria um grafo residual a partir de G
//   BinaryHeap pq(len);    // fila de priorides que armazena altura de nós com excesso positivo

//   preflowInitialize(G, Gf, altura, excesso, pq, s, t);

//   // Empura fluxo nos arcos admissíveis enquanto existir um vértice com execesso positivo
//   // Vértice com execesso positivo == ter um vértice na fila de prioridades
//   while (!pq.empty()) {
    
//     Node u = pq.getMax();
//     LinkedListImplementation::edge *aux = existAdmissibleEdge(Gf, u.id, altura);
   
//     if (aux != NULL) {

//       PushFlow(G, Gf, u.id, aux, altura, excesso, pq, s, t);
//     } 
//     else {
//       Relabel(Gf, u.id, altura);

//       u.priority = altura[u.id];
      
      
//       pq.changePriority(pq.getIndexNode(u.id), u);
//     }
//   }

//   // Calcula o fluxo que sai fonte
//   for (LinkedListImplementation::edge *aux = G.get_edge(s); aux != NULL; aux = aux->next) {
//     LinkedListImplementation::edge *auxResidual = Gf.get_edge(s, aux->destNode);
//     flow += aux->capacity - auxResidual->capacity;
//   }
  
//   return flow;
// }