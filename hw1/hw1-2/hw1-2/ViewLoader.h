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

        Coord3<double> mEye;
        Coord3<double> mVat;
        Coord3<double> mUnitVat;//unit vector which from eye to mVat
        Coord3<double> mVup;
        Coord3<double> mNormal;
        double mDistance;
        double mFovy;
        double mDnear;
        double mDfar;
        int mViewport[4];
        double mAspect;
};

#endif