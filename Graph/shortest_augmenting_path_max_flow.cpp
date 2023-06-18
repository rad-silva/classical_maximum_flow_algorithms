// Uma implementação do algoritmo de caminho aumentador mais curto
// Proposto por Edmonds-Karp
// Tempo de pior caso: O(m^2 * n)

// Ideia geral: Enquanto existir, procura um (s-t)-caminho simples mais curto
// (menor quantidade de arcos) e empurra fluxo ao longo desse caminho

#include "graph.h"
#include "./utils/bfs.cpp"

#include <algorithm>
#include <limits>
#include <chrono>

int shortest_augmenting_path_max_flow(ListGraph &Gf, int s, int t, int *numPaths) {
  int num_nodes = Gf.num_vertices();
  int u, v, flow = 0;
  int parent[num_nodes+1];

  // Empurra fluxo enquanto existir um (s,t)-caminho simples
  while (bfs(Gf, s, t, parent)) {

    // Encontra a capacidade residual mínima do caminho encontrado
    int path_capacity = INT_MAX;

    for (v = t; v != s; v = parent[v]) {
      u = parent[v];
      path_capacity = std::min(path_capacity, Gf.get_capacity(u, v));
    }

    // Atualiza as capacidades residuais dos arcos (u,v) que compõem
    // o caminho encontrado e seus respectivos arcos reversos (v,u)
    for (v = t; v != s; v = parent[v]) {
      u = parent[v];

      Gf.add_capacity(u, v, -path_capacity);  // arco direto
      Gf.add_capacity(v, u, +path_capacity);  // arco reverso
    }

    // Atualiza o valor do fluxo enviado e o número de caminhos encontrados
    flow += path_capacity;
    *numPaths += 1;
  }
  
  return flow;
}