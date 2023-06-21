#pragma once

#include "../graph.h"
#include <queue>


bool bfs(ListGraph &Gf, int s, int t, int parent[]) {
  int u, v;
  std::queue <int> fila;
  bool visited[Gf.num_vertices() + 1] {};

  for (int i = 0; i <= Gf.num_vertices(); i++) {
    parent[i] = -1;
  }

  fila.push(s);
  visited[s] = true;

  while (!fila.empty()) {
    u = fila.front(); fila.pop();
    LinkedListImplementation::edge *aux = Gf.get_edge(u);

    while (aux != NULL) {
      v = aux->destNode;

      if (visited[v] == false && aux->capacity > 0) {
        fila.push(v);
        parent[v] = u;
        visited[v] = true;
    
        if (v == t) {

          return true;  // retorna um indicador de que t foi alcançado
        }
      }

      aux = aux->next;
    }
  }

  return false; // retorna um indicador de que t não foi alcançado
}