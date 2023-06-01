#include "graph.cpp"
#include "./dependencies/heapNode.cpp"

#include <algorithm>
#include <iostream>

using namespace std;

auto startGf_bap = chrono::steady_clock::now();
auto endGf_bap = chrono::steady_clock::now();
auto totGf_bap = (p - p).count();

auto startWCP_bap = chrono::steady_clock::now();
auto endWCP_bap = chrono::steady_clock::now();
auto totWCP_bap = (p - p).count();

auto startEmpurraflowGf_bap = chrono::steady_clock::now();
auto endEmpurraflowGf_bap = chrono::steady_clock::now();
auto totEmpurraflowGf_bap = (p - p).count();

/* Algoritmo de Edmonds-Karp: Most Improved Augmenting Path */

int widest_capacity_path(ListGraph &Gf, int s, int t, int parent[]) {
  startWCP_bap = chrono::steady_clock::now();

  Node u, v, no;
  int delta;
  int num_vertices = Gf.num_vertices();

  int capacity[num_vertices + 1] = {};
  int visited[num_vertices + 1] = {};
  BinaryHeap pq(num_vertices + 1);

  for (int i = 0; i <= num_vertices; i++) {
    parent[i] = -1;
  }

  // Inicializa a capacidade de todos os (s,u)-caminhos como -inf
  // Exceto para o caso (s,s)-caminho que possui capacidade ilimitada
  for (int u = 1; u <= num_vertices; u++) {
    capacity[u] = INT_MIN;
  }
  capacity[s] = INT_MAX;

  // Insere s na fila de nós encontrados
  no.id = s;
  no.priority = capacity[s];
  pq.insert(no);

  parent[s] = -1;

  
  while (!pq.empty()) {

    // remove o nó u que possui a maior capacidade de (s,u)-caminho e marca como visitado
    u = pq.extractMax(); 
    visited[u.id] = 1;

    // Se u for igual a t, então t foi visitado e atribuída sua maior capacidade de (s-t)-caminho
    // Armazena na posição 0 do vetor parent o valor da capacidade caminho até t
    if (u.id == t) {
      parent[0] = u.priority;
      break;
    }

    // Se não:
    LinkedListImplementation::edge *aux = Gf.get_edge(u.id);

    /* para todos os nos v alcançaveis a partir de u
    verifica a capacidade do caminho de s até u e o arco (u,v).
    Se a capacidade desse caminho até v utilizando o arco (u,v)
    for melhor que a capacidade do caminho atual até v, 
    então v recebe o novo parent e nova capacidade              */
    while (aux != NULL) {

      if (aux->capacity > 0) {
        delta = min(u.priority, aux->capacity);

        if (delta > capacity[aux->destNode] and visited[aux->destNode] != 1) {
          parent[aux->destNode] = u.id;
          capacity[aux->destNode] = delta;

          v.id = aux->destNode;
          v.priority = delta;

          pq.insert(v);
        }
      }

      aux = aux->next;
    }
  }

  endWCP_bap = chrono::steady_clock::now();
  totWCP_bap += (endWCP_bap - startWCP_bap).count();
  // retorna uma confirmação de que t foi alcançado
  return (parent[t] != -1);
}

int most_improved_augmenting_path_max_flow(ListGraph &G, int s, int t, int *numPaths) {
  
  int u, v, capacity_path = 0, flow = 0;
  int parent[G.num_vertices() + 1];

  startGf_bap = chrono::steady_clock::now();
  ListGraph Gf(G);
  endGf_bap = chrono::steady_clock::now();
  totGf_bap += (endGf_bap - startGf_bap).count();

  // Empurra fluxo enquanto existir um (s,t)-caminho
  while (widest_capacity_path(Gf, s, t, parent)) {
    
    // capacity_path = INT_MAX;
    // Encontra a capacidade residual mínima do caminho encontrado
    // for (v = t; v != s; v = parent[v]) {
    //   u = parent[v];
    //   capacity_path = min(capacity_path, Gf.getCapacityEdge(u, v));
    // }

    // Define a capacidade residual mínima do caminho encontrado
    capacity_path = parent[0];

    startEmpurraflowGf_bap = chrono::steady_clock::now();

    for (v = t; v != s; v = parent[v]) {
      u = parent[v];
      Gf.add_capacity(u, v, -capacity_path);
      Gf.add_capacity(v, u, +capacity_path);
    }

    endEmpurraflowGf_bap = chrono::steady_clock::now();
    totEmpurraflowGf_bap += (endEmpurraflowGf_bap - startEmpurraflowGf_bap).count();

    flow += capacity_path;
    *numPaths += 1;
  }

  return flow;
}