#pragma once

#include "../graph.h"
#include "heapNode.cpp"
#include <queue>
#include <chrono>

int widest_capacity_path(ListGraph &Gf, int s, int t, int parent[], int *capacity_path) {

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
      *capacity_path = u.priority;
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
        delta = std::min(u.priority, aux->capacity);

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

  // retorna uma confirmação de que t foi alcançado
  return (parent[t] != -1);
}