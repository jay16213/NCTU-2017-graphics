#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include "glew.h"
#include "glut.h"
#include "FreeImage.h"
#include "Coord3.h"
#include "Srcpath.h"
using namespace std;

//texture type
#define NO_TEXTURE          0
#define SINGLE_TEXTURE      1
#define MULTI_TEXTURE       2
#define CUBE_MAP            3

//park
#define GEM                 0
#define BRUSH               1
#define TRUNK               2
#define WATER               3
#define HEDGE               4
#define LEAVES              5
#define SKYBOX              6
#define GROUNDV2            7
#define LITTLE_FOUNTAIN     8


//chess
#define ROOK                9
#define ROOM               10
#define KING               11
#define PAWN               12
#define QUEEN              13
#define KNIGHT             14
#define BISHOP             15
#define CHESSBOARD         16


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

    Rotate& operator=(const Rotate rhs)
    {
        mAngle = rhs.mAngle;
        mAxisVec = rhs.mAxisVec;
        return *this;
    }
    float mAngle;
    Coord3<float> mAxisVec;

};

class Model
{
public:
    Model() {}
    Model(
        int id,
        int type,
        Rotate r,
        Coord3<float> s,
        Coord3<float> t,
        vector<int> texObjIndex)
    {
        mId = id;
        mTextureType = type;
        mRotate = r;
        mScale = s;
        mTransfer = t;
        mTexObjIndex.assign(texObjIndex.begin(), texObjIndex.end());
    }
    ~Model() {}

    int mId;
    int mTextureType;
    vector<int> mTexObjIndex;
    Rotate mRotate;
    Coord3<float> mScale;
    Coord3<float> mTransfer;
};

class Texture{
public:
    Texture(int type, int imgIndex)
    {
        mType = type;
        //mNumOfFiles = files.size();
        mImgIndex = imgIndex;
    }
    ~Texture() {}

    int mType;
    //int mNumOfFiles;
    int mImgIndex;
};

class SceneLoader
{
public:
    SceneLoader();
    ~SceneLoader();

    int loadScene(string scene_file);
    void loadTexture();

    int mNumOfObjs;
    int mNumOfTextures;
    vector<Model> mModels;
    vector<Texture> mTexObjs;
private:
    int getObjId(string obj);
    int getImgFileId(string tex);
    Srcpath files;
};

#endif