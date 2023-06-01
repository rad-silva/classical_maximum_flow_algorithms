#include "graphList.cpp"

#include <queue>
#include <vector>
#include <algorithm>
#include <chrono>

void DPreflowInitialize_cut(Graph &Gf, int altura[], int excesso[], vector<std::queue<int>> &b, int s, int t) {
  
  // satura todos os arcos que saem da fonte no grafo Original -> f(s,v) = u(s,v)
  for (Edge *aux = Gf.get_edge(s); aux != NULL; aux = aux->next) {

    // Atualiza o excesso do nó que recebeu fluxo e o insere no bucket de altura 0
    if (aux->capacity > 0) {
      excesso[aux->destNode] = aux->capacity;
      b[0].push(aux->destNode);
    }

    // Atualiza as capacidades residuais dos arcos diretos e reversos
    Gf.add_capacity(aux->destNode, s, +aux->capacity);
    aux->capacity -= aux->capacity;
  }

  altura[s] = Gf.num_vertices();

  return;
}

bool isAdmissible_cut(int u, Edge *aux, int altura[]) {
  if (altura[u] == altura[aux->destNode] + 1 and aux->capacity > 0) {
    return true;
  }
  return false;
}

void DPushFlow_cut(Graph &Gf, int u, Edge *aux, int altura[], int excesso[], \
vector<std::queue<int>>& b, int s, int t) {
  
  int n = Gf.num_vertices();

  // Calcula a quantidade de excesso que é possível enviar
  int delta = min(excesso[u], aux->capacity);
  
  // Atualiza a capacidade dos arcos (u,v) e (v,u) no grafo residual
  aux->capacity -= delta;
  Gf.add_capacity(aux->destNode, u, delta);

  // Insere o nó que recebeu excesso na fila se ele se tornou ativo:
  // é diferente de s e t, não possuia excesso antes da operação e sua altura é menor que n
  if (excesso[aux->destNode] == 0 and aux->destNode != s and aux->destNode != t) {
    b[altura[aux->destNode]].push(aux->destNode);
  }

  // Atualiza o excesso enviado por u
  excesso[u] -= delta;

  // Atualiza o excesso recebido por v
  if (aux->destNode != s) {
    excesso[aux->destNode] += delta; 
  }

  return;
}

void DRelabel_cut(Graph &Gf, int u, int altura[], vector<std::queue<int>> &b, int *d) {
  // Para todos os arcos (u,v) com capacidade residual positiva,
  // encontra o vértice v com menor rótulo de altura e atualiza
  // a altura de u para que (u,v) se torne um arco admissível.

  int h_min = INT_MAX;

  for (Edge *aux = Gf.get_edge(u); aux != NULL; aux = aux->next) {
    if (aux->capacity > 0) {
      h_min = min(h_min, altura[aux->destNode] + 1);
    }
  }

  altura[u] = h_min;

  // Se a altura de u é maior ou igual |V|, u deixa de ser um nó ativo
  // Se não, insere u no bucket da sua nova altura e atualiza o valor de d*
  if (altura[u] < Gf.num_vertices()) {
    b[altura[u]].push(u);

    if (altura[u] > *d) {
      *d = altura[u];
    }
  }

  return;
}

void Discharge_cut(int u, Graph &Gf, int excesso[], int altura[], \
vector<std::queue<int>>& b, int *d, int s, int t, int *r) {
  
  int n = Gf.num_vertices();

  // Descarrega o excesso do nó u enquanto ele for ativo
  while (excesso[u] > 0 and altura[u] < n) {
    for (Edge *aux = Gf.get_edge(u); aux != NULL; aux = aux->next) {
      if (isAdmissible_cut(u, aux, altura)) {
        DPushFlow_cut(Gf, u, aux, altura, excesso, b, s, t);
      }
    }

    if (excesso[u] > 0) {
      DRelabel_cut(Gf, u, altura, b, d);
      *r += 1;
    }
  }

  return;
}

int maxFlowDPR_cut(Graph &G, int s, int t) {
  int flow = 0;
  int len = G.num_vertices() + 1;
  int excesso[len] = {}; // armazena o excesso presente em cada nó
  int altura[len] = {};  // armazena o rótulo de altura de cada nó

  Graph Gf(G);           // Cria um grafo residual a partir de G
  vector<queue<int>> b(2 * len); // Cria um vetor de filas
  
  DPreflowInitialize_cut(Gf, altura, excesso, b, s, t);
  int d = 0; int r = 0;

  while (d >= 0) {
    if (!b[d].empty()) {
      int u = b[d].front(); b[d].pop();
      Discharge_cut(u, Gf, excesso, altura, b, &d, s, t, &r);
    } 
    else {
      d = d - 1;
    }
  }
  // cout << r << endl;
  
  // int min_cut = 0;
  // int n = G.getNumVertices();

  // cout << n << endl;
  // for (int x : altura) {
  //   cout << x << " ";
  // } cout << "\n";

  // for (int x : excesso) {
  //   cout << x << " ";
  // } cout << "\n";

  // Gf.printGrafoImage();
  
  // S = {u in V : d(u) >= |V|}
  // Procura todos os arcos (u,v) tal que u 
  // pertecente a S e j não pertence ao conjunto
  // for (int u = 1; u < n + 1; u++) {
  //   if (altura[u] >= n) {
  //     for (Edge *aux = G.getEdge(u); aux != NULL; aux = aux->next) {
  //       if (altura[aux->destNode] < n) {
  //         min_cut += aux->capacity;
  //       }
  //     }
  //   }
  // }
  
  return excesso[t];
}