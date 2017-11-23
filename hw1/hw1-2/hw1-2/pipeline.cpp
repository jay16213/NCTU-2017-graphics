#include "headers.h"

void loadPipeLineData()
{
    int res = 0;
    printf("Please input the scene you want to test(1: Park; 2: Chess): ");
    cin >> res;

    switch (res)
    {
        case 1:
            /* load light, view, scene */
            light.loadLight(srcRootPath_park + string("park.light"));
            scene.loadScene(srcRootPath_park + string("park.scene"));
            view.loadView(srcRootPath_park + string("park.view"));
            zoomDegree = 20.0;
            dragDegree = 0.8;
            break;

        case 2:
            /* load light, view, scene */
            light.loadLight(srcRootPath_chess + string("Chess.light"));
            scene.loadScene(srcRootPath_chess + string("Chess.scene"));
            view.loadView(srcRootPath_chess + string("Chess.view"));
            zoomDegree = 0.1;
            dragDegree = 50.0;
            break;

        default:
            cout << "Error input" << endl;
            exit(0);
            break;
    }
    return;
}

void Display()
{
    //clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    objViewTransform();

    glutSwapBuffers();
    return;
}

void lighting()
{
    //clear the buffer
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);


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
        renderObj(objects[i]);
        glPopMatrix();
    }
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
            scene.mTransfer[selectedObj][X] += (double)((x - last_x) / dragDegree);
            scene.mTransfer[selectedObj][Y] -= (double)((y - last_y) / dragDegree);
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
