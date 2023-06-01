#pragma once

#include "../graph.cpp"

#include <vector>
#include <iostream>
#include <fstream> 

using namespace std;

void swap(vectorImplementation::edge &a, vectorImplementation::edge &b) {
	vectorImplementation::edge temp = a;
	a = b;
	b = temp;
}

int median(vector<vectorImplementation::edge> &v, vector<int> &priority, int start, int middle, int end) {
	if (priority[v[start].destNode] > priority[v[middle].destNode]) {
		if (priority[v[middle].destNode] > priority[v[end].destNode]) {
			return middle;
		}
		else if (priority[v[start].destNode] > priority[v[end].destNode]) {
			return end;
		}
		//else {
			return start;
		//}
	}
	//else {
		if (priority[v[start].destNode] > priority[v[end].destNode]) {
			return start;
		}
		else if (priority[v[middle].destNode] > priority[v[end].destNode]) {
			return end;
		}
		//else {
			return middle;
		//}
	//}
}

int separate(vector<vectorImplementation::edge>& v, int start, int end, vector<int> &priority) {
	int i = start;
	int j = end - 1;

	// utiliza o elemento do meio do vetor como pivô
	int middle = start + (end - start) / 2;
	int x = median(v, priority, start, middle, end);
	swap(v[x], v[end]);
	
	int pivot = priority[v[end].destNode];

	while (1) {
		while (i < end and priority[v[i].destNode] <= pivot) i++;
		while (j > i and priority[v[j].destNode] > pivot) j--;

		if (i >= j) break;
		swap(v[i], v[j]);
	}

	swap(v[i], v[end]);
	return i;
}

void quickSort(vector<vectorImplementation::edge> &v, int start, int end, vector<int> &priority) {
	if (start < end) {
		int i = separate(v, start, end, priority);
		quickSort(v, start, i - 1, priority);   // ordena à esquerda
		quickSort(v, i + 1, end, priority);			// ordena à direita
	}
}

void quickSortRSemiIter(vector<vectorImplementation::edge> &v, int start, int end, vector<int> &priority) {
	while (start < end) {
		int i = separate(v, start, end, priority);
		if (i - start < end - i) { // se o subvetor esquerdo é menor
			quickSort(v, start, i - 1, priority);   
			start = i + 1;
		}
		else { // se o subvetor direito é menor
			quickSort(v, i + 1, end, priority);	
			end = i - 1;
		}
	}
}


/* QuickSort para vetores do tipo int

void swap(int *i, int *j) {
	int temp = *i;
	*i = *j;
	*j = temp;
}

int separate(vector<int>& v, int start, int end) {
	int i = start;
	int j = end - 1;
	int pivot = v[end];

	while (1) {
		while (i < end and v[i] <= pivot) i++;
		while (j > i and v[j] > pivot) j--;

		if (i >= j) break;
		swap(&v[i], &v[j]);
	}

	swap(&v[i], &v[end]);
	return i;
}

void quickSort(vector<int>& v, int start, int end) {
	if (start < end) {
		int i = separate(v, start, end);
		quickSort(v, start, i - 1);   // ordena à esquerda
		quickSort(v, i + 1, end);			// ordena à direita
	}
	return;
}

int main () {
	vector<int> v = {3, 5, 6 ,4 ,2};
	quickSort(v, 0, v.size() - 1);

	for (int x : v) {
		cout << x << " ";
	}
	cout << endl;
	return 0;
}
*/