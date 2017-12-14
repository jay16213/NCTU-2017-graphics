#include "headers.h"

SceneLoader scene;
ViewLoader view;
LightLoader light;
map<int, mesh> objs;

double zoomDegree = 0.0, rotateDegree = M_PI / 150;

int numOfTextures;

Srcpath files;

int main(int argc, char **argv)
{   
    
    for (int i = 0; i < files.oNames.size(); i++)
        objs.insert(pair<int, mesh>(i, mesh(files.srcRootPath + files.oNames[i])));

    light.loadLight(files.srcRootPath + string("CornellBox.light"));
    scene.loadScene(files.srcRootPath + string("CornellBox.scene"));
    view.loadView(files.srcRootPath + string("CornellBox.view"));
    zoomDegree = 3.0;

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

void Display()
{
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
            GL_STENCIL_BUFFER_BIT | GL_ACCUM_BUFFER_BIT
    );

    //viewing and modeling transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        view.mEye[X], view.mEye[Y], view.mEye[Z],//eye
        view.mVat[X], view.mVat[Y], view.mVat[Z],//center
        view.mVup[X], view.mVup[Y], view.mVup[Z] //up
        );

    //compute lighting of objs
    lighting();

    //projection transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(view.mFovy, view.mAspect, view.mDnear, view.mDfar);

    //viewport transformation
    glViewport(view.mViewport[X], view.mViewport[Y], view.mViewport[2], view.mViewport[3]);

    //do obj transformation and rendering object
    glMatrixMode(GL_MODELVIEW);

    glColorMask(0xFF, 0xFF, 0xFF, 0xFF);
    drawMirror(&objs[2]);
    drawScene();
    glClear(GL_COLOR_BUFFER_BIT);


    objViewTransform();

    glFlush();
    return;
}

void lighting()
{
    glShadeModel(GL_SMOOTH);

    //enable lighting
    glEnable(GL_LIGHTING);

    //set light property
    for (size_t i = 0; i < light.mObjLight.size(); i++)
    {
        glEnable(GL_LIGHT0 + i);
        glLightfv(GL_LIGHT0 + i, GL_POSITION, light.mObjLight[i].mPosition);
        glLightfv(GL_LIGHT0 + i, GL_AMBIENT, light.mObjLight[i].mAmbient);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, light.mObjLight[i].mDiffuse);
        glLightfv(GL_LIGHT0 + i, GL_SPECULAR, light.mObjLight[i].mSpecular);
    }

    //set environment ambient
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light.mEnvAmbient);
    return;
}

void objViewTransform()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    //glEnable(GL_STENCIL_TEST);
    glCullFace(GL_BACK);

    for (int i = 0; i < 2; i++)
    {
        glStencilFunc(GL_EQUAL, i, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        /*if (i)
            glCullFace(GL_FRONT);
        else*/
        
        for (int j = 0; j < scene.mComponents.size(); j++)
        {
            Texture tex = scene.mComponents[j].mTex;
            for (int k = 0; k < scene.mComponents[j].mNumOfModels; k++)
            {
                Model model = scene.mComponents[j].mModels[k];
                //if (j == 1 && k == 2) continue;
                glPushMatrix();
                glTranslatef(
                    model.mTransfer[X] - (i ? 2* abs(model.mTransfer[X] + 40) : 0),
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

                renderObj(objs[model.mObjIndex], 0, tex.mType, 0);
                glPopMatrix();
            }
        }
    }
    return;
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
    double nx = view.mNormal[X], ny = view.mNormal[Y], nz = view.mNormal[Z];
    double cosT = cos(rotateDegree), sinT = sin(rotateDegree);
    double cost = cos(-rotateDegree), sint = sin(-rotateDegree);
    double eye_x = view.mEye[X], eye_z = view.mEye[Z];

    switch (key)
    {
    case 'w'://zoom in
        view.mEye[X] += view.mUnitVat[X] * zoomDegree;
        view.mEye[Y] += view.mUnitVat[Y] * zoomDegree;
        view.mEye[Z] += view.mUnitVat[Z] * zoomDegree;
        view.updateUnitVat();
        view.updateDistance();
        glutPostRedisplay();
        break;

    case 's'://zoom out
        view.mEye[X] -= view.mUnitVat[X] * zoomDegree;
        view.mEye[Y] -= view.mUnitVat[Y] * zoomDegree;
        view.mEye[Z] -= view.mUnitVat[Z] * zoomDegree;
        view.updateUnitVat();
        view.updateDistance();
        glutPostRedisplay();
        break;

    case 'a'://move left (circle the center)
        view.mEye[X] = cos(rotateDegree)*eye_x - sin(rotateDegree)*eye_z;
        view.mEye[Z] = sin(rotateDegree)*eye_x + cos(rotateDegree)*eye_z;

        view.updateUnitVat();
        printf("dis: %f\n", view.mDistance);
        printf("Vat: %f %f %f\n", view.mVat[X], view.mVat[Y], view.mVat[Z]);
        glutPostRedisplay();
        break;

    case 'd'://move right (circle the center)
        view.mEye[X] = cos(-rotateDegree)*eye_x - sin(-rotateDegree)*eye_z;
        view.mEye[Z] = sin(-rotateDegree)*eye_x + cos(-rotateDegree)*eye_z;

        view.updateUnitVat();
        printf("dis: %f\n", view.mDistance);
        printf("Vat: %f %f %f\n", view.mVat[X], view.mVat[Y], view.mVat[Z]);
        glutPostRedisplay();
        break;
    }

    return;
}
