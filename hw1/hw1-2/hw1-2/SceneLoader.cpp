#include "SceneLoader.h"
#include <iostream>

SceneLoader::SceneLoader() { mNumOfObjs = 0; }
SceneLoader::~SceneLoader() {}

int SceneLoader::loadScene(string scene_file)
{
    ifstream scene(scene_file, ios::in);
    if(scene.fail())
    {
        printf("Can not open the scene file '%s'\n", scene_file.c_str());
        return -1;
    }
    cout << "Load the scene info...";
    
    char param_name[20];
    while(!scene.eof())
    {
        memset(param_name, 0, 20);
        scene >> param_name;
        
        if(strcmp(param_name, "model") == 0)
        {
            char obj_name[20];
            float s[3], angle, r[3], t[3];
            memset(obj_name, 0, 20);
            
            scene >> obj_name;
            scene >> s[0] >> s[1] >> s[2];
            scene >> angle >> r[0] >> r[1] >> r[2];
            scene >> t[0] >> t[1] >> t[2];
            
            mScale.push_back(Coord3<float>(s));
            mRotate.push_back(Rotate(angle, Coord3<float>(r)));
            mTransfer.push_back(Coord3<float>(t));
            printf("scale: %F %f %f\n", mScale.back()[0], mScale.back()[1], mScale.back()[2]);
            mNumOfObjs++;
        }
    }
    
    cout << "Success" << endl;
    scene.close();
    return 0;
}