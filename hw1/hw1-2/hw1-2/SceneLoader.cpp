#include "SceneLoader.h"

SceneLoader::SceneLoader() { mNumOfObjs = 0; }
SceneLoader::~SceneLoader() { mFiles.clear(); }

int SceneLoader::loadScene(string scene_file)
{
    ifstream scene(scene_file, ios::in);
    if(scene.fail())
    {
        printf("Can not open the scene file '%s'\n", scene_file.c_str());
        return -1;
    }
    cout << "Load the scene info..." << endl;
    
    
    string line, param_name;
    while(getline(scene, line))
    {
        stringstream ss(line);
        ss >> param_name;

        if (param_name == "no-texture")
        {
            mType = NO_TEXTURE;
        }
        else if (param_name == "single-texture")
        {
            mType = SINGLE_TEXTURE;
            mFiles.clear();
            string file;
            ss >> file;
            mFiles.push_back(file);
        }
        else if (param_name == "multi-texture")
        {
            mType = MULTI_TEXTURE;
            mFiles.clear();
            string file;
            while (ss >> file) mFiles.push_back(file);
        }
        else if (param_name == "cube-map")
        {
            mType = CUBE_MAP;
            mFiles.clear();
            string file;
            while (ss >> file) mFiles.push_back(file);
        }
        else if(param_name == "model")
        {
            string obj_name;
            float s[3], angle, r[3], t[3];
            
            ss >> obj_name;
            ss >> s[0] >> s[1] >> s[2];
            ss >> angle >> r[0] >> r[1] >> r[2];
            ss >> t[0] >> t[1] >> t[2];
            
            int id;
            if ((id = getModelId(obj_name)) == -1)
            {
                cout << "get model id error" << endl;
                system("pause");
                exit(-1);
            }

            Model model(
                id,
                mType,
                Rotate(angle, Coord3<float>(r)),
                Coord3<float>(s),
                Coord3<float>(t),
                mFiles
            );

            mObjects.push_back(model);
            mNumOfObjs++;
        }
    }
    
    scene.close();
    return 0;
}

int SceneLoader::getModelId(string obj)
{
    for (size_t i = 0; i < files.fNames.size(); i++)
    {
        if (obj == files.fNames[i]) return i;
    }

    return -1;
}