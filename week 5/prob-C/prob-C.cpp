#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <limits>
#include <queue>

using namespace std;

// https://stackoverflow.com/questions/37096772/removing-minimum-no-of-edges-to-disconnect-two-vertices-in-a-graph

class Edge {
    Edge(int src, int dst, int cap) : src(src), dest(dst), cap(cap) {}

public:
    int src, dest, cap;    // source, destination, capacity
    int flow = 0;           // flow (may be negative if in opposite direction)
    Edge * reverse = nullptr;

    static pair<Edge*, Edge*> construct_undirected_pair(int node1, int node2, int cap) {
        Edge* e1 = new Edge(node1, node2, cap);
        Edge* e2 = new Edge(node2, node1, cap);
        e1->reverse = e2;
        e2->reverse = e1;
        return make_pair(e1, e2);
    }
};

/**
 * Executes breadth-first search on the given graph (defined by <edges, n_vertices>),
 *  and populates the predecessors vector.
 * This returns the shortest path for unweighted graphs.
 * @param edges
 * @param n_vertices
 * @param predecessors
 */
void breadth_first_search(const multimap<int, Edge*>& edges, vector<Edge*>& predecessors, int n_vertices, int src) {
    predecessors.clear();
    predecessors.resize(n_vertices, nullptr);

    queue<int> q;
    q.push(src);
    while (! q.empty()) {
        int curr = q.front(); q.pop();
        for (auto it = edges.equal_range(curr); it.first != it.second; ++it.first) {
            Edge* e = it.first->second;
            if (predecessors[e->dest] == nullptr and e->dest != src and e->cap > e->flow) {
                predecessors[e->dest] = e;
                q.push(e->dest);
            }
        }
    }
}

/**
 * Executes Edmonds-Karp algorithm for finding maximum flow in a graph.
 * @param edges
 * @param n_vertices
 * @return the maximum flow, or -1 if source and sink are not connected
 */
int edmonds_karp(const multimap<int, Edge*>& edges, int n_vertices, int src, int sink) {
    int flow = 0;
    bool first_iter = true;

    // Predecessors
    vector<Edge*> predecessors;
    do {
        // 1. Run BFS to find the shortest path from source to sink
        breadth_first_search(edges, predecessors, n_vertices, src);

        if (first_iter) {
            if (predecessors[sink] == nullptr) return -1;
            first_iter = false;
        }

        // 2. If an augmenting path was found
        if (predecessors[sink] != nullptr) {
            int df = numeric_limits<int>::max();
            // 2.1. See how much flow we can send
            for (Edge * e = predecessors[sink]; e != nullptr; e = predecessors[e->src])
                df = min(e->cap - e->flow, df);

            // 2.2. And update edge by that amount
            for (Edge * e = predecessors[sink]; e != nullptr; e = predecessors[e->src]) {
                e->flow += df;
                e->reverse->flow -= df;
            }

            flow += df;
        }
    } while (predecessors[sink] != nullptr);

    return flow;
}

class TestCase {

public:
    explicit TestCase(istream& in) {

    }

    string solve() {
        // TODO we want minimum cut of the graph, which is the max-flow of the graph;

        return "impossibruuu";
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    uint num_cases;
    cin >> num_cases;
    for (uint i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}
