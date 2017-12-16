#include "headers.h"

SceneLoader scene;
ViewLoader view;
LightLoader light;
map<int, mesh> objs;

double zoomDegree = 0.0, rotateDegree = M_PI / 150;

int focus;
Srcpath files;
Model *frontMirror, *backMirror;

int main(int argc, char **argv)
{   
    for (int i = 0; i < files.oNames.size(); i++)
        objs.insert(pair<int, mesh>(i, mesh(files.srcRootPath + files.oNames[i])));

    light.loadLight(files.srcRootPath + string("CornellBox.light"));
    scene.loadScene(files.srcRootPath + string("CornellBox.scene"));
    view.loadView(files.srcRootPath + string("CornellBox.view"));
    zoomDegree = 3.0;

    frontMirror = &scene.mComponents[1].mModels[2];
    backMirror = &scene.mComponents[1].mModels[3];
    focus = 1;
    view.mVat[X] = -10.0;
    view.mVat[Y] = 12.0;
    view.mVat[Z] = 0.0;

    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutCreateWindow("Assignment 2");
    glewInit();
    glutDisplayFunc(Display);
    glutReshapeFunc(ReShape);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
    return 0;
}

void lighting(int depth)
{
    glShadeModel(GL_SMOOTH);

    //enable lighting
    glEnable(GL_LIGHTING);

    double displacement = 0.0;
    while (depth > 0)
    {
        displacement -= 80;
        depth--;
    }

    //set light property
    for (size_t i = 0; i < light.mObjLight.size(); i++)
    {
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

    //lighting(0);
    view.updateRight();
    for (int pass = 0; pass < numOfPass; pass++)
    {
        glEnable(GL_STENCIL_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);

        cout << "--------------------------------" << endl;
        cout << "d:" << angle * pass << " r:" << (angle * pass) * c << endl;
        if (pass)
        {
            jitter[X] = view.mRight[X] * cos((angle * pass) * c) + view.mVup[X] * sin((angle * pass) * c);
            jitter[Y] = view.mRight[Y] * cos((angle * pass) * c) + view.mVup[Y] * sin((angle * pass) * c);
            jitter[Z] = view.mRight[Z] * cos((angle * pass) * c) + view.mVup[Z] * sin((angle * pass) * c);
        }

        double len = jitter.vecLen() * 5;//jitter distance to 0.2
        for (int i = 0; i < 3; i++) jitter[i] = jitter[i] / len;

        double eye_x = view.mEye[X] + jitter[X];
        double eye_y = view.mEye[Y] + jitter[Y];
        double eye_z = view.mEye[Z] + jitter[Z];

        cout << "right: " << view.mRight[0] << " " << view.mRight[1] << " " << view.mRight[2] << endl;
        cout << "jitter: " << jitter[X] << " " << jitter[Y] << " " << jitter[Z] << endl;
        cout << eye_x << " " << eye_y << " " << eye_z << endl;

        glEnable(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            eye_x, eye_y, eye_z,
            view.mVat[X], view.mVat[Y], view.mVat[Z],
            view.mVup[X], view.mVup[Y], view.mVup[Z]
        );

        //lighting(0);


        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(view.mFovy, view.mAspect, view.mDnear, view.mDfar);

        glViewport(view.mViewport[X], view.mViewport[Y], view.mViewport[2], view.mViewport[3]);

        glMatrixMode(GL_MODELVIEW);

        //glClear(GL_STENCIL_BUFFER_BIT);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

        drawMirror(frontMirror, 0);

        for (int i = 0; i < 4; i++)
        {
            glStencilFunc(GL_EQUAL, i + 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
            drawScene(i);

            if (i + 1 < 4)
            {
                glStencilFunc(GL_EQUAL, i + 1, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
                drawMirror(frontMirror, i + 1);
            }
        }
        glDepthMask(GL_TRUE);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        for (int i = 0; i < 4; i++)
        {
            lighting(i);

            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);

            glStencilFunc(GL_EQUAL, i, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

            glEnable(GL_CULL_FACE);
            if (i % 2)
            {
                glCullFace(GL_FRONT);
                glFrontFace(GL_CW);
            }
            else
            {
                glCullFace(GL_BACK);
                glFrontFace(GL_CCW);

            }
            drawScene(i);
        }
        glAccum(pass ? GL_ACCUM : GL_LOAD, 1.0 / numOfPass);
    }

    glAccum(GL_RETURN, 1.0);
    glFlush();
    return;
}

void drawMirror(Model *mirror, int depth)
{
    double displacement = 0.0;
    while (depth)
    {
        displacement -= 80;
        depth--;
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

void drawScene(int depth)
{
    for (int i = 0; i < scene.mComponents.size(); i++)
    {
        for (int j = 0; j < scene.mComponents[i].mNumOfModels; j++)
        {
            if (i == 1 && j >= 2) continue;
            Model *m = &scene.mComponents[i].mModels[j];

            double fd = fabs(m->mTransfer[X] + 40);
            double bd = fabs(m->mTransfer[X] - 40);
            double displacement = 0.0;

            int t = depth;
            while (t > 0)
            {
                displacement -= (t % 2) ? 2 * fd : 2 * bd;
                t--;
            }

            glPushMatrix();
            glTranslatef(m->mTransfer[X] + displacement, m->mTransfer[Y], m->mTransfer[Z]);
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
    double vat_x = view.mVat[X] - view.mEye[X];
    double vat_y = view.mVat[Y] - view.mEye[Y];
    double vat_z = view.mVat[Z] - view.mEye[Z];
    double eye_x = view.mEye[X], eye_z = view.mEye[Z];

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

            view.updateUnitVat();
            printf("dis: %f\n", view.mDistance);
            printf("Vat: %f %f %f\n", view.mVat[X], view.mVat[Y], view.mVat[Z]);
            break;

        case 'd'://move right (circle the center)
            view.mEye[X] = cos(-rotateDegree)*eye_x - sin(-rotateDegree)*eye_z;
            view.mEye[Z] = sin(-rotateDegree)*eye_x + cos(-rotateDegree)*eye_z;

            view.updateUnitVat();
            printf("dis: %f\n", view.mDistance);
            printf("Vat: %f %f %f\n", view.mVat[X], view.mVat[Y], view.mVat[Z]);
            break;

        case '1':
            focus = 1;
            cout << "focus on (-10, 12, 0)" << endl;
            view.mVat[X] = -10.0;
            view.mVat[Y] = 12.0;
            view.mVat[Z] = 0.0;
            break;
        
        case '2':
            focus = 2;
            cout << "focus on (-50, 12, 0)" << endl;
            view.mVat[X] = -50.0;
            view.mVat[Y] = 12.0;
            view.mVat[Z] = 0.0;
            break;

        case '3':
            focus = 3;
            cout << "focus on (-400, 12, 0)" << endl;
            view.mVat[X] = -400.0;
            view.mVat[Y] = 12.0;
            view.mVat[Z] = 0.0;
            break;
    }

    glutPostRedisplay();
    return;
}
