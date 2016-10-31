#include<iostream>
#include<stdio.h>

using namespace std;

void ZeroMatrix(int* array, int n);
void PrintMatrix(int* array, int n);

int main() {
    cout << "Please enter the number of vertices: ";
    int n;
    cin >> n;
    
    int a[n*n];
    int d[n*n];
    
    int node1, node2;
    bool working = true;
    
    ZeroMatrix(a, n);
    ZeroMatrix(d, n);
    
    while (working) {
        cout << "Enter first node of edge: ";
        cin >> node1;
        if (node1 < 0) {
            working = false;
            break;
        }
        
        cout << "Enter second node of edge: ";
        cin >> node2;
        
        a[node1 * n + node2] = 1;
        a[node2 * n + node1] = 1;
    }
    
    PrintMatrix(a, n);
    
    return 0;
}

void ZeroMatrix(int* array, int n) {
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            array[x*n+y] = 0;
        }
    }
}

void PrintMatrix(int* array, int n) {
    cout << "  ";
    for (int i = 0; i < n; i++){
        cout << i << " ";
    }
    cout << endl;
    for (int y = 0; y < n; y++) {
        cout << y << " ";
        for (int x = 0; x < n; x++){
            cout << array[x*n+y] << " ";
        }
        cout << endl;
    }
}
