#ifndef LIMITEDPRIORITYQUEUE_HPP_CK3XG1TH
#define LIMITEDPRIORITYQUEUE_HPP_CK3XG1TH

#include <cstddef>
#include <queue>
#include <vector>

#include "vertex.hpp"

//Vertex thats possibly a KNearestResult, float = distance
typedef std::pair<VertexPtr, float> VertexDistPair;

class CompareVertexDistPair{
    public:
        bool operator()(const VertexDistPair& a, const VertexDistPair& b){
            if(std::get<1>(a) < std::get<1>(b)){
                return true;
            }
            return false;
        }
};

class LimitedPriorityQueue
{
public:
    LimitedPriorityQueue (const size_t maxSize);
    void push(VertexDistPair a);
    bool empty() const { return m_queue.empty(); };
    bool full() const { return m_queue.size() >= m_size; };
    VertexDistPair top(){ return m_queue.top(); };
    void pop() { m_queue.pop(); };
    virtual ~LimitedPriorityQueue ();

private:
    const size_t m_size;
    std::priority_queue<VertexDistPair, std::vector<VertexDistPair>,
        CompareVertexDistPair> m_queue;
};

#endif /* end of include guard: LIMITEDPRIORITYQUEUE_HPP_CK3XG1TH */

