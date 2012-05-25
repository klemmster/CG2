#ifndef DOMAIN_HPP_GEPFEOX5
#define DOMAIN_HPP_GEPFEOX5

#include <vector>
#include <unistd.h>

class Domain
{
public:
    Domain(){};
    Domain (std::vector<float> mins, std::vector<float> maxs);
    virtual ~Domain () {};

    const float getMin(size_t dim) const { return m_Mins.at(dim); };
    const float getMax(size_t dim) const { return m_Maxs.at(dim); };

    void updateMin(float val, size_t dim){ m_Mins.at(dim) = val; };
    void updateMax(float val, size_t dim){ m_Maxs.at(dim) = val; };
    //TODO
    void draw() {};

private:
    std::vector<float> m_Mins;
    std::vector<float> m_Maxs;
};


#endif /* end of include guard: DOMAIN_HPP_GEPFEOX5 */

