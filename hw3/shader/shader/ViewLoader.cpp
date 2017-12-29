#include "ViewLoader.h"
#include <iostream>
#include <cmath>
ViewLoader::ViewLoader() {}
ViewLoader::~ViewLoader() {}

int ViewLoader::loadView(string view_file)
{
    ifstream view(view_file, ios::in);
    if(view.fail())
    {
        cout << "Can not open the view file '" << view_file << "'" << endl;
        return -1;
    }
    
    char param_name[20];
    while(!view.eof())
    {
        memset(param_name, 0, 20);
        view >> param_name;
        
        if(strcmp(param_name, "eye") == 0)
        {
            double x, y, z;
            view >> x >> y >> z;
            mEye = Coord3d(x, y, z);
        }
        else if(strcmp(param_name, "vat") == 0)
        {
            double x, y, z;
            view >> x >> y >> z;
            mVat = Coord3d(x, y, z);

            Coord3d v(mVat[0] - mEye[0], mVat[1] - mEye[1], mVat[2] - mEye[2]);
            double len = v.vecLen();

            for (int i = 0; i < 3; i++)
                mUnitVat[i] = (double) (v[i] / len);

            mDistance = len;
        }
        else if(strcmp(param_name, "vup") == 0)
        {
            double x, y, z;
            view >> x >> y >> z;
            mVup = Coord3d(x, y, z);

            Coord3d t(
                mUnitVat[1] * mVup[2] - mUnitVat[2] * mVup[1],
                mUnitVat[2] * mVup[0] - mUnitVat[0] * mVup[2],
                mUnitVat[0] * mVup[1] - mUnitVat[1] * mVat[0]
            );
            Coord3d normal(
                mUnitVat[1] * t[2] - mUnitVat[2] * t[1],
                mUnitVat[2] * t[0] - mUnitVat[0] * t[2],
                mUnitVat[0] * t[1] - mUnitVat[1] * t[0]
            );

            double l = normal.vecLen();
            for (int i = 0; i < 3; i++) mNormal[i] = (double)normal[i] / l;
        }
        else if(strcmp(param_name, "fovy") == 0)
        {
            double theta;
            view >> theta;
            mFovy = theta;
        }
        else if(strcmp(param_name, "dnear") == 0)
        {
            double d;
            view >> d;
            mDnear = d;
        }
        else if(strcmp(param_name, "dfar") == 0)
        {
            double d;
            view >> d;
            mDfar = d;
        }
        else if(strcmp(param_name, "viewport") == 0)
        {
            int x, y, w, h;
            view >> x >> y >> w >> h;
            mViewport[0] = x;
            mViewport[1] = y;
            mViewport[2] = w;
            mViewport[3] = h;
            mAspect = (double) w / h;
        }
    }
    
    view.close();
    return 0;
}

void ViewLoader::updateUnitVat()
{
    Coord3d v(mVat[0] - mEye[0], mVat[1] - mEye[1], mVat[2] - mEye[2]);
    
    v.normalize();
    mUnitVat = v;
    //printf("uptate unit: %f %f %f\n", mUnitVat[0], mUnitVat[1], mUnitVat[2]);
}

void ViewLoader::updateDistance()
{
    Coord3d v(mVat[0] - mEye[0], mVat[1] - mEye[1], mVat[2] - mEye[2]);
    mDistance = v.vecLen();
    //cout << "update dis " << mDistance << endl;
    return;
}

void ViewLoader::updateRight()
{
    Coord3d v(mVat[0] - mEye[0], mVat[1] - mEye[1], mVat[2] - mEye[2]);
    
    //vat cross vup
    mRight[0] = (v[1] * mVup[2]) - (v[2] * mVup[1]);//y1z2 - z1y2
    mRight[1] = (v[2] * mVup[0]) - (v[0] * mVup[2]);//z1x2 - x1z2
    mRight[2] = (v[0] * mVup[1]) - (v[1] * mVup[0]);//y1z2 - z1y2

    mRight.normalize();
    return;
}