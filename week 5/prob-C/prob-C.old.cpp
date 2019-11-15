#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <limits>
#include <queue>

using namespace std;

class Edge {
public:
    int src, dest, cap;     // source, destination, capacity
    int flow = 0;           // flow (may be negative if in opposite direction)
    Edge * reverse = nullptr;

    Edge(int src, int dst, int cap) : src(src), dest(dst), cap(cap) {}

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
    int n_vertices;
    int n_edges;
    multimap<int, int> connections;
    multimap<int, Edge*> edges;

    int source, sink;

public:
    explicit TestCase(istream& in) {
        in >> n_vertices >> n_edges;
        this->source = 1;
        this->sink = this->n_vertices;

        int a, b;
        for (int i = 0; i < n_edges; ++i) {
            in >> a >> b;
            connections.insert(make_pair(a, b));
        }

        make_flow_network();
    }

    /**
     * Convert each node in two nodes (one for input and another for output),
     *  with a 1-capacity edge between them, to control output flow.
     * And construct all edges.
     */
    void make_flow_network() {

        // Create an auxiliary output node for each vertex (for all vertices except source or sink)
        for (int i = 2; i <= this->n_vertices - 1; ++i) {
            auto edge_pair = Edge::construct_undirected_pair(i, vertex_to_aux_vertex_id(i), 1);
            edges.insert(make_pair(edge_pair.first->src, edge_pair.first));
            edges.insert(make_pair(edge_pair.second->src, edge_pair.second));
        }

        // Connect nodes
        for (auto p : connections) {
            auto edge_pair = construct_edge_pair(p.first, p.second, 1);

            edges.insert(make_pair(edge_pair.first->src, edge_pair.first));
            edges.insert(make_pair(edge_pair.second->src, edge_pair.second));
        }

        // New total vertex count
        this->n_vertices = 2 + (this->n_vertices - 2) * 2;

//        // Print all edges
//        for (auto p : edges) {
//            if (p.second->cap > 0)
//                cout << p.second->src << " -> " << p.second->dest << endl;
//        }
    }

    /**
     * All incoming edges go to regular node, all outgoing edges come from auxiliary outgoing node
     * (except for graph's source).
     */
    pair<Edge*, Edge*> construct_edge_pair(int src, int dst, int cap) {
        if (src > dst) swap(src, dst);

        Edge* e1;
        if (src == this->source || src == this->sink) {
            e1 = new Edge(src, dst, cap);
        } else {
            e1 = new Edge(vertex_to_aux_vertex_id(src), dst, cap);
        }

        Edge* e2;
        if (dst == this->source || dst == this->sink) {
            e2 = new Edge(dst, src, cap);
        } else {
            e2 = new Edge(vertex_to_aux_vertex_id(dst), src, cap);
        }
        e1->reverse = e2;
        e2->reverse = e1;

        this->edges.insert(make_pair(e1->src, e1));
        this->edges.insert(make_pair(e2->src, e2));
        return make_pair(e1, e2);
    }

    int vertex_to_aux_vertex_id(int vertex) {
        assert(vertex > this->source and vertex < this->sink);
        return this->sink - this->source + vertex;
    }

    string solve() {
        int max_flow = edmonds_karp(this->edges, this->n_vertices + 1, this->source, this->sink);
        return to_string(max_flow);
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
