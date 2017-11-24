#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>

//texture type
#define NO_TEXTURE     0
#define SINGLE_TEXTURE 1
#define MULTI_TEXTURE  2
#define CUBE_MAP       6

class Texture
{
public:
    Texture() {}
    Texture(int type, std::vector<int> imgIndex)
    {
        mType = type;
        mImgIndex.assign(imgIndex.begin(), imgIndex.end());
    }
    ~Texture() {}

    Texture& operator=(const Texture rhs)
    {
        mType = rhs.mType;
        mImgIndex.assign(rhs.mImgIndex.begin(), rhs.mImgIndex.end());
        return *this;
    }

    int mType;             //texture type(4 types)
    std::vector<int> mImgIndex; //the set of indexes to find the image filename
};

#endif
