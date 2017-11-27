#ifndef COORD3_H
#define COORD3_H

#include <cmath>

class Coord3
{
public:
    float ptr[3];

    Coord3() {}
    Coord3(float c1, float c2, float c3)
    {
        ptr[0] = c1;
        ptr[1] = c2;
        ptr[2] = c3;
    }
    Coord3(float *p)
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
        float ans[3];
        for (int i = 0; i < 3; i++)
            ans[i] = ptr[i] + rhs.ptr[i];
        return Coord3(ans);
    }

    Coord3 operator- (const Coord3 rhs)
    {
        float ans[3];
        for (int i = 0; i < 3; i++)
            ans[i] = ptr[i] - rhs.ptr[i];
        return Coord3(ans);
    }

    inline float& operator[](size_t index)
    {
        return ptr[index];
    }
};

class Coord3d
{
public:
    double ptr[3];

    Coord3d() {}
    Coord3d(double c1, double c2, double c3)
    {
        ptr[0] = c1;
        ptr[1] = c2;
        ptr[2] = c3;
    }
    Coord3d(double *p)
    {
        for (int i = 0; i < 3; i++) ptr[i] = p[i];
    }
    ~Coord3d() {}

    Coord3d& operator= (const Coord3 rhs)
    {
        for (int i = 0; i < 3; i++)
            ptr[i] = rhs.ptr[i];
        return *this;
    }

    Coord3d operator+ (const Coord3 rhs)
    {
        double ans[3];
        for (int i = 0; i < 3; i++)
            ans[i] = ptr[i] + rhs.ptr[i];
        return Coord3d(ans);
    }

    Coord3d operator- (const Coord3 rhs)
    {
        double ans[3];
        for (int i = 0; i < 3; i++)
            ans[i] = ptr[i] - rhs.ptr[i];
        return Coord3d(ans);
    }

    double vecLen()
    {
        double len2 = (ptr[0] * ptr[0]) + (ptr[1] * ptr[1]) + (ptr[2] * ptr[2]);
        return sqrt(len2);
    }

    inline double &operator[](size_t index)
    {
        return ptr[index];
    }
};

#endif