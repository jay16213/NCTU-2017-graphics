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
    int type;
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

            string file;
            while (ss >> file)
            {
                int id = getImgFileId(file);
                mTexObjs.push_back(Texture(SINGLE_TEXTURE, id));
                mNumOfTextures++;
            }
        }
        else if (param_name == "multi-texture")
        {
            mType = MULTI_TEXTURE;

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
            mType = CUBE_MAP;

            string file;
            while (ss >> file)
            {
                int id = getImgFileId(file);
                mTexObjs.push_back(Texture(CUBE_MAP, id));
                mNumOfTextures++;
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

            Model model(
                id,
                mType,
                Rotate(angle, Coord3<float>(r)),
                Coord3<float>(s),
                Coord3<float>(t),
                
            );

            mObjects.push_back(model);
            mNumOfObjs++;
        }
    }
    
    scene.close();
    return 0;
}

void SceneLoader::loadTexture()
{
    mTexObjects = new unsigned int[mNumOfTextures];

    FreeImage_Initialise();
    
    //generate the texture objects
    glGenTextures(mNumOfTextures, mTexObjects);

    //load the img files and bind them to the texture objects
    for (int i = 0; i < mNumOfTextures; i++)
    {
        switch (mTexFiles[i].mType)
        {
            case SIG
        }
        FIBITMAP *pImg = FreeImage_Load(FreeImage_GetFileType(imgFilename, 0), imgFilename);
        FIBITMAP *p32BitsImg = FreeImage_ConvertTo32Bits(pImg);
        int width = FreeImage_GetWidth(p32BitsImg);
        int height = FreeImage_GetHeight(p32BitsImg);
        
        glBindTexture(GL_TEXTURE_2D, texObjs);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, (void *)FreeImage_GetBits(p32BitsImg));
        glGenerateMipmap(GL_TEXTURE_2D);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        FreeImage_Unload(p32BitsImg);
        FreeImage_Unload(pImg);
    }
    FreeImage_DeInitialise();
    return;
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