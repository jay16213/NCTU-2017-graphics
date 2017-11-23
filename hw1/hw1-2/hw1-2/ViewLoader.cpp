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
            mEye = Coord3<double>(x, y, z);
            printf("eye: %f %f %f\n", mEye[0], mEye[1], mEye[2]);
        }
        else if(strcmp(param_name, "vat") == 0)
        {
            double x, y, z;
            view >> x >> y >> z;
            mVat = Coord3<double>(x, y, z);
            printf("vat: %f %f %f\n", mVat[0], mVat[1], mVat[2]);

            Coord3<double> v(mVat[0] - mEye[0], mVat[1] - mEye[1], mVat[2] - mEye[2]);
            double len = v.vecLen();
            printf("%f %f %f\n", v[0], v[1], v[2]);
            for (int i = 0; i < 3; i++)
                mUnitVat[i] = (double) (v[i] / len);

            mDistance = len;

            printf("unit: %f %f %f\n", mUnitVat[0], mUnitVat[1], mUnitVat[2]);
        }
        else if(strcmp(param_name, "vup") == 0)
        {
            double x, y, z;
            view >> x >> y >> z;
            mVup = Coord3<double>(x, y, z);
            printf("vup: %f %f %f\n", mVup[0], mVup[1], mVup[2]);

            Coord3<double> t(
                mUnitVat[1] * mVup[2] - mUnitVat[2] * mVup[1],
                mUnitVat[2] * mVup[0] - mUnitVat[0] * mVup[2],
                mUnitVat[0] * mVup[1] - mUnitVat[1] * mVat[0]
            );
            Coord3<double> normal(
                mUnitVat[1] * t[2] - mUnitVat[2] * t[1],
                mUnitVat[2] * t[0] - mUnitVat[0] * t[2],
                mUnitVat[0] * t[1] - mUnitVat[1] * t[0]
            );

            double l = normal.vecLen();
            for (int i = 0; i < 3; i++) mNormal[i] = (double)normal[i] / l;
            printf("normal: %f %f %f\n", mNormal[0], mNormal[1], mNormal[2]);
        }
        else if(strcmp(param_name, "fovy") == 0)
        {
            double theta;
            view >> theta;
            mFovy = theta;
            printf("dfovy: %f\n", mFovy);
        }
        else if(strcmp(param_name, "dnear") == 0)
        {
            double d;
            view >> d;
            mDnear = d;
            printf("dnear: %f\n", mDnear);
        }
        else if(strcmp(param_name, "dfar") == 0)
        {
            double d;
            view >> d;
            mDfar = d;
            printf("dfar: %f\n", mDfar);
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
            printf("viewport: %d %d %d %d\n", mViewport[0], mViewport[1], mViewport[2], mViewport[3]);
        }
    }
    
    view.close();
    return 0;
}

void ViewLoader::updateUnitVat()
{
    Coord3<double> v(mVat[0] - mEye[0], mVat[1] - mEye[1], mVat[2] - mEye[2]);
    double len = v.vecLen();
    for (int i = 0; i < 3; i++)
        mUnitVat[i] = (double)(v[i] / len);

    printf("uptate unit: %f %f %f\n", mUnitVat[0], mUnitVat[1], mUnitVat[2]);
}

void ViewLoader::updateDistance()
{
    Coord3<double> v(mVat[0] - mEye[0], mVat[1] - mEye[1], mVat[2] - mEye[2]);
    mDistance = v.vecLen();
    cout << "update dis " << mDistance << endl;
    return;
}