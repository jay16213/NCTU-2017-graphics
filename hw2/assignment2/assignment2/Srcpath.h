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
        srcRootPath.assign("scene/");
        oNames.push_back(string("Cornell_box.obj"));
        oNames.push_back(string("dice.obj"));
        oNames.push_back(string("Mirror.obj"));
        oNames.push_back(string("MirrorWall.obj"));
        oNames.push_back(string("MirrorWall_Floor.obj"));
        oNames.push_back(string("MirrorWall_left_right.obj"));
        oNames.push_back(string("table.obj"));
    }
    ~Srcpath() {}

    vector<string> oNames;//obj file

    string srcRootPath;
};

#endif
