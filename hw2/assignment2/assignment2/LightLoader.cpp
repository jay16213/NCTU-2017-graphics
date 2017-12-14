#include "LightLoader.h"
#include <iostream>

LightLoader::LightLoader() {}
LightLoader::~LightLoader() {}

int LightLoader::loadLight(string light_file)
{
    ifstream light(light_file, ios::in);
    if(light.fail())
    {
        printf("Can not open the light file '%s'!\n", light_file.c_str());
    }
    cout << "Load Light info...";
    
    char param_name[20];
    while(!light.eof())
    {
        memset(param_name, 0, 20);
        light >> param_name;
        
        if (strcmp(param_name, "light") == 0)
        {
            float p[3], a[3], d[3], s[3];
            light >> p[0] >> p[1] >> p[2];
            light >> a[0] >> a[1] >> a[2];
            light >> d[0] >> d[1] >> d[2];
            light >> s[0] >> s[1] >> s[2];

            mObjLight.push_back(LightInfo(p, a, d, s));
            printf("position: %f %f %f %f\n",
                    mObjLight.back().mPosition[0],
                    mObjLight.back().mPosition[1],
                    mObjLight.back().mPosition[2],
                    mObjLight.back().mPosition[3]
            );
            printf("a: %f %f %f\n",
                mObjLight.back().mAmbient[0],
                mObjLight.back().mAmbient[1],
                mObjLight.back().mAmbient[2],
                mObjLight.back().mAmbient[3]
            );
            printf("d: %f %f %f\n",
                mObjLight.back().mDiffuse[0],
                mObjLight.back().mDiffuse[1],
                mObjLight.back().mDiffuse[2],
                mObjLight.back().mDiffuse[3]
            );
            printf("s: %f %f %f\n",
                mObjLight.back().mSpecular[0],
                mObjLight.back().mSpecular[1],
                mObjLight.back().mSpecular[2],
                mObjLight.back().mSpecular[3]
           );
        }
        else if (strcmp(param_name, "ambient") == 0)
        {
            float ambient[3];
            light >> ambient[0] >> ambient[1] >> ambient[2];
            for (int i = 0; i < 3; i++) mEnvAmbient[i] = ambient[i];
            printf("env ambient: %f %f %f\n", mEnvAmbient[0], mEnvAmbient[1], mEnvAmbient[2]);
        }
    }
    
    cout << "Success" << endl;
    light.close();
    return 0;
}