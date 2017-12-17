#include "headers.h"

SceneLoader scene;
ViewLoader view;
LightLoader light;
map<int, mesh> objs;

double zoomDegree = 0.0, rotateDegree = M_PI / 150;

Model *frontMirror, *backMirror;

int main(int argc, char **argv)
{   
    Srcpath *files = new Srcpath;

    for (int i = 0; i < files->oNames.size(); i++)
        objs.insert(pair<int, mesh>(i, mesh(files->srcRootPath + files->oNames[i])));

    light.loadLight(files->srcRootPath + string("CornellBox.light"));
    scene.loadScene(files->srcRootPath + string("CornellBox.scene"));
    view.loadView(files->srcRootPath + string("CornellBox.view"));
    delete files;

    zoomDegree = 3.0;

    frontMirror = &scene.mComponents[1].mModels[2];
    backMirror = &scene.mComponents[1].mModels[3];

    view.mVat.assign(-10, 12, 0);

    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutCreateWindow("Assignment 2");
    //glewInit();
    glutDisplayFunc(Display);
    glutReshapeFunc(ReShape);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
    return 0;
}

void lighting(int depth)
{
    //enable lighting
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    //set light property
    for (size_t i = 0; i < light.mObjLight.size(); i++)
    {
        double fd = fabs(light.mObjLight[i].mPosition[X] + 40);
        double bd = fabs(light.mObjLight[i].mPosition[X] - 40);
        double displacement = 0.0;
        int t = depth;
        while (t > 0)
        {
            displacement -= (t % 2) ? 2*fd : 2*bd;
            t--;
        }
        glEnable(GL_LIGHT0 + i);
        GLfloat p[4] = {
            light.mObjLight[i].mPosition[X] + displacement,
            light.mObjLight[i].mPosition[Y],
            light.mObjLight[i].mPosition[Z],
            light.mObjLight[i].mPosition[3]
        };
        glLightfv(GL_LIGHT0 + i, GL_POSITION, p);
        glLightfv(GL_LIGHT0 + i, GL_AMBIENT, light.mObjLight[i].mAmbient);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, light.mObjLight[i].mDiffuse);
        glLightfv(GL_LIGHT0 + i, GL_SPECULAR, light.mObjLight[i].mSpecular);
    }

    //set environment ambient
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light.mEnvAmbient);
    return;
}

void Display()
{
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0);

    int depth = 0;
    int numOfPass = 8;

    Coord3d jitter(0.0, 0.0, 0.0);
    double angle = 360 / numOfPass;
    double c = M_PI / 180;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);

    glEnable(GL_STENCIL_TEST);

    view.updateRight();
    for (int pass = 0; pass < numOfPass; pass++)
    {
        glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cout << "--------------------------------" << endl;
        cout << "pass " << pass << endl;

        for (int i = 0; i < 3; i++)
            jitter[i] = view.mRight[i] * cos((angle * pass) * c) + view.mVup[i] * sin((angle * pass) * c);
        
        double len = jitter.vecLen() * 5;//jitter distance to 0.2
        for (int i = 0; i < 3; i++) jitter[i] = jitter[i] / len;
        
        glEnable(GL_MODELVIEW);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            view.mEye[X] + jitter[X], view.mEye[Y] + jitter[Y], view.mEye[Z] + jitter[Z],
            view.mVat[X], view.mVat[Y], view.mVat[Z],
            view.mVup[X], view.mVup[Y], view.mVup[Z]
        );

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(view.mFovy, view.mAspect, view.mDnear, view.mDfar);
        glViewport(view.mViewport[X], view.mViewport[Y], view.mViewport[2], view.mViewport[3]);

        lighting(0);

        glMatrixMode(GL_MODELVIEW);

        glDepthMask(GL_FALSE);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        
        setStencil();

        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        for (int i = 0; i <= 4; i++)
        {
            glStencilFunc(GL_EQUAL, i, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

            if (i % 2)
                glCullFace(GL_FRONT);
            else
                glCullFace(GL_BACK);

            drawScene(FRONT, i);
        }

        for (int i = 1; i <= 4; i++)
        {
            glStencilFunc(GL_EQUAL, i, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            if (i % 2)
                glCullFace(GL_FRONT);
            else
                glCullFace(GL_BACK);

            drawScene(BACK, i);
        }
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        glAccum(pass ? GL_ACCUM : GL_LOAD, 1.0 / numOfPass);
    }
    glAccum(GL_RETURN, 1.0);

    glFlush();
    return;
}

