#include "headers.h"

extern SceneLoader scene;
extern map<int, mesh> objs;

void renderObj(mesh obj, unsigned int *texObj, int texType, int texObjIndex)
{
    int lastMaterial = -1;
    for (size_t i = 0; i < obj.fTotal; i++)
    {
        // set material property if this face used different material
        if (lastMaterial != obj.faceList[i].m)
        {
            lastMaterial = (int)obj.faceList[i].m;
            glMaterialfv(GL_FRONT, GL_AMBIENT, obj.mList[lastMaterial].Ka);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, obj.mList[lastMaterial].Kd);
            glMaterialfv(GL_FRONT, GL_SPECULAR, obj.mList[lastMaterial].Ks);
            glMaterialfv(GL_FRONT, GL_SHININESS, &obj.mList[lastMaterial].Ns);
        }

        glBegin(GL_TRIANGLES);
        for (size_t j = 0; j < 3; j++)
        {
            glNormal3fv(obj.nList[obj.faceList[i][j].n].ptr);
            glVertex3fv(obj.vList[obj.faceList[i][j].v].ptr);
        }
        glEnd();
    }
}

void drawMirror(mesh *mirror)
{
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

    Model *m = &scene.mComponents[1].mModels[2];

    glPushMatrix();
    glTranslatef(
        m->mTransfer[X],
        m->mTransfer[Y],
        m->mTransfer[Z]
        );
    glRotatef(
        m->mAngle,
        m->mRotateAxisVec[X],
        m->mRotateAxisVec[Y],
        m->mRotateAxisVec[Z]
        );
    glScalef(
        m->mScale[X],
        m->mScale[Y],
        m->mScale[Z]
        );

    int lastMaterial = -1;
    for (size_t i = 0; i < mirror->fTotal; i++)
    {
        // set material property if this face used different material
        if (lastMaterial != mirror->faceList[i].m)
        {
            lastMaterial = (int)mirror->faceList[i].m;
            glMaterialfv(GL_FRONT, GL_AMBIENT, mirror->mList[lastMaterial].Ka);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mirror->mList[lastMaterial].Kd);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mirror->mList[lastMaterial].Ks);
            glMaterialfv(GL_FRONT, GL_SHININESS, &mirror->mList[lastMaterial].Ns);
        }

        glBegin(GL_TRIANGLES);
        for (size_t j = 0; j<3; j++)
        {
            glNormal3fv(mirror->nList[mirror->faceList[i][j].n].ptr);
            glVertex3fv(mirror->vList[mirror->faceList[i][j].v].ptr);
        }
        glEnd();
    }
    glPopMatrix();

}

void drawScene()
{
    Model *m = &scene.mComponents[1].mModels[2];
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    for (int i = 0; i < scene.mComponents.size(); i++)
    {
        Texture tex = scene.mComponents[i].mTex;

        for (int j = 0; j < scene.mComponents[i].mNumOfModels; j++)
        {
            Model model = scene.mComponents[i].mModels[j];

            glPushMatrix();
            glTranslatef(
                model.mTransfer[X],
                model.mTransfer[Y],
                model.mTransfer[Z]
                );
            glRotatef(
                model.mAngle,
                model.mRotateAxisVec[X],
                model.mRotateAxisVec[Y],
                model.mRotateAxisVec[Z]
                );
            glScalef(
                model.mScale[X],
                model.mScale[Y],
                model.mScale[Z]
                );

            mesh *obj = &objs[model.mObjIndex];

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
            glPopMatrix();
        }
    }
    glDisable(GL_CULL_FACE);
}