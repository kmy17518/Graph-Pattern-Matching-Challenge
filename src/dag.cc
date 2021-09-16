/**
 * @file dag.cc
 *
 */

#include "dag.h"

using namespace std;

void CalculateCoreValues(const Graph &query, vector<size_t> &core_value);

DAG::DAG(const Graph &data, const Graph &query, const CandidateSet &cs) {
    successor_.resize(query.GetNumVertices());
    predecessor_.resize(query.GetNumVertices());
    num_remaining_predecessor_. resize(query.GetNumVertices(), 0);
    matching_order_.resize(query.GetNumVertices(), -1);

    // Select root
    root_ = 0;
    for(size_t i = 1; i < query.GetNumVertices(); i++)
        if(cs.GetCandidateSize(i) * query.GetDegree(root_)
            < query.GetDegree(i) * cs.GetCandidateSize(root_))
            root_ = i;

    //Calculate Core Values
    vector<size_t> core_value(query.GetNumVertices(), 0);
    CalculateCoreValues(query, core_value);

    /*
    cout << "core ";
    for(size_t i = 0; i < query.GetNumVertices(); i++)
        cout << core_value[i] << " ";
    cout << "\n";
    //*/

    // Calculate Candidate Size and Make 'compare'
    vector<size_t> candidate_size;
    for(size_t i = 0; i < query.GetNumVertices(); i++)
        candidate_size.push_back(cs.GetCandidateSize(i));

    auto compare = [candidate_size, core_value](Vertex u, Vertex v) {
        return candidate_size[u] * core_value[v] > candidate_size[v] * core_value[u];
    };
    
    // BFS
    priority_queue<Vertex, vector<Vertex>, decltype(compare)> pq(compare);
    vector<size_t> check(query.GetNumVertices(), 0); // 0 : not pushed
                                                     // 1 : pushed but not poped
    pq.push(root_);                                  // 2 : poped
    check[root_] = 1;
    for(size_t i = 0; !pq.empty(); i++) {
        Vertex v = pq.top();
        pq.pop();
        matching_order_[i] = v;
        check[v] = 2;
        for(size_t j = query.GetNeighborStartOffset(v);
            j < query.GetNeighborEndOffset(v); j++) {
            Vertex u = query.GetNeighbor(j);
            if(check[u] < 2) {
                successor_[v].push_back(u);
                predecessor_[u].push_back(v);
                num_remaining_predecessor_[u]++;
                if(check[u] == 0) {
                    pq.push(u);
                    check[u] = 1;
                }
            }
        }
    }
}

DAG::~DAG() {} 

void CalculateCoreValues(const Graph &query, vector<size_t> &core_value) {
    size_t max_degree = 0;

    for(size_t i = 0; i < query.GetNumVertices(); i++) {
        core_value[i] = query.GetDegree(i);
        if(core_value[i] > max_degree)
            max_degree = core_value[i];
    }

    vector<size_t> bin(max_degree + 1, 0);

    for(size_t i = 0; i < query.GetNumVertices(); i++)
        bin[core_value[i]]++;

    size_t num = 0;
    size_t start = 0;
    for(size_t i = 0; i < max_degree + 1; i++) {
        num = bin[i];
        bin[i] = start;
        start += num;
    }

    vector<size_t> pos;
    vector<size_t> vert(query.GetNumVertices());

    for(size_t i = 0; i < query.GetNumVertices(); i++) {
        pos.push_back(bin[core_value[i]]);
        vert[pos[i]] = i;
        bin[core_value[i]]++;
    }

    for(size_t i = max_degree; i > 0; i--)
        bin[i] = bin[i - 1];
    
    bin[0] = 1;

    size_t w = 0;
    size_t du = 0;
    size_t pu = 0;
    size_t pw = 0;

    for(size_t i = 0; i < query.GetNumVertices(); i++) {
        size_t v = vert[i];
        size_t start_offset = query.GetNeighborStartOffset(i);
        size_t end_offset = query.GetNeighborEndOffset(i);
        for(size_t j = start_offset; j < end_offset; j++) {
            size_t u = query.GetNeighbor(j);
            if(core_value[u] > core_value[v]) {
                du = core_value[u];
                pu = pos[u];
                pw = bin[du];
                w = vert[pw];
                if(u != w) {
                    pos[u] = pw;
                    vert[pu] = w;
                    pos[w] = pu;
                    vert[pw] = u;
                }
                bin[du]++;
                core_value[u]--;
            }
        }
    }
}