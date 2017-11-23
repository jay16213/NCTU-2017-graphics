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
        fNames.push_back(string("gem.obj"));
        fNames.push_back(string("bush.obj"));
        fNames.push_back(string("trunk.obj"));
        fNames.push_back(string("water.obj"));
        fNames.push_back(string("hedge.obj"));
        fNames.push_back(string("leaves.obj"));
        fNames.push_back(string("skybox.obj"));
        fNames.push_back(string("groundv2.obj"));
        fNames.push_back(string("littlefountain.obj"));

        //gem.assign("gem.obj");
        //bush.assign("bush.obj");
        //trunk.assign("trunk.obj");
        //water.assign("water.obj");
        //hedge.assign("hedge.obj");
        //leaves.assign("leaves.obj");
        //skybox.assign("skybox.obj");
        //groundv2.assign("groundv2.obj");
        //littleFountain.assign("littleFountain.obj");
        
        //chessboard
        srcRootPath_chess.assign("Chess/");
        fNames.push_back(string("Rook.obj"));
        fNames.push_back(string("Room.obj"));
        fNames.push_back(string("King.obj"));
        fNames.push_back(string("Pawn.obj"));
        fNames.push_back(string("Queen.obj"));
        fNames.push_back(string("Knight.obj"));
        fNames.push_back(string("Bishop.obj"));
        fNames.push_back(string("Chessboard.obj"));
        //rook.assign("Rook.obj");
        //room.assign("Room.obj");
        //king.assign("King.obj");
        //pawn.assign("Pawn.obj");
        //queen.assign("Queen.obj");
        //knight.assign("Knight.obj");
        //bishop.assign("Bishop.obj");
        //chessboard.assign("Chessboard.obj");

    }
    ~Srcpath() {}

    vector<string> fNames;
    //park
    string srcRootPath_park;
    //string bush, gem, groundv2, hedge, leaves, littleFountain, skybox, trunk, water;
    //chessboard
    string srcRootPath_chess;
    //string bishop, chessboard, king, knight, pawn, queen, rook, room;
};

#endif
