#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include "glut.h"
#include "mesh.h"
#include "ViewLoader.h"
#include "LightLoader.h"
#include "SceneLoader.h"
using namespace std;

#define X 0
#define Y 1
#define Z 2

string srcRootPath("TestScene/");
string box("box.obj"), venus("venus.obj"), bunny("bunny.obj");
string redbox("redbox.obj"), yellowbox("yellowbox.obj"), bluebox("bluebox.obj");

SceneLoader scene;
ViewLoader view;
LightLoader light;
vector<mesh> objs;
double zoomDegree = 0.0, dragDegree = 0.0;
int selectedObj = -1;


void Display();
void ReShape(int w, int h);
void renderObj(mesh obj);
void objViewTransform();
void lighting();
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);

int main(int argc, char **argv)
{
    int res = 0, window_size = 0;
    printf("Please input the scene you want to test(1: TestScene1; 2: TestScene2): ");
    cin >> res;
    switch (res)
    {
        case 1:
            objs.push_back(mesh(srcRootPath + box));
            objs.push_back(mesh(srcRootPath + venus));
            objs.push_back(mesh(srcRootPath + bunny));
            scene.loadScene(srcRootPath + string("scene.scene"));
            light.loadLight(srcRootPath + string("light.light"));
            view.loadView(srcRootPath + string("view.view"));
            zoomDegree = 20.0;
            dragDegree = 1.0;
            window_size = 800;
            break;
        case 2:
            objs.push_back(mesh(srcRootPath + redbox));
            objs.push_back(mesh(srcRootPath + yellowbox));
            objs.push_back(mesh(srcRootPath + bluebox));
            objs.push_back(objs[0]);
            objs.push_back(objs[1]);
            objs.push_back(objs[2]);
            scene.loadScene(srcRootPath + string("scene2.scene"));
            light.loadLight(srcRootPath + string("scene2.light"));
            view.loadView(srcRootPath + string("scene2.view"));
            zoomDegree = 0.1;
            dragDegree = 50.0;
            window_size = 512;
            break;
        default:
            cout << "Error input" << endl;
            exit(0);
            break;
    }

    glutInit(&argc, argv);
    glutInitWindowSize(window_size, window_size);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("HW1-1");
    glutDisplayFunc(Display);
    glutReshapeFunc(ReShape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMainLoop();
    return 0;
}

void Display()
{
    //compute lighting of objs
    lighting();

    //viewing and modeling transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        view.mEye[X], view.mEye[Y], view.mEye[Z],//eye
        view.mVat[X], view.mVat[Y], view.mVat[Z],//center
        view.mVup[X], view.mVup[Y], view.mVup[Z] //up
    );

    //projection transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(view.mFovy, view.mAspect, view.mDnear, view.mDfar);

    //viewport transformation
    glViewport(view.mViewport[X], view.mViewport[Y], view.mViewport[2], view.mViewport[3]);

    //do obj transformation and rendering object
    glMatrixMode(GL_MODELVIEW);
    objViewTransform();

    glutSwapBuffers();
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

void renderObj(mesh obj)
{
    int lastMaterial = -1;
    for (size_t i = 0; i < obj.fTotal; ++i)
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
        for (size_t j = 0; j<3; ++j)
        {
            //textex corrd. object->tList[object->faceList[i][j].t].ptr
            glNormal3fv(obj.nList[obj.faceList[i][j].n].ptr);
            glVertex3fv(obj.vList[obj.faceList[i][j].v].ptr);
        }
        glEnd();
    }
}

void objViewTransform()
{
    for (int i = 0; i < scene.mNumOfObjs; i++)
    {
        glPushMatrix();
        glTranslatef(
            scene.mTransfer[i][X],
            scene.mTransfer[i][Y],
            scene.mTransfer[i][Z]
            );
        glRotatef(
            scene.mRotate[i].mAngle,
            scene.mRotate[i].mAxisVec[X],
            scene.mRotate[i].mAxisVec[Y],
            scene.mRotate[i].mAxisVec[Z]
            );
        glScalef(scene.mScale[i][X], scene.mScale[i][Y], scene.mScale[i][Z]);
        renderObj(objs[i]);
        glPopMatrix();
    }
}

void lighting()
{
    //clear the buffer
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //enable lighting
    glEnable(GL_LIGHTING);

    //set light property
    for (int i = 0; i < light.mObjLight.size(); i++)
    {
        cout << "light " << i << endl;
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

void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'w'://zoom in
            view.mEye[X] += view.mUnitVat[X] * zoomDegree;
            view.mEye[Y] += view.mUnitVat[Y] * zoomDegree;
            view.mEye[Z] += view.mUnitVat[Z] * zoomDegree;
            break;
        case 's'://zoom out
            view.mEye[X] -= view.mUnitVat[X] * zoomDegree;
            view.mEye[Y] -= view.mUnitVat[Y] * zoomDegree;
            view.mEye[Z] -= view.mUnitVat[Z] * zoomDegree;
            break;
        case 'a'://move left (circle the center)
            if (view.mVat[X] >= zoomDegree*100) view.mVat[X] = -zoomDegree*100;
            view.mVat[X] += zoomDegree;
            break;
        case 'd'://move right (circle the center)
            if (view.mVat[X] <= -zoomDegree*100) view.mVat[X] = zoomDegree*100;
            view.mVat[X] -= zoomDegree;
            break;
        case '1':
            selectedObj = 0;
            break;
        case '2':
            selectedObj = 1;
            break;
        case '3':
            selectedObj = 2;
            break;
        case '4':
            selectedObj = 3;
            break;
        case '5':
            selectedObj = 4;
            break;
        case '6':
            selectedObj = 5;
            break;
        case '7':
            selectedObj = 6;
            break;
        case '8':
            selectedObj = 7;
            break;
        case '9':
            selectedObj = 8;
            break;
    }

    glutPostRedisplay();
    return;
}

void Mouse(int button, int state, int x, int y)
{
    static int last_x, last_y;

    switch (state)
    {
    case GLUT_UP:
        if (selectedObj >= 0)
        {
            printf("move: %d %d\n", x - last_x, y - last_y);
            scene.mTransfer[selectedObj][X] += (double) ((x - last_x) / dragDegree);
            scene.mTransfer[selectedObj][Y] -= (double) ((y - last_y) / dragDegree);
        }
        break;

    case GLUT_DOWN:
        last_x = x;
        last_y = y;
        break;
    }

    glutPostRedisplay();
    return;
}