#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <deque>
#include <fstream>

#define MINV 5

using namespace std;

typedef vector<pair<int,int>> vii;

// Generate a GOOD random seed for mt19937
size_t sysrandom(void* dst, size_t dstlen) {
    char* buffer = reinterpret_cast<char*>(dst);
    std::ifstream stream(
	"/dev/urandom", 
	std::ios_base::binary | std::ios_base::in);
    stream.read(buffer, dstlen);

    return dstlen;
}

void genPermutation(const int& V, const int& TV, vii& map, mt19937& gen) {
    
    vector<int> P;

    for (int i=0; i<V; i++) {
        P.push_back(i);
    }

    shuffle(P.begin(), P.end(), gen);

    for (int i=0; i<V; i++) {
        map.push_back(make_pair(TV + i, TV + P[i]));
    }
}

// L + R vertices
// L * R edges
void genCompleteBipartite(const int& L, const int& R, int& TV, vii& edges) {

    if (L + R < MINV) {
        cerr << "Número baixo de vértices!" << endl;
        exit(1);
    }

    for (int i=0; i<L; i++) {
        for (int j=0; j<R; j++) {
            edges.push_back(make_pair(TV + i, TV + L + j));
        }
    }

    TV += L + R;
}

// V vertices
// V edges
void genCycle(const int& V, int& TV, vii& edges) {

    if (V < MINV) {
        cerr << "Número baixo de vértices!" << endl;
        exit(1);
    }

    if (V % 2 != 0) {
        cerr << "Ciclos devem ter número par de vértices!" << endl;
        exit(1);
    }

    for (int i=0; i<V-1; i++) {
        edges.push_back(make_pair(TV + i, TV + i + 1));
    }
    
    // Close the Cycle
    edges.push_back(make_pair(TV, TV + V - 1));
    TV += V;
}

// V vertices
// V-1 edges
void genPath(const int& V, int& TV, vii& edges) {

    if (V < MINV) {
        cerr << "Número baixo de vértices!" << endl;
        exit(1);
    }

    for (int i=0; i<V-1; i++) {
        edges.push_back(make_pair(TV + i, TV + i + 1));
    }
    
    TV += V;
}

// V vertices
// V-1 edges
void genTree(const int& V, int& TV, vii& edges, mt19937& gen) {

    if (V < MINV) {
        cerr << "Número baixo de vértices!" << endl;
        exit(1);
    }

    deque<int>  sh; // Not added
    vector<int> th; // Added

    for (int i=0; i<V; i++) {
        sh.push_back(i); 
    }

    shuffle(sh.begin(), sh.end(), gen);

    // Add root
    th.push_back(sh.front());
    sh.pop_front();

    while (!sh.empty()) {
	uniform_int_distribution<> dist(0, th.size()-1);

        int a = dist(gen);  // Random idx from th 
        int b = sh.front(); // Get next from queue

        sh.pop_front();
        th.push_back(b);

        edges.push_back(make_pair(TV + th[a], TV + b));
    }

    TV += V;
}

int main() {

    int N, TV = 1;

    // Random initialization
    uint_least32_t seed;    
    sysrandom(&seed, sizeof(seed));
    mt19937 gen(seed);

    cin >> N;

    if (N <= 0) {
        cerr << "Número incorreto de naves!" << endl;
        return 1;
    }

    vii edges;
    vii map;

    for (int i=0; i<N; i++) {
        int t, v, l, r;
        cin >> t; 

        switch(t) {
        case 1:
            cin >> l >> r;
            genCompleteBipartite(l, r, TV, edges);
            genPermutation(l + r, TV, map, gen);
            break;
        case 2:
            cin >> v;
            genCycle(v, TV, edges);
            genPermutation(v, TV, map, gen);
            break;
        case 3:
            cin >> v;
            genPath(v, TV, edges);
            genPermutation(v, TV, map, gen);
            break;
        case 4:
            cin >> v;
            genTree(v, TV, edges, gen);
            genPermutation(v, TV, map, gen);
            break;
        default:
            cerr << "Identificador incorreto de tipo de nave!" << endl;
            return 1;
        }
    }

    shuffle(edges.begin(), edges.end(), gen);
    
    cout << TV - 1 << " " << edges.size() << endl;

    for (const auto& p : edges)
        cout << p.first << " " << p.second << endl;

    for (const auto& p : map)
        cout << p.first << " " << p.second << endl;

    cerr << "SEED: " << seed << endl;

    return 0;
}