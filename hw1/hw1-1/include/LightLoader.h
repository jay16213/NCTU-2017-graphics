#ifndef LIGHTLOADER_H
#define LIGHTLOADER_H

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include "Coord3.h"
using namespace std;

class LightInfo
{
public:
    LightInfo() {}
    LightInfo(float *position, float *ambient, float *diffuse, float *specular)
    {
        mPosition[3] = 1.0;
        mAmbient[3] = 1.0;
        mDiffuse[3] = 1.0;
        mSpecular[3] = 1.0;
        for (int i = 0; i < 3; i++)
        {
            mPosition[i] = position[i];
            mAmbient[i] = ambient[i];
            mDiffuse[i] = diffuse[i];
            mSpecular[i] = specular[i];
        }
    }
    ~LightInfo() {}

    float mPosition[4];
    float mAmbient[4];
    float mDiffuse[4];
    float mSpecular[4];
};

class LightLoader
{
public:
    LightLoader();
    ~LightLoader();
        
    int loadLight(string light_file);
        
    vector<LightInfo> mObjLight;
    float mEnvAmbient[3];
};

#endif