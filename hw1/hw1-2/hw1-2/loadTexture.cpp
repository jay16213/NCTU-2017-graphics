#include "headers.h"

extern Srcpath files;
extern string srcRootPath;

void loadTexture(Texture *tex, unsigned int *texObj, int *texObjIndex)
{
    switch (tex->mType)
    {
        case NO_TEXTURE:
            break;
        case SINGLE_TEXTURE:
            loadSingleTexture(tex, texObj, texObjIndex);
            break;
        case MULTI_TEXTURE:
            loadMultiTexture(tex, texObj, texObjIndex);
            break;
        case CUBE_MAP:
            loadCubeMapTexture(tex, texObj, texObjIndex);
            break;
    }
    return;
}

void loadSingleTexture(Texture *tex, unsigned int *texObj, int *texObjIndex)
{
    string imgFilename = srcRootPath + files.tNames[tex->mImgIndex[0]].c_str();
    FIBITMAP *pImg = FreeImage_Load(FreeImage_GetFileType(imgFilename.c_str(), 0), imgFilename.c_str());
    FIBITMAP *p32BitsImg = FreeImage_ConvertTo32Bits(pImg);
    int width = FreeImage_GetWidth(p32BitsImg);
    int height = FreeImage_GetHeight(p32BitsImg);

    glBindTexture(GL_TEXTURE_2D, texObj[(*texObjIndex)++]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
        0, GL_BGRA, GL_UNSIGNED_BYTE, (void *)FreeImage_GetBits(p32BitsImg));

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    FreeImage_Unload(p32BitsImg);
    FreeImage_Unload(pImg);
    return;
}

void loadMultiTexture(Texture *tex, unsigned int *texObj, int *texObjIndex)
{
    for (int i = 0; i < tex->mNumOfTextures; i++)
    {
        string imgFilename = srcRootPath + files.tNames[tex->mImgIndex[i]].c_str();
        FIBITMAP *pImg = FreeImage_Load(FreeImage_GetFileType(imgFilename.c_str(), 0), imgFilename.c_str());
        FIBITMAP *p32BitsImg = FreeImage_ConvertTo32Bits(pImg);
        int width = FreeImage_GetWidth(p32BitsImg);
        int height = FreeImage_GetHeight(p32BitsImg);

        glBindTexture(GL_TEXTURE_2D, texObj[(*texObjIndex)++]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
            0, GL_BGRA, GL_UNSIGNED_BYTE, (void *)FreeImage_GetBits(p32BitsImg));

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        FreeImage_Unload(p32BitsImg);
        FreeImage_Unload(pImg);
    }
    
    return;
}

void loadCubeMapTexture(Texture *tex, unsigned int *texObj, int *texObjIndex)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, texObj[(*texObjIndex)]);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FIBITMAP *pImg, *p32BitsImg;
    int width, height;
    for (int i = 0; i < 6; i++)
    {
        string imgFilename = srcRootPath + files.tNames[tex->mImgIndex[i]];
        pImg = FreeImage_Load(FreeImage_GetFileType(imgFilename.c_str(), 0), imgFilename.c_str());
        p32BitsImg = FreeImage_ConvertTo32Bits(pImg);
        width = FreeImage_GetWidth(p32BitsImg);
        height = FreeImage_GetHeight(p32BitsImg);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height,
            0, GL_BGRA, GL_UNSIGNED_BYTE, (void *)FreeImage_GetBits(p32BitsImg));

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 3);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        FreeImage_Unload(p32BitsImg);
        FreeImage_Unload(pImg);
    }
    return;
}