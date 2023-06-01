#pragma once

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

// Inicializa um grafo sem arcos
ListGraph::ListGraph(int numV) {
  n = numV;         
  G = new LinkedListImplementation::edge*[n + 1]; 

  for (int u = 0; u <= n; u++) {
    G[u] = NULL;
  }
}

// Constroi um Grafo residual a partir de um grafo original
ListGraph::ListGraph(ListGraph &G_original) {
  this->n = G_original.num_vertices();
  this->G = new LinkedListImplementation::edge*[this->n + 1];

  for (int u = 0; u <= this->n; u++) {
    this->G[u] = NULL;
  }

  // Insere os arcos diretos presentes no grafo original
  // e os arcos reversos de capacidade zero caso não exista no grafo original
  for (int u = 0; u <= this->n; u++) {
    LinkedListImplementation::edge *e = G_original.get_edge(u);

    while (e != NULL) {
      add_edge(u, e->destNode, e->capacity);    // direct edge

      if (G_original.get_edge(e->destNode, u) == NULL) {
        add_edge(e->destNode, u, 0);            // reverse edge
      }
      
      e = e->next;
    }
  }
}

// Constroi um grafo residual delta a partir de um grafo residual
// utilizando apenas os arcos de capacidade residual maior que delta 
ListGraph::ListGraph(ListGraph &G_residual, int delta) {
  this->n = G_residual.num_vertices();
  this->G = new LinkedListImplementation::edge*[this->n + 1];

  if (delta > 1) {
    // Insere os arcos no grafo residual delta
    for (int u = 0; u <= this->n; u++) {
      this->G[u] = NULL;
      LinkedListImplementation::edge *e = G_residual.get_edge(u);

      while (e != NULL) {
        if (e->capacity >= delta) {
          add_edge(u, e->destNode, e->capacity); // direct edge

          LinkedListImplementation::edge *e_reverse = G_residual.get_edge(e->destNode, u);
          if (e_reverse->capacity < delta) {
            add_edge(e->destNode, u, 0);           // reverse edge
          }
        }
                    
        e = e->next;
      }
    }

  } else { // Gf(delta) = Gf

    for (int u = 0; u <= this->n; u++) {
      this->G[u] = NULL;
      LinkedListImplementation::edge *e = G_residual.get_edge(u);

      while (e != NULL) {
        add_edge(u, e->destNode, e->capacity); // direct edge         
        e = e->next;
      }
    }
  }
}


// Adiciona um arco com origem u, destino v, e capacidade capacity
void ListGraph::add_edge(int u, int v, int capacity) {
  LinkedListImplementation::edge *newEdge = new LinkedListImplementation::edge;
  newEdge->destNode = v;
  newEdge->capacity = capacity;
  newEdge->next = G[u];

  G[u] = newEdge;
  m++;
}

// Acrescenta capacityEdit unidades na capacidade atual do arco (u,v)
void ListGraph::add_capacity(int u, int v, int capacityEdit) {
  LinkedListImplementation::edge *e = get_edge(u, v);
  
  if (e != NULL) {
    e->capacity += capacityEdit;
  }
}

// Acrescenta pushFlow unidades no fluxo atual do arco (u,v)
void ListGraph::add_flow(int u, int v, int pushFlow) {
  LinkedListImplementation::edge *e = get_edge(u, v);

  if (e != NULL and e->flow + pushFlow <= e->capacity) {
    e->flow += pushFlow;
  }
}

// Percorre o grafo definindo todo fluxo como zero
void ListGraph::zera_flow() {

  for (int u = 0; u <= n; u++) {
    LinkedListImplementation::edge *e = get_edge(u);
    
    while (e != NULL) {
      e->flow = 0;
      e = e->next;
    }
  }
}

// Retorna um apontador para o primeiro arco saindo de u
LinkedListImplementation::edge * ListGraph::get_edge(int u) { return G[u]; }

// Retorna um apontador para o arco (u,v), caso exista, ou para NULL
LinkedListImplementation::edge * ListGraph::get_edge(int u, int v) {
  LinkedListImplementation::edge *e = G[u];

  while (e != NULL) {
    if(e->destNode == v)
      return e;
    e = e->next;
  }

  return NULL;
}


