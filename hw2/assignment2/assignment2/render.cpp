#include "headers.h"

extern map<int, mesh> objs;
extern SceneLoader scene;

void renderObj(mesh *obj, int depth)
{
    int lastMaterial = -1;
    for (size_t i = 0; i < obj->fTotal; i++)
    {
        if (lastMaterial != obj->faceList[i].m)
        {
            lastMaterial = (int)obj->faceList[i].m;
            glMaterialfv(GL_FRONT, GL_AMBIENT, obj->mList[lastMaterial].Ka);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, obj->mList[lastMaterial].Kd);
            glMaterialfv(GL_FRONT, GL_SPECULAR, obj->mList[lastMaterial].Ks);
            glMaterialfv(GL_FRONT, GL_SHININESS, &obj->mList[lastMaterial].Ns);
        }

        glBegin(GL_TRIANGLES);
        for (size_t j = 0; j < 3; j++)
        {
            glNormal3fv(obj->nList[obj->faceList[i][j].n].ptr);
            glVertex3fv(obj->vList[obj->faceList[i][j].v].ptr);
        }
        glEnd();
    }
    return;
}