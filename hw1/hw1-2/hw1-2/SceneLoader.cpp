#include "SceneLoader.h"

SceneLoader::SceneLoader() { mNumOfObjs = 0; mNumOfTextures = 0; }
SceneLoader::~SceneLoader() { }

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
    int texType, start = 0;
    while(getline(scene, line))
    {
        stringstream ss(line);
        ss >> param_name;

        if (param_name == "no-texture")
        {
            texType = NO_TEXTURE;
        }
        else if (param_name == "single-texture")
        {
            texType = SINGLE_TEXTURE;

            string file;
            ss >> file;
            int id = getImgFileId(file);
            mTexObjs.push_back(Texture(SINGLE_TEXTURE, id));
            mNumOfTextures++;
        }
        else if (param_name == "multi-texture")
        {
            texType = MULTI_TEXTURE;

            string file;
            while (ss >> file)
            {
                int id = getImgFileId(file);
                mTexObjs.push_back(Texture(MULTI_TEXTURE, id));
                mNumOfTextures++;
            }
        }
        else if (param_name == "cube-map")
        {
            texType = CUBE_MAP;
            mNumOfTextures++;
            string file;
            while (ss >> file)
            {
                int id = getImgFileId(file);
                mTexObjs.push_back(Texture(CUBE_MAP, id));
            }
        }
        else if(param_name == "model")
        {
            string obj_name;
            float s[3], angle, r[3], t[3];
            
            ss >> obj_name;
            ss >> s[0] >> s[1] >> s[2];
            ss >> angle >> r[0] >> r[1] >> r[2];
            ss >> t[0] >> t[1] >> t[2];
            
            int id = getObjId(obj_name);
            vector<int> texObjIndex;
            for (int i = start; i < mNumOfTextures; i++) texObjIndex.push_back(i);

            Model model(
                id,
                texType,
                Rotate(angle, Coord3<float>(r)),
                Coord3<float>(s),
                Coord3<float>(t),
                texObjIndex
            );

            mModels.push_back(model);
            mNumOfObjs++;
            start = mNumOfTextures;
        }
    }
    
    scene.close();
    return 0;
}

int SceneLoader::getObjId(string obj)
{
    for (size_t i = 0; i < files.oNames.size(); i++)
    {
        if (obj == files.oNames[i]) return i;
    }

    cout << "get Obj id error" << endl;
    system("pause");
    exit(-1);
    return -1;
}

int SceneLoader::getImgFileId(string tex)
{
    for (size_t i = 0; i < files.tNames.size(); i++)
    {
        if (tex == files.tNames[i]) return i;
    }

    cout << "get Tex id error" << endl;
    system("pause");
    exit(-1);
    return -1;
}