#ifndef MODEL_H
#define MODEL_H

#include "Coord3.h"

class Model
{
public:
    Model() {}
    Model(int objindex, float angle, Coord3f rAv, Coord3f scale, Coord3f transfer)
    {
        mObjIndex = objindex;
        mAngle = angle;
        mRotateAxisVec = rAv;
        mScale = scale;
        mTransfer = transfer;
    }
    ~Model() {}

    Model& operator=(const Model rhs)
    {
        mObjIndex = rhs.mObjIndex;
        mAngle = rhs.mAngle;
        mRotateAxisVec = rhs.mRotateAxisVec;
        mScale = rhs.mScale;
        mTransfer = rhs.mTransfer;
        return *this;
    }

    int mObjIndex;          // the index to find the obj filename
    float mAngle;           // rotate angle
    Coord3f mRotateAxisVec; // rotate axis vector
    Coord3f mScale;         // scale value of each direction
    Coord3f mTransfer;      // the transfer vector
};

#endif
