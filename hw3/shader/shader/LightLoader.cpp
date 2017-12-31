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
        }
        else if (strcmp(param_name, "ambient") == 0)
        {
            float ambient[3];
            light >> ambient[0] >> ambient[1] >> ambient[2];
            for (int i = 0; i < 3; i++) mEnvAmbient[i] = ambient[i];
        }
    }
    
    cout << "Success" << endl;
    light.close();
    return 0;
}