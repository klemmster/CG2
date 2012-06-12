#ifndef NORMAL_H
#define NORMAL_H

#include <vector>
#include <memory>
#include "vec.hpp"

using namespace std;

class Normal;
typedef shared_ptr<Normal> NormalPtr;
typedef vector<NormalPtr> NormalList;
typedef shared_ptr<NormalList> NormalListPtr;

class Normal: vec3f
{
    public:
        Normal(const vec3f normal);
        Normal(const float dirX, const float dirY, const float dirZ);
        Normal(const float dirX, const float dirY, const float dirZ, const vec3f color);
        virtual ~Normal();

    protected:
    private:

        Normal() {};
        vec3f m_Color;
};

#endif // NORMAL_H
