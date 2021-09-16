/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

using namespace std;

Backtrack::Backtrack(clock_t start) { start_time_ = start; }
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
    cout << "t " << query.GetNumVertices() << "\n";

    DAG queryDAG(data, query, cs);

    /*
    cout << "matching ";
    for(size_t i = 0; i < query.GetNumVertices(); i++) {
        cout << queryDAG.GetMatchingOrder(i) << " ";
    }
    cout << "\n";
    //*/

    Matching.resize(query.GetNumVertices(), -1);
    isMatched_.resize(query.GetNumVertices(), false);
    data_matched_vertex_.resize(data.GetNumVertices(), false);
    num_correct_embeddings_ = num_wrong_embeddings_ = 0;

    FindMatches(data, query, cs, queryDAG, 0);

    //print();
}
/*
bool Backtrack::IsCorrect(const Graph &data, const Graph &query, const CandidateSet &cs) const{
    bool ans = true;
    for(size_t i = 0; i < query.GetNumVertices(); i++) {
        ans = ans && (query.GetLabel(i) == data.GetLabel(Matching[i]));
        if(!ans) {
            //cout << "label " << i << "\n";
            return ans;
        }
        for(size_t j = i + 1; j < query.GetNumVertices(); j++) {
            ans = ans && (Matching[i] != Matching[j]);
            if(!ans) {
                //cout << "not injective " << i << j << "\n";
                return ans;
            }
            if(query.IsNeighbor(i, j))
                ans = ans && data.IsNeighbor(Matching[i], Matching[j]);
            if(!ans) {
                //cout << "not neighbor " << i << j << "\n";
                return ans;
            }
        }
    }
    return ans;
}
*/
void Backtrack::FindMatches(const Graph &data, const Graph &query,
                            const CandidateSet &cs, DAG &queryDAG, size_t level) {
    if(level == query.GetNumVertices()) {
        
        /*
        if(IsCorrect(data, query, cs)) {
            num_correct_embeddings_++;
            //cout << "correct: ";
        }
        else {
            num_wrong_embeddings_++;
            cout << "wrong: ";
            exit(0);
        }
        //*/
        num_correct_embeddings_++;

        //*
        cout << "a ";
        for(size_t i = 0; i < query.GetNumVertices(); i++)
            cout << Matching[i] << " ";
        cout << "\n";
        //*/

        
        if(num_correct_embeddings_ >= 100000) {
            //print();
            exit(EXIT_SUCCESS);
        }
        
        return;
    }

    Vertex v = queryDAG.GetMatchingOrder(level);

    for(size_t i = 0; i < cs.GetCandidateSize(v); i++) {
        Matching[v] = cs.GetCandidate(v, i);
        if(data_matched_vertex_[Matching[v]]) {
            Matching[v] = -1;
            continue;
        }

        isMatched_[v] = true;
        for(size_t j = 0; j < queryDAG.GetNumPredecessor(v); j++) {
            if(!data.IsNeighbor(Matching[queryDAG.GetPredecessor(v, j)], Matching[v])) {
                isMatched_[v] = false;
                break;
            }
        }

        if(isMatched_[v]) {
            data_matched_vertex_[Matching[v]] = true;
            FindMatches(data, query, cs, queryDAG, level + 1);
            data_matched_vertex_[Matching[v]] = false;
        }
        Matching[v] = -1;
        isMatched_[v] = false;
    }
    //*
    if(static_cast<double>(clock() - start_time_)/CLOCKS_PER_SEC > 10) {
        //print();
        exit(EXIT_SUCCESS);
    }
    //*/
}