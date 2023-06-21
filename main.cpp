#include "./Graph/graph.h"
#include "./Graph/graph.cpp"

#include "./Graph/utils/getWord.cpp"

#include "./Graph/most_improved_augmenting_path_max_flow.cpp"
#include "./Graph/capacity_scaling_max_flow.cpp"
#include "./Graph/shortest_augmenting_path_max_flow.cpp"
#include "./Graph/push_relabel_max_flow.cpp"
#include "./Graph/highest_push_relabel_max_flow_v1.cpp"
#include "./Graph/highest_push_relabel_max_flow_v2.cpp"
#include "./Graph/highest_push_relabel_max_flow_v3.cpp"
#include "./Graph/highest_push_relabel_max_flow_v4.cpp"
#include "./Graph/highest_push_relabel_max_flow_v5.cpp"


#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <math.h>

using namespace std;


int main(int argc, char **argv)
{
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
  string algorithm = argv[2];
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


  int fmax = 0;
  int num_paths = 0;
  long time_execution;

  ListGraph Gf_list(G_list);

  if (algorithm == "BAP") {
    std::chrono::time_point start = std::chrono::steady_clock::now();
      fmax = most_improved_augmenting_path_max_flow(Gf_list, s, t, &num_paths);
    std::chrono::time_point end = std::chrono::steady_clock::now();
    time_execution = (end - start).count();

    cout << "Arcos: " << G_list.num_edges() << endl;
    cout << "Vertices: " << G_list.num_vertices() << endl; 
    cout << "Fluxo: " << fmax << endl;
    cout << "Tempo(ns): " << time_execution << endl;
    cout << "Caminhos: " << num_paths << endl;
  }

  else if (algorithm == "CS") {
    std::chrono::time_point start = std::chrono::steady_clock::now();
      fmax = capacity_scaling_max_flow(Gf_list, s, t, &num_paths);
    std::chrono::time_point end = std::chrono::steady_clock::now();
    time_execution = (end - start).count();

    cout << "Arcos: " << G_list.num_edges() << endl;
    cout << "Vertices: " << G_list.num_vertices() << endl; 
    cout << "Fluxo: " << fmax << endl;
    cout << "Tempo(ns): " << time_execution << endl;
    cout << "Caminhos: " << num_paths << endl;
  }

  else if (algorithm == "SAP") {
    std::chrono::time_point start = std::chrono::steady_clock::now();
      fmax = shortest_augmenting_path_max_flow(Gf_list, s, t, &num_paths);
    std::chrono::time_point end = std::chrono::steady_clock::now();
    time_execution = (end - start).count();

    cout << "Arcos: " << G_list.num_edges() << endl;
    cout << "Vertices: " << G_list.num_vertices() << endl; 
    cout << "Fluxo: " << fmax << endl;
    cout << "Tempo(ns): " << time_execution << endl;
    cout << "Caminhos: " << num_paths << endl;
  }

  else if (algorithm == "PR") {
    push_relabel_max_flow max_flow(G_list);
    fmax = max_flow.solve(s, t);

    cout << "Arcos: " << G_list.num_edges() << endl;
    cout << "Vertices: " << G_list.num_vertices() << endl; 
    cout << "Fluxo: " << max_flow.get_max_flow() << endl;
    cout << "Tempo(ns): " << max_flow.get_time_execution() << endl;
    cout << "Pushs: " << max_flow.get_num_pushs() << endl;
    cout << "Relabels: " << max_flow.get_num_relabels() << endl;
  }

  else if (algorithm == "HPRv1") {
    highest_push_relabel_max_flow_v1 max_flow(G_list);
    fmax = max_flow.solve(s, t);

    cout << "Arcos: " << G_list.num_edges() << endl;
    cout << "Vertices: " << G_list.num_vertices() << endl; 
    cout << "Fluxo: " << max_flow.get_max_flow() << endl;
    cout << "Tempo(ns): " << max_flow.get_time_execution() << endl;
    cout << "Pushs: " << max_flow.get_num_pushs() << endl;
    cout << "Relabels: " << max_flow.get_num_relabels() << endl;
  }

  else if (algorithm == "HPRv2") {
    highest_push_relabel_max_flow_v2 max_flow(G_vector);
    fmax = max_flow.solve(s, t);

    cout << "Arcos: " << G_list.num_edges() << endl;
    cout << "Vertices: " << G_list.num_vertices() << endl; 
    cout << "Fluxo: " << max_flow.get_max_flow() << endl;
    cout << "Tempo(ns): " << max_flow.get_time_execution() << endl;
    cout << "Pushs: " << max_flow.get_num_pushs() << endl;
    cout << "Relabels: " << max_flow.get_num_relabels() << endl;
  }

  // else if (algorithm == "HPRv3") {
  //   highest_push_relabel_max_flow_v3 max_flow(G_vector);
  //   fmax = max_flow.solve(s, t);

  //   cout << "Arcos: " << G_list.num_edges() << endl;
  //   cout << "Vertices: " << G_list.num_vertices() << endl; 
  //   cout << "Fluxo: " << max_flow.get_max_flow() << endl;
  //   cout << "Tempo(ns): " << max_flow.get_time_execution() << endl;
  //   cout << "Pushs: " << max_flow.get_num_pushs() << endl;
  //   cout << "Relabels: " << max_flow.get_num_relabels() << endl;
  // }

  else if (algorithm == "HPRv4") {
    highest_push_relabel_max_flow_v4 max_flow(G_list);
    fmax = max_flow.solve(s, t);

    cout << "Arcos: " << G_list.num_edges() << endl;
    cout << "Vertices: " << G_list.num_vertices() << endl; 
    cout << "Fluxo: " << max_flow.get_max_flow() << endl;
    cout << "Tempo(ns): " << max_flow.get_time_execution() << endl;
    cout << "Pushs: " << max_flow.get_num_pushs() << endl;
    cout << "Relabels: " << max_flow.get_num_relabels() << endl;
  }

  else if (algorithm == "HPRv5") {
    highest_push_relabel_max_flow_v5 max_flow(G_list);
    fmax = max_flow.solve(s, t);

    cout << "Arcos: " << G_list.num_edges() << endl;
    cout << "Vertices: " << G_list.num_vertices() << endl; 
    cout << "Fluxo: " << max_flow.get_max_flow() << endl;
    cout << "Tempo(ns): " << max_flow.get_time_execution() << endl;
    cout << "Pushs: " << max_flow.get_num_pushs() << endl;
    cout << "Relabels: " << max_flow.get_num_relabels() << endl;
  }
  
  return 0;
}