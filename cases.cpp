#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <deque>
#include <fstream>
#include <unistd.h>
#include <string>
#include <unordered_map>

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

bool isPath(const vii& edges) {
    unordered_map<int, int> check;

    for (const auto& p : edges) {
        if (check.find(p.first) == check.end()) {
            check[p.first] = 1;
        } else {
            check[p.first]++;
        }

        if (check.find(p.second) == check.end()) {
            check[p.second] = 1;
        } else {
            check[p.second]++;
        }
    }

    int one = 3;

    for (auto& p : check) {
        if (p.second == 1) one--;
        if (!one) return false;
    }

    return true; 
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

    vii aux;

    do {
        aux.clear();

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

            aux.push_back(make_pair(TV + th[a], TV + b));
        }
    } while (isPath(aux));

    edges.insert(edges.end(), aux.begin(), aux.end());

    TV += V;
}

int main(int argc, char** argv) {

    int N, TV = 1;
    int R = 0, F = 0, B = 0, T = 0;

    int sflag = 0, fflag = 0, c;
    string filename;
    uint_least32_t seed;    
    opterr = 0;

    while ((c = getopt (argc, argv, "s:f:")) != -1)
    switch (c) {
    case 's':
        sflag = 1;
        seed = atoi(optarg);
        break;
    case 'f':
        fflag = 1;
        filename = optarg;
        break;
    case '?':
        if (optopt == 's' || optopt == 'f')
            cerr << "Option -" << optopt << " requires an argument." << endl;
        else
            cerr << "Unknown option -" << optopt << "." << endl;
    default:
        return 1;
    }

    // Random Seed Creation
    if (!sflag)
        sysrandom(&seed, sizeof(seed));

    // Random Engine Initialization
    mt19937 gen(seed);

    cin >> N;

    if (N < 2) {
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
            B++;
            genPermutation(l + r, TV, map, gen);
            genCompleteBipartite(l, r, TV, edges);
            break;
        case 2:
            cin >> v;
            T++;
            genPermutation(v, TV, map, gen);
            genCycle(v, TV, edges);
            break;
        case 3:
            cin >> v;
            R++;
            genPermutation(v, TV, map, gen);
            genPath(v, TV, edges);
            break;
        case 4:
            cin >> v;
            F++;
            genPermutation(v, TV, map, gen);
            genTree(v, TV, edges, gen);
            break;
        default:
            cerr << "Identificador incorreto de tipo de nave!" << endl;
            return 1;
        }
    }

    // Shuffle the configurations
    shuffle(edges.begin(), edges.end(), gen);
    shuffle(map.begin(), map.end(), gen);
    
    // Print Test Case
    cout << TV - 1 << " " << edges.size() << endl;

    for (const auto& p : edges)
        cout << p.first << " " << p.second << endl;

    for (const auto& p : map)
        cout << p.first << " " << p.second << endl;

    cerr << R << " " << F << " " << B << " " << T << endl;

    if (fflag) {
        ofstream file;
        file.open(filename, ios::out);
        file << seed;
        file.close();
    }


    return 0;
}
