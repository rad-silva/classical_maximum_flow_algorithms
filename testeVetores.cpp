#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std;

struct edge {
    int destNode, flow = 0, capacity;
    //edge *reverse = nullptr;
    edge *next;
};

int main () {
    int n = 10000000;
    edge e;

    long arr_init = 0, arr_acess_random = 0, arr_acess_sequencial = 0;
    long vec_init = 0, vec_acess_random = 0, vec_acess_sequencial = 0;
    std::chrono::time_point start = chrono::steady_clock::now();
    std::chrono::time_point end = chrono::steady_clock::now();


    edge *arr = new edge[n];
    vector<edge> vec;

    // Testa tempo de inserir elementos nos vetores
    start = chrono::steady_clock::now();
    for (int i = 0; i < n; i++)
        arr[i] = {i, i, 0, nullptr};
    end = chrono::steady_clock::now();

    arr_init = (end - start).count();

    start = chrono::steady_clock::now();
    for (int i = 0; i < n; i++) {
        vec.push_back( edge {i, i, 0, nullptr} );
    }
    end = chrono::steady_clock::now();

    vec_init = (end - start).count();


    // Testando tempo de acesso aleatório
    for (int i = 0; i < int(n/100); i++) {
        std::random_device rd; 
        std::mt19937 gen(rd()); 
        std::uniform_int_distribution<> dist(0, n - 1);

        int k = dist(gen); // Gera um número aleatório usando o gerador e a distribuição definidos


        start = chrono::steady_clock::now();
        e = arr[k];
        end = chrono::steady_clock::now();
        arr_acess_random += (end - start).count();

        start = chrono::steady_clock::now();
        e = vec[k];
        end = chrono::steady_clock::now();
        vec_acess_random += (end - start).count();
    }

    // Testando tempo de acesso sequencial
    for (int i = int(n/10000); i < int(n/10); i++) {
        start = chrono::steady_clock::now();
        e = arr[i];
        end = chrono::steady_clock::now();
        arr_acess_sequencial += (end - start).count();

        start = chrono::steady_clock::now();
        e = vec[i];
        end = chrono::steady_clock::now();
        vec_acess_sequencial += (end - start).count();
    }

    // Impressão dos resultados
    
    cout << "Tempo de insercao:\n";
    cout << "array : " << arr_init / 1e9 << endl;
    cout << "vector: " << vec_init / 1e9 << endl;

    cout << "\n";

    cout << "Tempo de acesso aleatorio:\n";
    cout << "array : " << arr_acess_random / 1e9 << endl;
    cout << "vector: " << vec_acess_random / 1e9 << endl;

    cout << "\n";

    cout << "Tempo de acesso sequencial:\n";
    cout << "array : " << arr_acess_sequencial / 1e9 << endl;
    cout << "vector: " << vec_acess_sequencial / 1e9 << endl;

    delete arr;
    return 0;
}