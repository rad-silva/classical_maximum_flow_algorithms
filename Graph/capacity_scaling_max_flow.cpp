// Uma implementação do algoritmo Capacity Scaling
// Proposto por Ahuja e Orlin
// Tempo de pior caso: O(m^2 * log(U))

// Ideia geral: Procura o arco de maior capacidade [U = max_u(i,j)) para todo (i,j) \in Gf]
// no grafo recebido e cria um grafo residual GfΔ em função do parâmentro Δ = U que contém
// apenas o arcos com uf(i,j) = Δ.
// Enquanto existir, procura um (s-t)-caminho simples no grafo residual Δ e empurra fluxo.
// Quando não existirem mais caminhos em GfΔ, atualiza o parâmetro Δ para a metade do seu valor 
// e repete o processo de encontrar caminhos e enviar fluxo até que Δ = 1 e não haja mais (s-t)-caminhos.


#include <math.h>
#include <algorithm>

#include "graph.h"
#include "./utils/bfs.cpp"


int capacity_scaling_max_flow(ListGraph &Gf, int s, int t, int *numPaths) {
  int num_nodes = Gf.num_vertices();
  int parent[num_nodes + 1];
  int u, v, flow = 0;
  int max_capacity = 0;
  
  // Obtém a maior capacidade dentre os arcos de Gf
  LinkedListImplementation::edge *e;

  for (u = 1; u < num_nodes; u++) {
    for (e = Gf.get_edge(u); e != NULL; e = e->next) {
      max_capacity = std::max(max_capacity, e->capacity);
    }
  }

  int delta = pow(2, int(log(max_capacity) / log(2)));

  while (delta >= 1) {
    ListGraph Gf_delta(Gf, delta);
  
    // Procura e empurra fluxo enquanto existir um (s,t)-caminhosimples no Grafo residual Delta
    while (bfs(Gf_delta, s, t, parent)) {

      int path_capacity = INT_MAX;

      // Obtem a capacidade residual mínima do caminho encontrado
      for (v = t; v != s; v = parent[v]) {
        u = parent[v];
        path_capacity = std::min(path_capacity, Gf.get_capacity(u, v));
      }
     
      // Atualiza as capacidades residuais dos arcos de Gf_delta e Gf
      for (v = t; v != s; v = parent[v]) {
        u = parent[v];
        
        Gf_delta.add_capacity(u, v, -path_capacity);
        Gf_delta.add_capacity(v, u, +path_capacity);

        Gf.add_capacity(u, v, -path_capacity);
        Gf.add_capacity(v, u, +path_capacity);
      }
     
      // Atualiza o valor do fluxo enviado e o número de caminhos encontrados
      flow += path_capacity;
      *numPaths += 1;
    }

    delta = int(delta / 2);
  }

  return flow;
}