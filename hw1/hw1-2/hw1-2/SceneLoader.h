#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include "Coord3.h"
using namespace std;

class Rotate
{
public:
    Rotate() {}
    Rotate(float angle, Coord3<float> axisVec)
    {
        mAngle = angle;
        mAxisVec = axisVec;
    }
    ~Rotate() {}
    float mAngle;
    Coord3<float> mAxisVec;

};

class SceneLoader
{
public:
    SceneLoader();
    ~SceneLoader();

    int loadScene(string scene_file);

    int mNumOfObjs;
    vector<Rotate> mRotate;
    vector<Coord3<float>> mScale;
    vector<Coord3<float>> mTransfer;
};

#endif