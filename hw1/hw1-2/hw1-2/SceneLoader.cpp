#include "SceneLoader.h"

SceneLoader::SceneLoader() { mNumOfTextures = 0; }
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
    
    
    string line, param;
    int insert = 0;
    Texture tex;
    vector<Model> models;
    while(getline(scene, line))
    {
        stringstream ss(line);
        ss >> param;

        if (param == "no-texture")
        {
            if (insert)
            {
                mComponents.push_back(Component(tex, models));
                models.clear();
                insert = 0;
            }

            tex = Texture(NO_TEXTURE, vector<int>());
        }
        else if (param == "single-texture")
        {
            if (insert)
            {
                mComponents.push_back(Component(tex, models));
                models.clear();
                insert = 0;
            }

            ss >> param;
            int imgIndex = getImgFileId(param);
            tex = Texture(SINGLE_TEXTURE, vector<int>(1, imgIndex));
            mNumOfTextures++;
            printf("Texture: %d %s\n", tex.mType, files.tNames[tex.mImgIndex[0]].c_str());
        }
        else if (param == "multi-texture")
        {
            if (insert)
            {
                mComponents.push_back(Component(tex, models));
                models.clear();
                insert = 0;
            }

            vector<int> imgIndex;
            while (ss >> param)
            {
                imgIndex.push_back(getImgFileId(param));
                mNumOfTextures++;
            }
            tex = Texture(MULTI_TEXTURE, imgIndex);

            printf("Texture: %d ", tex.mType);
            for (int i = 0; i < tex.mImgIndex.size(); i++)
            {
                printf("%s ", files.tNames[tex.mImgIndex[i]].c_str());
            }
            printf("\n");
        }
        else if (param == "cube-map")
        {
            if (insert)
            {
                mComponents.push_back(Component(tex, models));
                models.clear();
                insert = 0;
            }

            vector<int> imgIndex;
            while (ss >> param)
                imgIndex.push_back(getImgFileId(param));

            tex = Texture(CUBE_MAP, imgIndex);
            mNumOfTextures++;

            printf("Texture: %d ", tex.mType);
            for (int i = 0; i < tex.mImgIndex.size(); i++)
            {
                printf("%s ", files.tNames[tex.mImgIndex[i]].c_str());
            }
            printf("\n");
        }
        else if (param == "model")
        {
            insert = 1;
            string obj_name;
            float angle, rAv[3], s[3], t[3];

            ss >> obj_name;
            ss >> s[0] >> s[1] >> s[2];
            ss >> angle >> rAv[0] >> rAv[1] >> rAv[2];
            ss >> t[0] >> t[1] >> t[2];

            int objIndex = getObjId(obj_name);
            models.push_back(Model(objIndex, angle, Coord3f(rAv), Coord3f(s), Coord3f(t)));
        }
    }
    mComponents.push_back(Component(tex, models));
    
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