void setStencil()
{
    //front
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

    drawMirror(frontMirror, FRONT, 0);

    for (int i = 0; i <= 4; i++)
    {
        glStencilFunc(GL_EQUAL, i + 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);

        drawScene(FRONT, i);

        glStencilFunc(GL_EQUAL, i + 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
        drawMirror(frontMirror, FRONT, i + 1);
    }

    //back
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

    drawMirror(backMirror, BACK, 0);

    for (int i = 0; i <= 4; i++)
    {
        glStencilFunc(GL_EQUAL, i + 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);

        drawScene(BACK, i);

        glStencilFunc(GL_EQUAL, i + 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
        drawMirror(backMirror, BACK, i + 1);
    }

    return;
}

void drawMirror(Model *mirror, int dir, int depth)
{
    double displacement = 0.0;
    switch (dir)
    {
        case FRONT:
            while (depth--) displacement -= 80.0;
            break;
        case BACK:
            while (depth--) displacement += 80.0;
            break;
    }

    glPushMatrix();
    glTranslatef(
        mirror->mTransfer[X] + displacement,
        mirror->mTransfer[Y],
        mirror->mTransfer[Z]
    );
    glRotatef(mirror->mAngle, mirror->mRotateAxisVec[X], mirror->mRotateAxisVec[Y], mirror->mRotateAxisVec[Z]);
    glScalef(mirror->mScale[X], mirror->mScale[Y], mirror->mScale[Z]);

    renderObj(&objs[mirror->mObjIndex], depth);
    glPopMatrix();
}

void drawScene(int dir, int depth)
{
    for (int i = 0; i < scene.mComponents.size(); i++)
    {
        for (int j = 0; j < scene.mComponents[i].mNumOfModels; j++)
        {
            if (i == 1 && j >= 2) continue;
            Model *m = &scene.mComponents[i].mModels[j];

            double fd = 2 * fabs(m->mTransfer[X] + 40.0);
            double bd = 2 * fabs(m->mTransfer[X] - 40.0);
            double displacement = 0.0;

            int t = depth;
            switch (dir)
            {
                case FRONT:
                    while (t > 0)
                    {
                        displacement -= (t % 2) ? fd : bd;
                        t--;
                    }
                    break;
                case BACK:
                    while (t > 0)
                    {
                        displacement += (t % 2) ? bd : fd;
                        t--;
                    }
                    break;
            }

            glPushMatrix();
            glTranslatef(
                m->mTransfer[X] + displacement,
                m->mTransfer[Y],
                m->mTransfer[Z]
            );
            
            glScalef(pow(-1.0, depth), 1.0, 1.0);
            glRotatef(m->mAngle, m->mRotateAxisVec[X], m->mRotateAxisVec[Y], m->mRotateAxisVec[Z]);
            glScalef(m->mScale[X], m->mScale[Y], m->mScale[Z]);

            renderObj(&objs[m->mObjIndex], depth);
            glPopMatrix();
        }
    }
}

void ReShape(int w, int h)
{
    glViewport(view.mViewport[X], view.mViewport[Y], (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(view.mFovy, view.mAspect, view.mDnear, view.mDfar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        view.mEye[X], view.mEye[Y], view.mEye[Z],//eye
        view.mVat[X], view.mVat[Y], view.mVat[Z],//center
        view.mVup[X], view.mVup[Y], view.mVup[Z] //up
    );
    return;
}

void Keyboard(unsigned char key, int x, int y)
{
    double eye_x = view.mEye[X];
    double eye_z = view.mEye[Z];

    switch (key)
    {
        case 'w'://zoom in
            view.mEye[X] += view.mUnitVat[X] * zoomDegree;
            view.mEye[Y] += view.mUnitVat[Y] * zoomDegree;
            view.mEye[Z] += view.mUnitVat[Z] * zoomDegree;
            view.updateUnitVat();
            view.updateDistance();
            break;

        case 's'://zoom out
            view.mEye[X] -= view.mUnitVat[X] * zoomDegree;
            view.mEye[Y] -= view.mUnitVat[Y] * zoomDegree;
            view.mEye[Z] -= view.mUnitVat[Z] * zoomDegree;
            view.updateUnitVat();
            view.updateDistance();
            break;

        case 'a'://move left (circle the center)
            view.mEye[X] = cos(rotateDegree)*eye_x - sin(rotateDegree)*eye_z;
            view.mEye[Z] = sin(rotateDegree)*eye_x + cos(rotateDegree)*eye_z;

            cout << view.mEye[X] << " " << view.mEye[Y] << " " << view.mEye[Z] << endl;
            view.updateUnitVat();
            break;

        case 'd'://move right (circle the center)
            view.mEye[X] = cos(-rotateDegree)*eye_x - sin(-rotateDegree)*eye_z;
            view.mEye[Z] = sin(-rotateDegree)*eye_x + cos(-rotateDegree)*eye_z;

            cout << view.mEye[X] << " " << view.mEye[Y] << " " << view.mEye[Z] << endl;
            view.updateUnitVat();
            break;

        case '1':
            view.mVat.assign(-10.0, 12.0, 0.0);
            view.updateRight();
            break;
        
        case '2':
            view.mVat.assign(-50.0, 12.0, 0.0);
            view.updateRight();
            break;

        case '3':
            view.mVat.assign(-400.0, 12.0, 0.0);
            view.updateRight();
            break;
        
        case '4':
            view.mEye.assign(200, 80, 0);
            break;
    }

    glutPostRedisplay();
    return;
}
