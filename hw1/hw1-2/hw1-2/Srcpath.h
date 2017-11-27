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
        srcRootPath_park.assign("Park/");
        oNames.push_back(string("gem.obj"));
        oNames.push_back(string("bush.obj"));
        oNames.push_back(string("trunk.obj"));
        oNames.push_back(string("water.obj"));
        oNames.push_back(string("hedge.obj"));
        oNames.push_back(string("leaves.obj"));
        oNames.push_back(string("skybox.obj"));
        oNames.push_back(string("groundv2.obj"));
        oNames.push_back(string("LittleFountain.obj"));
        
        tNames.push_back(string("bush.tga"));
        tNames.push_back(string("EnvXp.bmp"));
        tNames.push_back(string("EnvXn.bmp"));
        tNames.push_back(string("EnvYp.bmp"));
        tNames.push_back(string("EnvYn.bmp"));
        tNames.push_back(string("EnvZp.bmp"));
        tNames.push_back(string("EnvZn.bmp"));
        tNames.push_back(string("hedge.bmp"));
        tNames.push_back(string("grass.bmp"));
        tNames.push_back(string("trunk.jpg"));
        tNames.push_back(string("water.png"));
        tNames.push_back(string("leaves.png"));
        tNames.push_back(string("GroundLightMap.bmp"));
        tNames.push_back(string("skybox2048x2048.bmp"));
        tNames.push_back(string("Fountain_diffuse.bmp"));
        
        //chessboard
        srcRootPath_chess.assign("Chess/");
        oNames.push_back(string("Rook.obj"));
        oNames.push_back(string("Room.obj"));
        oNames.push_back(string("King.obj"));
        oNames.push_back(string("Pawn.obj"));
        oNames.push_back(string("Queen.obj"));
        oNames.push_back(string("Knight.obj"));
        oNames.push_back(string("Bishop.obj"));
        oNames.push_back(string("Chessboard.obj"));
        
        tNames.push_back(string("Grid.bmp"));
        tNames.push_back(string("Room.bmp"));
        tNames.push_back(string("Wood.bmp"));
        tNames.push_back(string("Env_positive_x.bmp"));
        tNames.push_back(string("Env_negative_x.bmp"));
        tNames.push_back(string("Env_positive_y.bmp"));
        tNames.push_back(string("Env_negative_y.bmp"));
        tNames.push_back(string("Env_positive_z.bmp"));
        tNames.push_back(string("Env_negative_z.bmp"));
    }
    ~Srcpath() {}

    vector<string> oNames;//obj file
    vector<string> tNames;//texture img
    //park
    string srcRootPath_park;
    //chessboard
    string srcRootPath_chess;
};

#endif
