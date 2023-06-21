#include <iostream>

void minhaFuncao() {
    std::cout << "Função chamada!" << std::endl;
}

int main() {
    int x = 0;
    int limite = 20; // Limite de operações antes de chamar a função
    
    for (int i = 0; i <= 100; i++) { // Exemplo de 5000 operações
        x = (i + 1) % limite;
        std::cout << x << std::endl;
        
        if (x == 0) {
            minhaFuncao();
        }
    }
    
    return 0;
}