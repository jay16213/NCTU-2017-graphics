#ifndef VIEWLOADER_H
#define VIEWLOADER_H

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cstdio>
#include <string>
#include "Coord3.h"
using namespace std;

class ViewLoader{
    public:
        ViewLoader();
        ~ViewLoader();
        
        int loadView(string view_file);
        void updateUnitVat();
        void updateDistance();
        void updateRight();

        Coord3d mEye;
        Coord3d mVat;
        Coord3d mRight;
        Coord3d mUnitVat;//unit vector which from eye to mVat
        Coord3d mVup;
        Coord3d mNormal;
        double mDistance;
        double mFovy;
        double mDnear;
        double mDfar;
        int mViewport[4];
        double mAspect;
};

#endif