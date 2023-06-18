#pragma once

#include <iostream>
#include <vector>

struct Node {
  int id, priority;
};

class BinaryHeap {
private:
  std::vector<Node> heap;
  std::vector<int> rotulo;
  int size;

public:
  BinaryHeap(int num);
  // ~BinaryHeap(); 

  static int parent(int i) { return (i - 1) / 2; }
  static int leftChild(int i) { return 2 * i + 1; }
  static int rightChild(int i) { return 2 * i + 2; }

  bool empty() { return heap.size() == 0; }

  void insert(Node data);
  void upHeap(int i);
  void downHeap(int i);
  void changePriority(int i, Node data);
  Node extractMax();

  int  getIndexNode(int i);
  Node getNode(int rotuloNode);
  Node getMax();

  void printHeap();
};
  
BinaryHeap::BinaryHeap(int num) : size(0), rotulo(num, -1) {}

void BinaryHeap::insert(Node data) {
  if (rotulo[data.id] != -1) {
    changePriority(rotulo[data.id], data);

  } else {
    // Insere o elemento no fim do vetor heap e atualiza a posição do elemento no vetor rótulos
    heap.push_back(data);
    rotulo[data.id] = heap.size() - 1;
    size++;

    // Sobe o elemento na árvore para manter a propriedade de heap
    upHeap(size - 1);
  }
}

void BinaryHeap::upHeap(int i) {
  while (i != 0 and heap[parent(i)].priority < heap[i].priority) {
    std::swap(heap[parent(i)], heap[i]);
    std::swap(rotulo[heap[parent(i)].id], rotulo[heap[i].id]);
    i = parent(i);
  }
}

// moves the item at position i of array a
// into its appropriate position
void BinaryHeap::downHeap(int i){
  int left = leftChild(i);
  int right = rightChild(i);

  // encontra o maior entre 3 nós
  int largest = i;

  // checa se o filho esquerdo é maior que o nó atual
  if (left < size && heap[left].priority > heap[largest].priority) {
    largest = left;
  }

  // checa se o filho direito é maior que o nó atual
  if (right < size && heap[right].priority > heap[largest].priority) {
    largest = right;
  }

  // Troca o nó i com seu maior filho
  // e repete o processo até que o nó atual seja maior que qualquer um de seus filhos
  if (largest != i) {
    std::swap(heap[i], heap[largest]);
    std::swap(rotulo[heap[i].id], rotulo[heap[largest].id]);
    downHeap(largest);
  }
}

void BinaryHeap::changePriority(int i, Node data) {
  if (i > size) return;

  Node aux = heap[i];
  heap[i] = data;

  if (data.priority > aux.priority) upHeap(i);
  if (data.priority < aux.priority) downHeap(i);
}

Node BinaryHeap::extractMax() {
  // Backup do primeiro elemento do heap
  Node maxItem = heap[0];

  // Substitui o primeiro elemento do heap pelo ultimo
  heap[0] = heap.back();
  rotulo[heap[0].id] = 0;

  // Remove o ultimo elemento do heap
  heap.pop_back(); 
  rotulo[maxItem.id] = -1;
  size--;

  // mantém a propriedade de heap atualizando o heap 
  downHeap(0);

  return maxItem;
}

Node BinaryHeap::getMax() {
  return heap[0];
}

int BinaryHeap::getIndexNode(int i) {
  return rotulo[i];
}

Node BinaryHeap::getNode(int rotuloNode) {
  if (rotulo[rotuloNode] != -1) {
    return heap[rotulo[rotuloNode]];
  }

  Node aux;
  aux.id = rotuloNode;
  aux.priority = 0;

  return aux;
}

void BinaryHeap::printHeap() {
  for (int i = 0; i < size; i++) {
    std::cout << "["<< i << "] id:" << heap[i].id << " prio:"<< heap[i].priority << "  " ;
  }
  std::cout << "\n";
}