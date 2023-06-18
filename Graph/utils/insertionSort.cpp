#pragma once

#include "../graph.h"

#include <vector>
#include <iostream>

void insertionSort(std::vector<vectorImplementation::edge> &v, std::vector<int> &priority) {
  for (int i = 1; i < v.size(); i++) {
    vectorImplementation::edge aux = v[i];

    int j;
    for (j = i - 1; j >= 0 and priority[v[j].destNode] > priority[aux.destNode]; j--) {
      v[j+1] = v[j];
    }

    v[j+1] = aux;
  }
  return;
}

void insertionSortv2(std::vector<vectorImplementation::edge> &v, std::vector<int> &priority) {
  for (int i = v.size() - 2; i >= 0; i--) {
    vectorImplementation::edge aux = v[i];

    int j;
    for (j = i + 1; j < v.size() and priority[v[j].destNode] < priority[aux.destNode]; j++) {
      v[j-1] = v[j];
    }

    v[j-1] = aux;
  }
}

