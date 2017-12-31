#include "headers.h"

extern Srcpath *files;
string srcRootPath = "gem/";

void loadTexture(Texture *tex, unsigned int *texObj, int *texObjIndex)
{
    switch (tex->mType)
    {
        case NO_TEXTURE:
            break;
        case SINGLE_TEXTURE:
            loadSingleTexture(tex, texObj, texObjIndex);
            break;
    }
    return;
}

void loadSingleTexture(Texture *tex, unsigned int *texObj, int *texObjIndex)
{
    string imgFilename = srcRootPath + files->tNames[tex->mImgIndex[0]].c_str();
    FIBITMAP *pImg = FreeImage_Load(FreeImage_GetFileType(imgFilename.c_str(), 0), imgFilename.c_str());
    FIBITMAP *p32BitsImg = FreeImage_ConvertTo32Bits(pImg);
    int width = FreeImage_GetWidth(p32BitsImg);
    int height = FreeImage_GetHeight(p32BitsImg);

    glBindTexture(GL_TEXTURE_2D, texObj[(*texObjIndex)]);
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
