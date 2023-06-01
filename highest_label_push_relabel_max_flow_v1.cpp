#include "graph.cpp"

#include <queue>
#include <vector>
#include <algorithm>
#include <chrono>


auto totGf_dpr1= (p - p).count();
auto totpush_dpr1= (p - p).count();
auto totrelabel_dpr1= (p - p).count();
auto totatualiza1 = (p - p).count();
auto totatualiza2 = (p - p).count();
auto totbucket = (p - p).count();

int n_push = 0;
int n_relabel = 0;


namespace operations_highest_label_push_relabel_max_flow_v1 {

  void preflowInitialize(ListGraph &Gf, vector<int>& excesso, vector<int>& altura, vector<std::queue<int>> &b, int s, int t) {
    // satura todos os arcos que saem da fonte no grafo Original -> f(s,v) = u(s,v)
    for (LinkedListImplementation::edge *aux = Gf.get_edge(s); aux != NULL; aux = aux->next) {

      // Atualiza o excesso do nó que recebeu fluxo e o insere na fila de prioridade de altura
      if (aux->destNode != t and aux->capacity > 0) {
        excesso[aux->destNode] = aux->capacity;
        b[0].push(aux->destNode);
      }

      // Atualiza as capacidades residuais dos arcos diretos e reversos
      Gf.add_capacity(aux->destNode, s, +aux->capacity);
      aux->capacity -= aux->capacity;
    }

    altura[s] = Gf.num_vertices();
  }

  bool isAdmissible(int u, int v, int capacity, vector<int>& altura) {
    if (altura[u] == altura[v] + 1 and capacity > 0) {
      return true;
    }
    return false;
  }



  void pushFlow(ListGraph &Gf, int u, LinkedListImplementation::edge *aux, vector<int>& excesso,\
  vector<int>& altura, vector<std::queue<int>>& b, int s, int t) {
    n_push++;

    // Empurra fluxo no arco (u,v)
    int delta = min(excesso[u], aux->capacity);

    auto startatualiza= chrono::steady_clock::now();

    // Atualiza a capacidade dos arcos (u,v) e (v,u) no grafo residual
    aux->capacity -= delta;

    auto endatualiza= chrono::steady_clock::now();
    totatualiza1 += (endatualiza - startatualiza).count();

    startatualiza= chrono::steady_clock::now();
    Gf.add_capacity(aux->destNode, u, delta);

    endatualiza= chrono::steady_clock::now();
    totatualiza2 += (endatualiza - startatualiza).count();

    auto startbucket= chrono::steady_clock::now();
    // Insere o nó que recebeu excesso na fila se ele é diferente de s e t, e não possuia excesso antes da operação
    if (excesso[aux->destNode] == 0 and (aux->destNode != s and aux->destNode != t)) {
      b[altura[aux->destNode]].push(aux->destNode);
    }
    auto endbucket= chrono::steady_clock::now();
    totbucket += (endbucket - startbucket).count();

    // Atualiza o excesso de u
    excesso[u] -= delta;

    // Atualiza o excesso recebido por v
    if (aux->destNode != s and aux->destNode != t) {
      excesso[aux->destNode] += delta; 
    }
  }

  void relabel(ListGraph &Gf, int u, vector<int>& altura, vector<std::queue<int>> &b, int *d) {
    // Para todos os arcos (u,v) com capacidade residual positiva,
    // encontra o vértice v com menor rótulo de altura e atualiza
    // a altura de u para que (u,v) se torne um arco admissível.
    n_relabel++;

    int h_min = INT_MAX;
    for (LinkedListImplementation::edge *aux = Gf.get_edge(u); aux != NULL; aux = aux->next) {
      if (aux->capacity > 0) {
        h_min = min(h_min, altura[aux->destNode] + 1);
      }
    }

    altura[u] = h_min;

    // Como no algoritmo highest label Push-Relabel pegamos sempre o nó com maior altura, d* sempre aumenta ao fazer relabel
    if (altura[u] > *d) {
      *d = altura[u];
    }
  }

  void discharge(int u, ListGraph &Gf, vector<int>& excesso, vector<int>& altura, \
  vector<std::queue<int>>& b, int *d, int s, int t) {
    while (excesso[u] > 0) {
      auto startpush_dpr1= chrono::steady_clock::now();

      for (LinkedListImplementation::edge *aux = Gf.get_edge(u); aux != NULL; aux = aux->next) {
        if (isAdmissible(u, aux->destNode, aux->capacity, altura)) {
          pushFlow(Gf, u, aux, excesso, altura, b, s, t);
        }
      }
      auto endpush_dpr1= chrono::steady_clock::now();
      totpush_dpr1 += (endpush_dpr1 - startpush_dpr1).count();

      if (excesso[u] > 0) {
        auto startrelabel_dpr1= chrono::steady_clock::now();
        relabel(Gf, u, altura, b, d);
        auto endrelabel_dpr1= chrono::steady_clock::now();
        totrelabel_dpr1 += (endrelabel_dpr1 - startrelabel_dpr1).count();
      }
    }
  }
}


int highest_label_push_relabel_max_flow_v1(ListGraph &G, int s, int t) {
  int flow = 0;
  int len = G.num_vertices() + 1;
  vector<int> excesso(len);
  vector<int> altura(len);
  //int excesso[len] = {}; // armazena o excesso presente em cada nó
  //int altura[len] = {};  // armazena o rótulo de altura de cada nó

  auto startGf_dpr1= chrono::steady_clock::now();

  ListGraph Gf(G);           // Cria um grafo residual a partir de G
  vector<queue<int>> b(2 * len); // Cria um vetor de filas

  auto endGf_dpr1= chrono::steady_clock::now();
  totGf_dpr1 += (endGf_dpr1 - startGf_dpr1).count();
  
  operations_highest_label_push_relabel_max_flow_v1::preflowInitialize(Gf, excesso, altura, b, s, t);
  int d = 0;

  while (d >= 0) {
    if (!b[d].empty()) {
      int u = b[d].front(); b[d].pop();
      operations_highest_label_push_relabel_max_flow_v1::discharge(u, Gf, excesso, altura, b, &d, s, t);
    } 
    else {
      d = d - 1;
    }
  }

  // Calcula o fluxo que sai fonte
  for (LinkedListImplementation::edge *aux = G.get_edge(s); aux != NULL; aux = aux->next) {
    LinkedListImplementation::edge *auxResidual = Gf.get_edge(s, aux->destNode);
    flow += aux->capacity - auxResidual->capacity;
  }

  int totdir = totatualiza1 / 1000000;
  int totrev = totatualiza2 / 1000000;
  int totbuck = totbucket / 1000000;

  cout << "push direto: " << totdir << endl;
  cout << "push reverso: " << totrev << endl;
  cout << "insere bucket: " << totbuck << endl;
  cout << "nro push: " << n_push << endl;
  cout << "nro relabel: " << n_relabel << endl;
  
  return flow;
}