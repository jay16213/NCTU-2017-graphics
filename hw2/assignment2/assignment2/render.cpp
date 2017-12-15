#include "headers.h"
extern map<int, mesh> objs;
extern SceneLoader scene;

void drawMirror(Model *mirror, int depth)
{
    glPushMatrix();
    glTranslatef(mirror->mTransfer[X], mirror->mTransfer[Y], mirror->mTransfer[Z]);
    glRotatef(
        mirror->mAngle,
        mirror->mRotateAxisVec[X],
        mirror->mRotateAxisVec[Y],
        mirror->mRotateAxisVec[Z]
    );
    glScalef(mirror->mScale[X], mirror->mScale[Y], mirror->mScale[Z]);

    renderObj(&objs[mirror->mObjIndex], depth);
    glPopMatrix();

    return;
}

void drawScene(int depth)
{
    for (int i = 0; i < scene.mComponents.size(); i++)
    {
        for (int j = 0; j < scene.mComponents[i].mNumOfModels; j++)
        {
            Model *m = &scene.mComponents[i].mModels[j];

            glPushMatrix();
            glTranslatef(m->mTransfer[X], m->mTransfer[Y], m->mTransfer[Z]);
            glRotatef(m->mAngle, m->mRotateAxisVec[X], m->mRotateAxisVec[Y], m->mRotateAxisVec[Z]);
            glScalef(m->mScale[X], m->mScale[Y], m->mScale[Z]);

            renderObj(&objs[m->mObjIndex], depth);
            glPopMatrix();
        }
    }
    return;
}

void renderObj(mesh *obj, int depth)
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
        for (size_t j = 0; j < 3; j++)
        {
            glNormal3fv(obj->nList[obj->faceList[i][j].n].ptr);
            glVertex3f(
                obj->vList[obj->faceList[i][j].v].ptr[X],
                obj->vList[obj->faceList[i][j].v].ptr[Y],
                obj->vList[obj->faceList[i][j].v].ptr[Z]
            );
        }
        glEnd();
    }
    return;
}