// Retorna o valor da capacidade do arco (u,v)
int ListGraph::get_capacity(int u, int v) {
  LinkedListImplementation::edge *e = get_edge(u, v);
  
  if (e != NULL) {
    return (e->capacity);
  }

  return -1;
}

// Retorna o número de vértices contido no grafo
int ListGraph::num_vertices() { return this->n; }

// Retorna o número de Arcos contido no grafo
int ListGraph::num_edges() { return this->m; }


void ListGraph::print_grafo() {
  for (int u = 0; u <= n; u++) {
    LinkedListImplementation::edge *e = G[u];

    while (e != NULL) {
      cout << "a " << u << " " << e->destNode << " " << e->flow << " " << e->capacity << endl;
      e = e->next;
    }
  }
}

ListGraph::~ListGraph() {
  if (G != NULL) {
    for (int u = 0; u <= n; u++) {
      LinkedListImplementation::edge *e_temp = G[u];

      while (e_temp != NULL) {
        LinkedListImplementation::edge *prox = e_temp->next;
        delete e_temp;
        e_temp = prox;
      }
    }
    
    delete[] G;
  }
}


/***************************************************************************************************************/
/***************************************************************************************************************/
/***************************************************************************************************************/


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


// Constroi um grafo a partir de um arquivo de texto no formato
VectorGraph::VectorGraph(int numV) {
  n = numV;
  G = vector<vector<vectorImplementation::edge>> (n + 1);
}


// Constroi um Grafo residual a partir de um grafo original
VectorGraph::VectorGraph(VectorGraph &originalGraph) {
  this->n = originalGraph.num_vertices();
  this->G = vector<vector<vectorImplementation::edge>> (n+1);

  // Insere os arcos diretos presentes no grafo original
  // e os arcos reversos de capacidade zero caso não exista no grafo original
  for (int u = 0; u < this->n + 1; u++) {
    vector<vectorImplementation::edge> &v = originalGraph.get_edge_list(u);

    for (vectorImplementation::edge e : v) {
      add_edge(u, e.destNode, e.flow, e.capacity);   // direct edge

      if (originalGraph.get_edge_pointer(e.destNode, u) == NULL) {
        add_edge(e.destNode, u, 0, 0);               // reverse edge
      }
    }
  }
}

// Adiciona um arco (u,v) na lista de arcos que saem de u
void VectorGraph::add_edge(int u, int v, int flow, int capacity) {
  G[u].push_back( vectorImplementation::edge{v, flow, capacity} );
  m++;
}

// Acrescenta capacityEdit unidades na capacidade atual do arco (u,v)
void VectorGraph::add_capacity(int u, int v, int capacityEdit) {
  vectorImplementation::edge *e = get_edge_pointer(u, v);
  
  if (e != NULL) {
    e->capacity += capacityEdit;
  }
}

// Acrescenta pushFlow unidades no fluxo atual do arco (u,v)
void VectorGraph::add_flow(int u, int v, int pushFlow) {
  vectorImplementation::edge *e = get_edge_pointer(u, v);

  if (e != NULL and e->flow + pushFlow <= e->capacity) {
    e->flow += pushFlow;
  }
}

// Retorna um apontador para o primeiro arco saindo de u
vector<vectorImplementation::edge> & VectorGraph::get_edge_list(int u) {
  return G[u]; 
}

// Retorna um apontador para o arco (u,v), caso exista, ou para NULL, caso contrário
vectorImplementation::edge * VectorGraph::get_edge_pointer(int u, int v) {
  for (int i = 0; i < G[u].size(); i++) {
    if(G[u][i].destNode == v)
      return &(G[u][i]);
  }
  
  return nullptr;
}

// Retorna o número de vértices contido no grafo
int VectorGraph::num_vertices() { return this->n; }

// Retorna o número de Arcos contido no grafo
int VectorGraph::num_edges() { return this->m; }


// percorre o grafo e imprime no formato: identificador origem destino capacidade
void VectorGraph::print_grafo() {
  for (int u = 1; u < n + 1; u++) {
    for (vectorImplementation::edge e: G[u]) {
      cout << "a " << u << " " << e.destNode << " " << e.flow << " " << e.capacity << endl;
    }
  }
}

VectorGraph::~VectorGraph() {
  // não é necessário desalocar memória
}