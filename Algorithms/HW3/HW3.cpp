#include<iostream>
#include<stdio.h>
#include<list>
#include<sstream>

using namespace std;

struct item {
    int node;
    int steps; 
};

void GetAdjacencyMatrix(int* array, int n);
int BFS(int* array, int v1, int v2, int n);
int BFSLog(int* adj_matrix, int v1, int v2, int n);
bool GetDistanceMatrix(int* adj_matrix, int* dis_matrix, int n);
int Diameter(int* adj_matrix, int* dis_matrix, int n);
string ConnectedComponents(int* adj_matrix, int* dis_matrix, int n);
void ZeroMatrix(int* array, int n);
void PrintMatrix(int* array, int n);

int main() {
    cout << "Please enter the number of vertices: ";
    int n;
    cin >> n;
    
    int a[n*n];
    int d[n*n];
    
    ZeroMatrix(a, n);
    ZeroMatrix(d, n);
    
    GetAdjacencyMatrix(a, n);

    bool connected = GetDistanceMatrix(a, d, n);
    
    cout << endl << endl << "Adjacency Matrix" << endl << endl;   
    PrintMatrix(a, n);
    
    cout << endl << endl << "Distance Matrix" << endl << endl;
    PrintMatrix(d, n);
    
    if (connected) {
        cout << "Network is connected." << endl;
        cout << "\t Diameter is: " << Diameter(a, d, n) << endl;
    } else {
        cout << "Graph is not connected." << endl << "Connected Components:\n" << ConnectedComponents(a, d, n);
    }
    cout << endl;
    return 0;
}

void GetAdjacencyMatrix(int* array, int n) {
    int node1, node2;
    bool working = true;

    while (working) {
        cout << "Enter first node of edge: ";
        cin >> node1;
        if (node1 < 0) {
            working = false;
            break;
        }
        
        cout << "Enter second node of edge: ";
        cin >> node2;
        
        array[node1 * n + node2] = 1;
        array[node2 * n + node1] = 1;
    }    
}


int BFS(int* adj_matrix, int v1, int v2, int n) {
    if (v1 == v2) {
        //same node
        return 0;
    }
    
    int visited[n*n];
    ZeroMatrix(visited, n);
    
    int steps, node;
    list<item> q;
    
    item a, b, t;
    a.node = v1;
    a.steps = 0;
    q.push_back(a);
    
    //mark v1 as visited
    for (int i = 0; i < n; i++) {
        visited[i*n+v1] = 1;
    }
    
    while (!q.empty()){
        t = q.front();
        q.pop_front();
        for (int i = 0; i < n; i++){
            if (adj_matrix[t.node*n+i] == 1 && visited[t.node*n+i] != 1) {
                //link present and new node not visited
                
                //add node i to back of queue
                b.node = i;
                b.steps = t.steps + 1;
                q.push_back(b);
                
                //mark node i as visited
                for (int i2 = 0; i2 < n; i2++) {
                    visited[i2*n+i] = 1;
                }
                
                //break if we reached the node we're looking for
                if (i == v2) {
                    return t.steps + 1;
                }
            }
        }
    }
    return -1;
}

int BFSLog(int* adj_matrix, int v1, int v2, int n) {
    if (v1 == v2) {
        //same node
        return 0;
    }
    
    cout << "Starting at node " << v1 << ", looking for node " << v2 << endl; 
    
    int visited[n*n];
    ZeroMatrix(visited, n);
    
    int steps, node;
    list<item> q;
    
    item a, b, t;
    a.node = v1;
    a.steps = 0;
    q.push_back(a);
    
    //mark v1 as visited
    for (int i = 0; i < n; i++) {
        visited[i*n+v1] = 1;
    }
    
    while (!q.empty()){
        t = q.front();
        q.pop_front();
        cout << "Current Node: " << t.node << endl;
        for (int i = 0; i < n; i++){
            if (adj_matrix[t.node*n+i] == 1 && visited[t.node*n+i] != 1) {
                //link present and new node not visited
                
                //add node i to back of queue
                b.node = i;
                b.steps = t.steps + 1;
                q.push_back(b);
                
                cout << "\tFound: " << i << " (" << t.steps + 1 << " steps)" << endl;
                
                //mark node i as visited
                for (int i2 = 0; i2 < n; i2++) {
                    visited[i2*n+i] = 1;
                }
                
                //break if we reached the node we're looking for
                if (i == v2) {
                    cout << "Reached node " << v2 << " in " << t.steps + 1 << " steps." << endl << endl;
                    return t.steps + 1;
                }
            }
        }
    }
    return -1;
}

bool GetDistanceMatrix(int* adj_matrix, int* dis_matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int i2 = 0; i2 < n; i2++) {
            dis_matrix[i*n+i2] = BFS(adj_matrix, i, i2, n);
        }
    }
    
    for (int i = 0; i < n*n; i++){
        if (dis_matrix[i] == -1) {
            return false;
        }
    }
    return true;
}

int Diameter(int* adj_matrix, int* dis_matrix, int n) {
    int max = 0;
    if (GetDistanceMatrix(adj_matrix, dis_matrix, n)) {
        for (int i = 0; i < n*n; i++) {
            if (max < dis_matrix[i]){
                max = dis_matrix[i];
            }
        }
    } else {
        return -1;
    }
    return max;
}

string ConnectedComponents(int* adj_matrix, int* dis_matrix, int n) {
    string components = "";
    ostringstream oss;
    if (!GetDistanceMatrix(adj_matrix, dis_matrix, n)) {
        int visited[n*n];
        ZeroMatrix(visited, n);
        
        list<int> q;
        int c;
        
        oss << "\t";
        
        for (int i = 0; i < n; i++) {
            if (visited[i] != 1) {
                q.push_back(i);
                
                for (int i2 = 0; i2 < n; i2++) {
                    visited[i2*n+i] = 1;
                }
                
                oss << "[" << i;
                while (!q.empty()) {
                    c = q.front();
                    q.pop_front();
                    
                    for (int i2 = 0; i2 < n; i2++){
                        if (adj_matrix[c*n+i2] == 1 && visited[c*n+i2] != 1) {
                            //link present and new node not visited
                            
                            //add node i to back of queue
                            q.push_back(i2);
                            
                            oss << ", " << i2;
                            //mark node i as visited
                            for (int i3 = 0; i3 < n; i3++) {
                                visited[i3*n+i2] = 1;
                            }                        
                        }
                    }
                }
                oss << "]\n\t";
            }
        }
        components = oss.str();
    } else {
        components = "n/a: graph is connected.";
    }
    return components;
}


void ZeroMatrix(int* matrix, int n) {
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            matrix[x*n+y] = 0;
        }
    }
}

void PrintMatrix(int* matrix, int n) {
    cout << "\t";
    for (int i = 0; i < n; i++){
        cout << i << "\t";
    }
    cout << endl << endl << endl;
    for (int y = 0; y < n; y++) {
        cout << y << "\t";
        for (int x = 0; x < n; x++){
            cout << matrix[x*n+y] << "\t";
        }
        cout << endl << endl << endl;
    }
}
