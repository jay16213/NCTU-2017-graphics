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
        
        Coord3 mEye;
        Coord3 mVat;
        Coord3d mUnitVat;//unit vector of mVat
        Coord3 mVup;
        double mFovy;
        double mDnear;
        double mDfar;
        int mViewport[4];
        double mAspect;
};

#endif