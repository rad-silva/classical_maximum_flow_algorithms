#include <queue>
#include <limits.h>
#include <string.h>
#include <iostream>
#include <fstream> 

using namespace std;

typedef struct No2 {
  int prioridade, rotulo;
  struct No2 *fwd, *bwd; // abreviation forward and backward
} no;

class Fila {
  int n; 
  No2 *q;

public:
  Fila();

  void insertElement(int rotulo, int prioridade);
  void changePriority(int rot, int prio);
  No2 removeElement();
  No2 getElement();
  int getTam() {return n;};

  bool empty();
  void printFila();
};

void Fila::printFila() {
  No2 * aux = q;
  while (aux != NULL) {
    cout << "[" << aux->rotulo << "," << aux->prioridade << "] ";
    aux = aux->fwd;
  }
  cout << endl;
}

Fila::Fila() {
  q = new No2;
  q = NULL;
  n = 0;
}

void Fila::insertElement(int rotulo, int prioridade) {
  No2 *novoNo = new No2;
  novoNo->prioridade = prioridade;
  novoNo->rotulo = rotulo;


  if (n == 0 or novoNo->prioridade > q->prioridade) {
    novoNo->fwd = q;
    novoNo->bwd = NULL;
    q = novoNo;

  } else {
    
    No2 *aux = q;

    while (aux != NULL and novoNo->prioridade <= aux->prioridade) {
      novoNo->bwd = aux;
      aux = aux->fwd;
    }

    novoNo->fwd = aux;

    novoNo->bwd->fwd = novoNo;
    if (novoNo->fwd != NULL) {
      novoNo->fwd->bwd = novoNo;
    }
  }

  n++;
}


No2 Fila::getElement() {
  No2 aux = *q;
  return(aux);
}

No2 Fila::removeElement() {
  No2 *aux = q;
  No2 backup = *q;

  if (n != 0) {
    q = q->fwd;
    q->bwd = NULL;
    delete aux; 
  }

  n--;
  return backup;
}

bool Fila::empty() {
  return (n == 0);
}

void Fila::changePriority(int rot, int prio) {
  No2 *ptr = q;
  int prioridadeAntiga;

  while (ptr != NULL) {
    if (ptr->rotulo == rot) {
      prioridadeAntiga = ptr->prioridade;
      ptr->prioridade = prio;
      break;
    }
    ptr = ptr->fwd;
  }

  if (ptr->prioridade > prioridadeAntiga and ptr != q) {
   
    ptr->bwd->fwd = ptr->fwd;
    
    if (ptr->fwd != NULL) {
      ptr->fwd->bwd = ptr->bwd;
    }

    No2 * aux = ptr->bwd;

    while (aux != NULL and aux->prioridade < ptr->prioridade) {
      ptr->fwd = aux;
      aux = aux->bwd;
    }
    
    ptr->bwd = aux;
    ptr->fwd->bwd = ptr;

    if (ptr->bwd != NULL) {
      ptr->bwd->fwd = ptr;
    }

    if (ptr->fwd == q) {
      q = ptr;
    }
  }
}

// int main () {
//   Fila f;

//   f.insertElement(5,1);
//   f.printFila();
//   f.insertElement(8,2);
//   f.printFila();
//   f.insertElement(7,3);
//   f.printFila();
//   f.insertElement(1,4);
//   f.printFila();
//   f.insertElement(3,5);
//   f.printFila();

//   No2 aux = f.removeElement();
//   cout << "remove: " << aux.rotulo << " " << aux.prioridade << endl;
//   f.printFila();
  
//   f.changePriority(10,3);
//   f.printFila();
//   return 0;
// }
