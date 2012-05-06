#include "limitedPriorityQueue.hpp"

LimitedPriorityQueue::LimitedPriorityQueue(const size_t maxSize):
   m_size(maxSize){
};

LimitedPriorityQueue::~LimitedPriorityQueue(){};

void LimitedPriorityQueue::push(VertexDistPair a){
    if(m_queue.size() <= m_size){
        m_queue.push(a);
    }else if(std::get<1>(m_queue.top()) > std::get<1>(a)){
        m_queue.pop();
        m_queue.push(a);
    }
}
