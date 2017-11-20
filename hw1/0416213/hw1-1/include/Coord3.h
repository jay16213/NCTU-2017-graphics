#ifndef COORD3_H
#define COORD3_H

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

    inline float& operator[](size_t index)
    {
        return ptr[index];
    }
};

#endif