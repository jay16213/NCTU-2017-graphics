#ifndef COORD3_H
#define COORD3_H

#include <cmath>

template<typename T>
class Coord3
{
public:
    T ptr[3];

    Coord3() {}
    Coord3(T c1, T c2, T c3)
    {
        ptr[0] = c1;
        ptr[1] = c2;
        ptr[2] = c3;
    }
    Coord3(T *p)
    {
        for (int i = 0; i < 3; i++) ptr[i] = p[i];
    }
    ~Coord3() {}

    Coord3& operator= (const Coord3 rhs)
    {
        for (int i = 0; i < 3; i++)
            ptr[i] = rhs.ptr[i];
        return *this;
    }

    Coord3 operator+ (const Coord3 rhs)
    {
        T ans[3];
        for (int i = 0; i < 3; i++)
            ans[i] = ptr[i] + rhs.ptr[i];
        return Coord3(ans);
    }

    Coord3 operator- (const Coord3 rhs)
    {
        T ans[3];
        for (int i = 0; i < 3; i++)
            ans[i] = ptr[i] - rhs.ptr[i];
        return Coord3(ans);
    }

    inline T& operator[](size_t index)
    {
        return ptr[index];
    }

    T vecLen()
    {
        T len2 = (ptr[0] * ptr[0]) + (ptr[1] * ptr[1]) + (ptr[2] * ptr[2]);
        return sqrt(len2);
    }
};

#endif