#include <queue>
#include <iostream>

typedef struct height_queue HQ;
struct height_queue {
  int heightLabel;
  std::queue<int> queueNodeLabel;
  HQ *next;
};


class ListHeight {
  HQ *header = nullptr;
  HQ *alocateNode(int altura, int u);

public:
  void pushElement(int altura, int u);
  int popFrontElement();
  bool empty();
  void printList();
};


HQ *ListHeight::alocateNode(int altura, int u) {
  HQ *newNode = new HQ;
  newNode->heightLabel = altura;
  newNode->queueNodeLabel.push(u);

  return newNode;
}

void ListHeight::pushElement(int altura, int u) {

  if (header == nullptr or header->heightLabel < altura) {
    HQ *newNode = alocateNode(altura, u);
    newNode->next = header;
    header = newNode;
  }
  else {

    HQ *aux;
    for (aux = header; aux->next != nullptr and aux->next->heightLabel > altura; aux = aux->next);

    // Se foi encontrado um nó com altura igual a de u, insere u nesse nó
    if (aux->next != nullptr and aux->next->heightLabel == altura) {
      aux->next->queueNodeLabel.push(u);

    } 
    else {
      HQ *newNode = alocateNode(altura, u);
      newNode->next = aux->next;
      aux->next = newNode;
    }
  }
}

int ListHeight::popFrontElement() {
  if (!empty()) {
    int nodeLabel = header->queueNodeLabel.front();
    header->queueNodeLabel.pop();

    if (header->queueNodeLabel.empty()) {
      HQ *aux = header;
      header = header->next;
      delete aux;
    }

    return nodeLabel;
  }

  return -1;
}

bool ListHeight::empty() {
  return header == nullptr;
}

void ListHeight::printList() {
  std::queue<int> copyqueue;

  for (HQ *aux = header; aux != nullptr; aux = aux->next) {
    std::cout << aux->heightLabel << ": ";

    while (!aux->queueNodeLabel.empty()) {
      int r = aux->queueNodeLabel.front();
      aux->queueNodeLabel.pop();
      copyqueue.push(r);

      std::cout << r << " ";
    }

    std::cout << "\n";
    aux->queueNodeLabel = copyqueue;
  }
}


// // modulo de teste
// int main() {
//   ListHeight b;

//   // Teste 1: Inserindo elemento na lista vazia
//   b.pushElement(10, 2);

//   // Teste 2: Inserindo elemento em um nó existente na lista (primeiro)
//   b.pushElement(10, 4);

//   // Teste 3: Inserindo elemento não contido
//   b.pushElement(8, 5);

//   // Teste 4: Inserindo elemento em um nó existente na lista (ultimo)
//   b.pushElement(8, 1);

//   // Teste 5: Inserindo elemento cujo nó não existe entre dois nós existentes
//   b.pushElement(9, 8);

//   b.pushElement(11, 6);

//   b.printList();
//   cout << endl;

//   // Teste 6: removendo um elemento do primeiro nó
//   int x = b.popFrontElement();
//   cout << x << "\n\n";

//   b.printList();

//   // Teste 7: removendo o ultimo elemento contido no primeiro nó -> fila vazia;
//   x = b.popFrontElement();
//   cout << x << "\n\n";

//   b.printList();

//   return 0;
// }