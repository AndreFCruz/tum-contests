#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <limits>
#include <queue>

using namespace std;

class Edge {
    Edge(int src, int dst, int cap) : src(src), dest(dst), cap(cap) {}

public:
    int src, dest, cap;    // source, destination, capacity
    int flow = 0;           // flow (may be negative if in opposite direction)
    Edge * reverse = nullptr;

    static pair<Edge*, Edge*> construct_directed_pair(int from, int to, int cap) {
        Edge* e1 = new Edge(from, to, cap);
        Edge* e2 = new Edge(to, from, 0);
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

    return (int) flow;
}

class TestCase {
    int n_people, n_dishes, n_beverages;
    vector<int> dish_prefs;     // dish preferences
    vector<int> beverage_prefs; // beverage preferences

    int n_vertices;
    int source, sink;
    multimap<int, Edge*> edges;

public:
    explicit TestCase(istream& in) {
        in >> n_people >> n_dishes >> n_beverages;

        dish_prefs.resize(n_people);
        beverage_prefs.resize(n_people);
        for (int i = 0; i < n_people; ++i)
            in >> dish_prefs[i] >> beverage_prefs[i];

        make_flow_network();
    }

    ~TestCase() {
        // Delete all allocated edges
        for (auto p : edges)
            delete p.second;
    }

    void make_flow_network() {
        this->n_vertices = this->n_people + 2 * this->n_dishes + this->n_beverages;

        // Connect a new arbitrary source node to all people (consumers)
        this->source = this->n_vertices++;
        for (int i = 0; i < this->n_people; ++i) {
            auto p = Edge::construct_directed_pair(this->source, person_to_vertex_id(i), 1);
            edges.insert(make_pair(p.first->src, p.first));
            edges.insert(make_pair(p.second->src, p.second));
        } // NOTE undirected edges here are useless (and possibly incorrect?)

        // Connect all dishes to an auxiliary vertex to control outgoing flow
        for (int i = 1; i <= this->n_dishes; ++i) {
            int dish_vertex = dish_to_vertex_id(i);
            int aux_vertex = dish_to_aux_vertex_id(i);
            auto edg = Edge::construct_directed_pair(dish_vertex, aux_vertex, 1);
            edges.insert(make_pair(edg.first->src, edg.first));
            edges.insert(make_pair(edg.second->src, edg.second));
        }

        // Connect all people to their favoured dishes; and favoured dishes' auxiliary outgoing vertex to beverages
        // (creates constraint that both must be correct for flow to pass)
        for (int i = 0; i < this->n_people; ++i) {
            int dish_vertex = dish_to_vertex_id(dish_prefs[i]);
            int dish_aux_vertex = dish_to_aux_vertex_id(dish_prefs[i]);
            int bev_vertex = beverage_to_vertex_id(beverage_prefs[i]);
            auto d_edge = Edge::construct_directed_pair(person_to_vertex_id(i), dish_vertex, 1);
            auto b_edge = Edge::construct_directed_pair(dish_aux_vertex, bev_vertex, 1);

            // Dish preference (Person -> Dish)
            edges.insert(make_pair(d_edge.first->src, d_edge.first));
            edges.insert(make_pair(d_edge.second->src, d_edge.second));

            // Beverage preference (Dish -> Beverage)
            edges.insert(make_pair(b_edge.first->src, b_edge.first));
            edges.insert(make_pair(b_edge.second->src, b_edge.second));
        }

        // Connect all beverages to a new arbitrary sink node
        this->sink = this->n_vertices++;
        for (int i = 1; i <= this->n_beverages; ++i) {
            auto p = Edge::construct_directed_pair(beverage_to_vertex_id(i), this->sink, 1);
            edges.insert(make_pair(p.first->src, p.first));
            edges.insert(make_pair(p.second->src, p.second));
        }

//        // Print all edges
//        for (auto p : edges) {
//            if (p.second->cap > 0)
//                cout << p.second->src << " -> " << p.second->dest << endl;
//        }
    }

    /**
     *
     * @param person numbered 0 to number-of-people
     * @return
     */
    int person_to_vertex_id(int person) {
        assert(person >= 0 and person <= this->n_people - 1);
        // ids in [0; n_people[
        return person; // 0-indexed person id
    }

    /**
     *
     * @param dish numbered 1 to number-of-dishes
     * @return corresponding vertex id, [n_people, n_people + 2 * dishes - 1]
     */
    int dish_to_vertex_id(int dish) {
        assert(dish >= 1 and dish <= this->n_dishes);
        // ids in [n_people; n_people + n_dishes[
        return this->n_people + 2 * (dish - 1);
    }

    /**
     * Auxiliary vertex to control outgoing flow of a dish node
     * @param dish numbered 1 to number-of-dishes
     * @return corresponding vertex id, [n_people + 1, n_people + 2 * dishes]
     */
    int dish_to_aux_vertex_id(int dish) {
        assert(dish >= 1 and dish <= this->n_dishes);
        return this->dish_to_vertex_id(dish) + 1;
    }

    /**
     *
     * @param beverage numbered 1 to number-of-beverages
     * @return corresponding vertex id, [n_people + 2 * dishes, n_people + 2 * dishes + n_]
     */
    int beverage_to_vertex_id(int beverage) {
        assert(beverage >= 1 and beverage <= this->n_beverages);
        // ids in [n_people + n_dishes; n_people + n_dishes + n_beverages[
        return this->n_people + 2 * this->n_dishes + beverage - 1;
    }

    string solve() {
        // Bipartite matching problem
        int flow = edmonds_karp(this->edges, this->n_vertices, this->source, this->sink);
        return to_string(flow);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    int num_cases;
    cin >> num_cases;
    for (int i = 0; i < num_cases; ++i) {
        cout << "Case #" << i + 1 << ": " << TestCase(cin).solve() << endl;
    }

    return EXIT_SUCCESS;
}
