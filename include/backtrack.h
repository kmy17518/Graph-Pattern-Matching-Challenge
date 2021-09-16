/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "dag.h"
#include "graph.h"

class Backtrack {
public:
    Backtrack(clock_t start);
    ~Backtrack();

    void PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs);
private:
    clock_t start_time_;
    std::vector<Vertex> Matching;
    std::vector<bool> isMatched_;
    std::vector<Vertex> data_matched_vertex_;
    size_t num_correct_embeddings_;
    size_t num_wrong_embeddings_;

    void FindMatches(const Graph &data, const Graph &query,
                     const CandidateSet &cs, DAG &queryDAG, size_t level);
    bool IsCorrect(const Graph &data, const Graph &query, const CandidateSet &cs) const;
    inline void print() const;
};

inline void Backtrack::print() const {
    std::cout << "correct: "<< num_correct_embeddings_ << "\n";
    std::cout << "wrong: "<< num_wrong_embeddings_ << "\n";
    std::cout << static_cast<double>(clock() - start_time_)/CLOCKS_PER_SEC << "s\n";
}

#endif  // BACKTRACK_H_
