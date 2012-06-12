#include "normal.hpp"

Normal::Normal(const vec3f normal)
    : vec3f(normal)
{
    m_Color = vec3f(0, 1, 0);
}

Normal::Normal(const float dirX, const float dirY, const float dirZ)
    : vec3f(dirX, dirY, dirZ)
{
    m_Color = vec3f(0, 1, 0);
}

Normal::Normal(const float dirX, const float dirY, const float dirZ, const vec3f color)
    : vec3f(dirX, dirY, dirZ),
      m_Color(color)
{

}

Normal::~Normal()
{
    //dtor
}
