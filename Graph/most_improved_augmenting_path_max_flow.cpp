// Uma implementação do algoritmo de caminho aumentador mais curto
// Proposto por Edmonds-Karp
// Tempo de pior caso: O(m * log(mU) * (m + n log(n)))

// Ideia geral: Enquanto existir, procura um (s-t)-caminho simples cuja capacidade
// residual mínima do caminho seja a maior possível e empurra fluxo ao longo dele

#include <algorithm>

#include "graph.h"
#include "./utils/heapNode.cpp"
#include "./utils/wcp.cpp"


int most_improved_augmenting_path_max_flow(ListGraph &Gf, int s, int t, int *numPaths) {
  int num_nodes = Gf.num_vertices();
  int parent[num_nodes + 1];
  int u, v, capacity_path = 0;
  int flow = 0;


  // Procura um (s,t)-caminho mais largo e empurra fluxo enquanto existir
  while (widest_capacity_path(Gf, s, t, parent, &capacity_path)) {

    // Atualiza as capacidades residuais dos arcos (u,v) que compõem
    // o caminho encontrado e seus respectivos arcos reversos (v,u)
    for (v = t; v != s; v = parent[v]) {
      u = parent[v];

      Gf.add_capacity(u, v, -capacity_path);
      Gf.add_capacity(v, u, +capacity_path);
    }

    // Atualiza o valor do fluxo enviado e o númerod e caminhos encontrados
    flow += capacity_path;
    *numPaths += 1;
  }

  return flow;
}