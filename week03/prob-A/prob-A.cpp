// Week 3 - Problem A
// Transitive and reflexive greetings
// -> Find Minimum Spanning Tree
// (to minimize shouting distance)

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

class UnionFind {
private:
    size_t n;
    vector<int> parent;
    vector<int> size;

public:
    UnionFind(size_t n) : n(n) {
        this->size = vector<int>(n, 1);
        this->parent = vector<int>(n);

        int i = 0;
        generate(parent.begin(), parent.end(), [&]() {
            return i++;
        });

    }

    int find(int a) {
        // Find root
        int root = a;
        while (parent[root] != root)
            root = parent[root];

        // Compress path
        int current = a, next_elem;
        while (current != root) {
            next_elem = parent[current];
            parent[current] = root;
            current = next_elem;
        }

        return root;
    }

    int merge(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) // a and b are already merged
            return a;

        // Weighted union: update smaller component
        int a_size = size[a], b_size = size[b];
        if (a_size < b_size)
            swap(a, b);

        parent[b] = a;
        // Update size accordingly
        size[a] = a_size + b_size;
        return a;
    }
};

struct Edge {
    int src, dest, weight;
    Edge(int s, int d, int w) : src(s), dest(d), weight(w) {};
};

struct Graph {
    int num_vertices;
    vector<Edge> edges;

    explicit Graph(istream& in) {
        in >> num_vertices;

        // Undirected graph
        edges.reserve((num_vertices * (num_vertices - 1)) / 2);
        int weight;
        for (int row = 0; row < num_vertices; ++row) {
            for (int col = 0; col < num_vertices; ++col) {
                in >> weight;
                if (col > row)
                    edges.emplace_back(row, col, weight);
            }
        }
    };
};

/**
 * Implementation of Kruskal's algorithm for finding
 *  the minimum spanning tree.
 * @param graph represented by a collection of Nodes
 * @return the collection of Edges that corresponds to the MST
 */
vector<const Edge*> mst_kruskal(Graph& graph) {
    // Sort Edges by weight
    sort(graph.edges.begin(), graph.edges.end(), [](const Edge& e1, const Edge& e2) {return e1.weight < e2.weight;});

    UnionFind uf(graph.num_vertices);
    vector<const Edge*> mst;
    for (const Edge& e : graph.edges) {
        if (uf.find(e.src) != uf.find(e.dest)) {
            uf.merge(e.src, e.dest);
            mst.push_back(&e);
        }
    }

    return mst;
}

int main() {
    int num_cases;
    cin >> num_cases;
    for (int i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ":" << endl;
        Graph g(cin);
        auto mst = mst_kruskal(g);
        sort(mst.begin(), mst.end(), [](const Edge* e1, const Edge* e2) {
            return e1->src != e2->src ? e1->src < e2->src : e1->dest < e2->dest;
        });
        for (const Edge* e : mst)
            cout << e->src + 1 << " " << e->dest + 1 << endl;
    }

    return EXIT_SUCCESS;
}