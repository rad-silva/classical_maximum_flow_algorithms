
/*#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <fstream> 
#include <limits.h>
#include <string.h>
#include <vector>
#include <chrono>

using namespace std;

auto p = chrono::steady_clock::now();

// Grafo implementado no formato lista de adjascência:
// - Um vector que representa o conjuto de nós do grafo
// - Cada nó (posição do vector) armazena um ponteiro para o primeiro edge
//      da lista encadeada de out edges referente a ele
namespace LinkedListImplementation {
  struct edge {
    int destNode, flow = 0, capacity;
    edge *next;
  };
}

class ListGraph {
  int m = 0, n = 0;
  LinkedListImplementation::edge **G; 

public:
  ListGraph(int numV);
  ListGraph(ListGraph &G_original);
  ListGraph(ListGraph &G_original, int delta);
  ~ListGraph();

  void add_edge(int u, int v, int capacity);
  void add_flow(int u, int v, int flow);
  void add_capacity(int u, int v, int capacityEdit);
  void zera_flow();

  LinkedListImplementation::edge *get_edge(int u);
  LinkedListImplementation::edge *get_edge(int u, int v);

  int num_vertices();
  int num_edges();
  int get_capacity(int u, int v);
  
  void print_grafo();
};



// Grafo implementado no formato semi-lista de adjascência:
// - Um vector que representa o conjuto de nós do grafo
// - Cada nó (posição do vector) armazena um vector contendo os out edges referente a ele
namespace vectorImplementation {
  struct edge {
    int destNode = -1;
    int flow = 0;
    int capacity = 0;
  };
}

class VectorGraph {
  int m = 0, n = 0;
  vector<vector<vectorImplementation::edge>> G;

public:
  VectorGraph(int numV);
  VectorGraph(VectorGraph &originalGraph);
  ~VectorGraph();

  void add_edge(int u, int v, int flow, int capacity);
  void add_flow(int u, int v, int pushFlow);
  void add_capacity(int u, int v, int capacityEdit);

  vectorImplementation::edge *get_edge_pointer(int u, int v);
  vector<vectorImplementation::edge> & get_edge_list(int u);

  int num_vertices();
  int num_edges();
  
  void print_statistics();
  void print_grafo();
};

#endif // GRAPH_HPP*/