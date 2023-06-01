#include "graph.cpp"

#include "./dependencies/getWord.cpp"
#include "most_improved_augmenting_path_max_flow.cpp"
#include "shortest_augmenting_path_max_flow.cpp"
#include "capacity_scaling_max_flow.cpp"
#include "push_relabel_max_flow.cpp"
#include "highest_label_push_relabel_max_flow_v1.cpp"
#include "highest_label_push_relabel_max_flow_v2.cpp"
#include "highest_label_push_relabel_max_flow_v3.cpp"

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <math.h>

using namespace std;

// ./instancias/BVZ-tsukuba0.max

int main(int argc, char **argv) {

  if(argc < 2) {
    cout << "Numero incorreto de parâmetros. \n";
    cout << "Exemplo: " << argv[0] << " inst01.in \n";
    return 0;
  }

  // Open to file instance
  ifstream file(argv[1]);

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


  auto startG = chrono::steady_clock::now();

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


  ListGraph G_list(n);
  VectorGraph G_vector(n);

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

        G_list.add_edge(u, v, capacity);
        G_vector.add_edge(u, v, 0, capacity);
      }
    }
  }

  auto endG = chrono::steady_clock::now();
  auto Gctor = (endG - startG).count();

  //

  int fmax = 0;
  auto start = chrono::steady_clock::now();

  if (strcmp(argv[2], "BAP") == 0) {
    fmax = most_improved_augmenting_path_max_flow(G_list, s, t, &numPaths);
    cout << "Tempo grafo: " << Gctor << endl;
    cout << "Tempo grafo residual: " << totGf_bap << endl;
    cout << "Tempo WCP: " << totWCP_bap << endl;
    cout << "Tempo empurra fluxo: " << totEmpurraflowGf_bap << endl;
  }
  else if (strcmp(argv[2], "CS") == 0) {
    fmax = capacity_scaling_max_flow(G_list, s, t, &numPaths);
    cout << "Tempo grafo: " << Gctor << endl;
    cout << "Tempo grafo residual: " << totGf_cs << endl;
    cout << "Tempo grafo residual(delta): " << totGfdelta_cs << endl;
    cout << "Tempo BFS: " << totBFS << endl;
    cout << "Tempo calcula capacidade caminho: " << totCalculaMinCapacity_cs << endl;
    cout << "Tempo empurra fluxo: " << totEmpurraflowGfGfdelta_cs << endl;
  }
  else if (strcmp(argv[2], "SAP") == 0) {
    fmax = shortest_augmenting_path_max_flow(G_list, s, t, &numPaths);
    cout << "Tempo grafo: " << Gctor << endl;
    cout << "Tempo grafo residual: " << totResidG << endl;
    cout << "Tempo BFS: " << totBFS << endl;
    cout << "Tempo calcula capacidade caminho: " << totMinCapacidade << endl;
    cout << "Tempo empurra fluxo: " << totEmpurraflow << endl;
  }
  else if (strcmp(argv[2], "PR") == 0) {
    fmax = push_relabel_max_flow(G_list, s, t);
    cout << "Tempo grafo: " << Gctor << endl;
    cout << "Tempo grafo residual: " << totGf_pr << endl;
    cout << "Tempo push: " << totpush_pr << endl;
    cout << "Tempo relabel: " << totrelabel_pr << endl;
    cout << "Tempo heap: " << totfila_pr << endl;
  }
  else if (strcmp(argv[2], "HLPRv1") == 0) {
    fmax = highest_label_push_relabel_max_flow_v1(G_list, s, t);
    cout << "Tempo grafo: " << Gctor / 1000000 << endl;
    cout << "Tempo grafo residual: " << totGf_dpr1 / 1000000 << endl;
    cout << "Tempo push: " << totpush_dpr1 / 1000000 << endl;
    cout << "Tempo relabel: " << totrelabel_dpr1 / 1000000 << endl;
  }
  else if (strcmp(argv[2], "HLPRv2") == 0) {
    fmax = highest_label_push_relabel_max_flow_v2(G_vector, s, t);
    cout << "Tempo grafo: " << Gctor << endl;
    cout << "Tempo grafo residual: " << totGf_dprv2 << endl;
    cout << "Tempo sort: " << totsort_dprv2 << endl;
    cout << "Tempo push: " << totpush_dprv2 << endl;
    cout << "Tempo relabel: " << totrelabel_dprv2 << endl;
    cout << "Tempo discharge: " << totdischarge_dprv2 << endl;
  }
  else if (strcmp(argv[2], "HLPRv3") == 0) {
    fmax = highest_label_push_relabel_max_flow_v3(G_vector, s, t);
  }
  else {
    cout << "Metodo \'" << argv[2] << "\' inválido!\n\n";
  }

  auto end = chrono::steady_clock::now();
  auto duration = (end - start).count();

  //

  
  

  int numV = 0, numE = 0;

  if (strcmp(argv[2], "BAP") == 0 or strcmp(argv[2], "CS") == 0 or strcmp(argv[2], "SAP") == 0 or \
  strcmp(argv[2], "PR") == 0 or strcmp(argv[2], "HLPRv1") == 0) {
    numV =  G_list.num_vertices();
    numE = G_list.num_edges();
  }

  else if (strcmp(argv[2], "HLPRv2") == 0 or strcmp(argv[2], "HLPRv3") == 0) {
    numV =  G_vector.num_vertices();
    numE = G_vector.num_edges();
  }

  cout << "Vertices: " << numV << endl;
  cout << "Arcos: " << numE << endl;
  cout << "Fluxo: " << fmax << endl; 
  cout << "Tempo(ns): " << duration << endl;

  if (strcmp(argv[2], "BAP") == 0 or strcmp(argv[2], "CS") == 0 or strcmp(argv[2], "SAP") == 0) {
    cout << "Caminhos: " << numPaths << endl;
  }
  
  return 0;
}