#pragma once

#include "../graph.cpp"

#include <vector>
#include <iostream>
#include <fstream> 
#include <algorithm> // Para a função max_element


using namespace std;

void countingSort(vector<vectorImplementation::edge>& arr, const vector<int>& altura) {
    int n = arr.size();
    vector<vectorImplementation::edge> output(n);

    // Encontra o valor máximo de altura
    int max_height = *max_element(altura.begin(), altura.end());

    // Inicializa o array de contagem com zeros
    vector<int> count(max_height + 1, 0);

    // Conta a frequência de cada altura
    for (int i = 0; i < n; i++) {
        count[altura[arr[i].destNode]]++;
    }

    // Calcula as posições corretas dos elementos no array de saída
    for (int i = 1; i <= max_height; i++) {
        count[i] += count[i - 1];
    }

    // Preenche o array de saída com os elementos ordenados
    for (int i = n - 1; i >= 0; i--) {
        int height = altura[arr[i].destNode];
        output[count[height] - 1] = arr[i];
        count[height]--;
    }

    // Copia os elementos ordenados de volta para o array original
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}