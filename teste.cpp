#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>

#include "./dependencies/getWord.cpp"


using namespace std;

// Estrutura para representar um arco no grafo
struct Edge {
    int from, to, capacity, flow;
};

// Classe para representar um grafo de fluxo
class FlowGraph {
    int numVertices;
    vector<Edge> edges;
    vector<vector<int>> adjList;
    vector<int> excess;
    vector<int> height;
    vector<int> count;
    vector<bool> active;
    vector<int> current;
    vector<queue<int>> buckets;

public:
    FlowGraph(int numVertices) : numVertices(numVertices), adjList(numVertices),
                                 excess(numVertices, 0), height(numVertices, 0),
                                 count(2 * numVertices, 0), active(numVertices, false),
                                 current(numVertices, 0), buckets(2 * numVertices) {}

    // Função para adicionar um arco direcionado ao grafo
    void addEdge(int from, int to, int capacity) {
        Edge e1 = {from, to, capacity, 0};
        Edge e2 = {to, from, 0, 0};

        adjList[from].push_back(edges.size());
        edges.push_back(e1);

        adjList[to].push_back(edges.size());
        edges.push_back(e2);
    }

    // Função para executar o algoritmo Push-Relabel com seleção de nó ativo
    int maxFlow(int source, int sink) {
        height[source] = numVertices;
        active[source] = active[sink] = true;

        for (int i = 0; i < adjList[source].size(); ++i) {
            int id = adjList[source][i];
            int v = edges[id].to;
            int cap = edges[id].capacity;

            excess[source] += cap;
            excess[v] -= cap;

            edges[id].flow = cap;

            edges[id ^ 1].capacity = cap;
            edges[id ^ 1].flow = -cap;

            if (v != source && v != sink) {
                buckets[0].push(v);
                active[v] = true;
            }
        }

        int maxHeight = 2 * numVertices - 1;
        int u = 0;

        // Loop principal do algoritmo
        while (u < numVertices) {
            if (active[u]) {
                bool pushed = false;

                for (int i = current[u]; i < adjList[u].size(); ++i) {
                    int id = adjList[u][i];
                    int v = edges[id].to;

                    if (edges[id].capacity - edges[id].flow > 0 && height[u] == height[v] + 1) {
                        int flow = min(excess[u], edges[id].capacity - edges[id].flow);

                        edges[id].flow += flow;
                        edges[id ^ 1].flow -= flow;

                        excess[u] -= flow;
                        excess[v] += flow;

                        if (v != source && v != sink && excess[v] > 0 && !active[v]) {
                            buckets[height[v]].push(v);
                            active[v] = true;
                        }

                        pushed = true;

                        if (excess[u] == 0)
                            break;
                    }
                }

                if (!pushed) {
                    current[u] = adjList[u].size();

                    if (--count[height[u]] == 0) {
                        for (int v = 0; v < numVertices; ++v) {
                            if (height[v] > height[u] && height[v] < numVertices) {
                                count[height[v]]--;
                                height[v] = numVertices + 1;
                            }
                        }
                    }

                    int minHeight = numVertices;

                    for (int i = 0; i < adjList[u].size(); ++i) {
                        int id = adjList[u][i];
                        int v = edges[id].to;

                        if (edges[id].capacity - edges[id].flow > 0)
                            minHeight = min(minHeight, height[v]);
                    }

                    height[u] = minHeight + 1;

                    if (u != source && u != sink && excess[u] > 0) {
                        buckets[height[u]].push(u);
                        active[u] = true;
                    }
                }
            }

            if (u == numVertices - 1) {
                u = 0;
                continue;
            }

            if (!buckets[u].empty()) {
                int v = buckets[u].front();
                buckets[u].pop();
                active[v] = false;
                current[v] = 0;
                u = v;
            } else {
                u++;
            }
        }

        return excess[sink];
    }
};

int main() {
cout << "opa";

  // Open to file instance
  ifstream file("./instancias/inst04.in");

  if (!file.is_open()) {
    cout << "erro na abertura do arquivo.\n\n";
    return EXIT_FAILURE;
  }


  //

  string word, label, type, line;
  char separator = ' ';
  int startIndex = 0;
  int n = 0, s = 0, t = 0;
  int u, v, capacity;
  int numPaths = 0;

  // Lê o nro de vértices do grafo e o rótulo dos vétices fonte e terminal
  getline(file, line);
  startIndex = 6;
  startIndex = getWord(line, startIndex, separator, &word); // nro nodes
  n = stoi(word);

  getline(file, line);
  startIndex = 2;
  startIndex = getWord(line, startIndex, separator, &label); // s
  s = stoi(label);

  getline(file, line);
  startIndex = 2;
  startIndex = getWord(line, startIndex, separator, &label); // t
  t = stoi(label);

    FlowGraph graph(n+1);

  // Percorre o arquivo e insere cada arco no grafo
  while (getline(file, line)) { 

    if (line[0] == 'a') {
      startIndex = 2;
      while (startIndex < line.length()) {
        startIndex = getWord(line, startIndex, separator, &word);
        u = stoi(word);

        startIndex = getWord(line, startIndex, separator, &word);
        v = stoi(word);

        startIndex = getWord(line, startIndex, separator, &word);
        capacity = stoi(word);

        graph.addEdge(u, v, capacity);
      }
    }
  }


    
    int maxFlow = graph.maxFlow(s, t);

    cout << "Fluxo máximo: " << maxFlow << endl;

    return 0;
}