/**
 * @file dag.h
 *
 */

#ifndef DAG_H_
#define DAG_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"

class DAG {
public:
    DAG(const Graph &data, const Graph &query, const CandidateSet &cs);
    ~DAG();

    inline Vertex GetRoot() const;
    inline Vertex GetSuccessor(Vertex u, size_t k) const;
    inline size_t GetNumSuccessor(Vertex u) const;
    inline Vertex GetPredecessor(Vertex u, size_t k) const;
    inline size_t GetNumPredecessor(Vertex u) const;
    inline size_t GetNumRemainingPredecessor(Vertex u) const;
    inline void AddNumRemainingPredecessor(Vertex u, size_t n);
    inline Vertex GetMatchingOrder(size_t n);
private:
    Vertex root_;
    std::vector<std::vector<Vertex>> successor_, predecessor_;
    std::vector<size_t> num_remaining_predecessor_;
    std::vector<Vertex> matching_order_;
};

inline Vertex DAG::GetRoot() const { return root_; }

inline Vertex DAG::GetSuccessor(Vertex u, size_t k) const { 
    return successor_[u][k];
}

inline size_t DAG::GetNumSuccessor(Vertex u) const { return successor_[u].size(); }

inline Vertex DAG::GetPredecessor(Vertex u, size_t k) const { 
    return predecessor_[u][k];
}

inline size_t DAG::GetNumPredecessor(Vertex u) const { return predecessor_[u].size(); }

inline size_t DAG::GetNumRemainingPredecessor(Vertex u) const { return num_remaining_predecessor_[u]; }

inline void DAG::AddNumRemainingPredecessor(Vertex u, size_t n) { num_remaining_predecessor_[u] += n; }

inline Vertex DAG::GetMatchingOrder(size_t n) { return matching_order_[n]; }
#endif // DAG_H_