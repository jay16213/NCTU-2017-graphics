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
#include "Texture.h"
#include "Srcpath.h"
#include "Coord3.h"
#include "Model.h"
using namespace std;

//a component is composed of one texture and multi model
//where the texture has 4 types(no, single, multi, cube map)
class Component
{
public:
    Component() {}
    Component(Texture tex, vector<Model> models)
    {
        mTex = tex;
        mModels.assign(models.begin(), models.end());
    }
    ~Component() {}

    Texture mTex;          // one texture
    vector<Model> mModels;  // multi model
};

// the whole is composed of many component
class SceneLoader
{
public:
    SceneLoader();
    ~SceneLoader();

    int loadScene(string scene_file);
    vector<Component> mComponents;

private:
    int getObjId(string obj);
    int getImgFileId(string tex);
    Srcpath files;
};

#endif