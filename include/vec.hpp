#ifndef _VEC_HPP
#define _VEC_HPP

#include <iostream>
#include <cmath>

class vec3f
{

public:
    float _v[3];

    vec3f()
    {
        _v[0] = 0;
        _v[1] = 0;
        _v[2] = 0;
    }

    vec3f(float x, float y, float z)
    {
        _v[0] = x;
        _v[1] = y;
        _v[2] = z;
    }

    float operator[] (int i) const
    {
        return _v[i];
    }

    float& operator[] (int i)
    {
        return _v[i];
    }

    float* p()
    {
        return _v;
    }

    const float* p() const
    {
        return _v;
    }

    const float &x() const { return _v[0]; }
    const float &y() const { return _v[1]; }
    const float &z() const { return _v[2]; }

};

inline vec3f operator/ (const vec3f& n, float d)
{
    return vec3f(n[0]/d, n[1]/d, n[2]/d);
}

inline vec3f operator* (const vec3f& n, float d)
{
    return vec3f(n[0]*d, n[1]*d, n[2]*d);
}

inline vec3f operator* (float d, const vec3f& n)
{
    return vec3f(n[0]*d, n[1]*d, n[2]*d);
}

inline vec3f operator+ (const vec3f& a, const vec3f& b)
{
    return vec3f(a[0]+b[0], a[1]+b[1], a[2]+b[2]);
}

inline vec3f operator- (const vec3f& a, const vec3f& b)
{
    return vec3f(a[0]-b[0], a[1]-b[1], a[2]-b[2]);
}

inline std::ostream& operator<< (std::ostream& s, const vec3f& v)
{
    return (s << v[0] << ", " << v[1] << ", " << v[2]);
}

inline vec3f cross(const vec3f& a, const vec3f& b)
{
    return vec3f(a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]);
}

inline float dot(const vec3f& a, const vec3f& b)
{
    return (a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);
}

inline float norm(const vec3f& v)
{
    return std::sqrt(dot(v,v));
}

inline vec3f normalize(const vec3f& v)
{
    float l = norm(v);
    if (l == 0) return v;
    return (v / l);
}

inline vec3f pairMult(const vec3f& a, const vec3f& b){
    return vec3f(a._v[0] * b._v[0],
                 a._v[1] * b._v[1],
                 a._v[2] * b._v[2]);
    }

#endif
