#ifndef SRCPATH_H
#define SRCPATH_H

#include <vector>
#include <string>
using namespace std;

class Srcpath
{
public:
    Srcpath()
    {
        //park
        srcRootPath_sub.assign("gem/");
        oNames.push_back(string("gem.obj"));
        tNames.push_back(string("ocean_v3_256.bmp"));
        srcRootPath_pho.assign("phong/");
        oNames.push_back(string("box.obj"));
        oNames.push_back(string("bunny.obj"));
        oNames.push_back(string("venus.obj"));
    }
    ~Srcpath() {}

    vector<string> oNames;//obj file
    vector<string> tNames;//tex file
    string srcRootPath_sub;
    string srcRootPath_pho;
};

#endif
