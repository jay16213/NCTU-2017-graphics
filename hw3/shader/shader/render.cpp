#include "headers.h"

void renderObj(mesh *obj, unsigned int *texObj, int texType, int texObjIndex)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    switch (texType)
    {
        case NO_TEXTURE:
            renderWithNoTex(obj);
            break;
        case SINGLE_TEXTURE:
            renderWithSingleTex(obj, texObj, texType, texObjIndex);
            break;
    }
}

void renderWithNoTex(mesh *obj)
{
    int lastMaterial = -1;
    for (size_t i = 0; i < obj->fTotal; i++)
    {
        // set material property if this face used different material
        if (lastMaterial != obj->faceList[i].m)
        {
            lastMaterial = (int)obj->faceList[i].m;
            glMaterialfv(GL_FRONT, GL_AMBIENT, obj->mList[lastMaterial].Ka);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, obj->mList[lastMaterial].Kd);
            glMaterialfv(GL_FRONT, GL_SPECULAR, obj->mList[lastMaterial].Ks);
            glMaterialfv(GL_FRONT, GL_SHININESS, &obj->mList[lastMaterial].Ns);
        }

        glBegin(GL_TRIANGLES);
        for (size_t j = 0; j<3; j++)
        {
            glNormal3fv(obj->nList[obj->faceList[i][j].n].ptr);
            glVertex3fv(obj->vList[obj->faceList[i][j].v].ptr);
        }
        glEnd();
    }
}

void renderWithSingleTex(mesh *obj, unsigned int *texObj, int texType, int texObjIndex)
{
    int lastMaterial = -1;
    for (size_t i = 0; i < obj->fTotal; i++)
    {
        // set material property if this face used different material
        if (lastMaterial != obj->faceList[i].m)
        {
            lastMaterial = (int)obj->faceList[i].m;
            glMaterialfv(GL_FRONT, GL_AMBIENT, obj->mList[lastMaterial].Ka);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, obj->mList[lastMaterial].Kd);
            glMaterialfv(GL_FRONT, GL_SPECULAR, obj->mList[lastMaterial].Ks);
            glMaterialfv(GL_FRONT, GL_SHININESS, &obj->mList[lastMaterial].Ns);
        }

        glBegin(GL_TRIANGLES);
        for (size_t j = 0; j<3; j++)
        {
            glMultiTexCoord2fv(GL_TEXTURE0, obj->tList[obj->faceList[i][j].t].ptr);
            glNormal3fv(obj->nList[obj->faceList[i][j].n].ptr);
            glVertex3fv(obj->vList[obj->faceList[i][j].v].ptr);
        }
        glEnd();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}
