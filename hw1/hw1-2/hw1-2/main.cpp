#include "headers.h"

SceneLoader scene;
ViewLoader view;
LightLoader light;
map<int, mesh> objs;

double zoomDegree = 0.0, dragDegree = 0.0, rotateDegree = M_PI / 180;
int selectedObj = -1;

int numOfTextures;

Srcpath files;
string srcRootPath;

int main(int argc, char **argv)
{   
    
    int res = 0;
    printf("Please input the scene you want to test(1: Park; 2: Chess): ");
    cin >> res;

    switch (res)
    {
        case 1:
            srcRootPath = files.srcRootPath_park;
            for (int i = 0; i < 9; i++)
                objs.insert(pair<int, mesh>(i, mesh(srcRootPath + files.oNames[i], res)));

            light.loadLight(files.srcRootPath_park + string("park.light"));
            scene.loadScene(files.srcRootPath_park + string("park.scene"));
            view.loadView(files.srcRootPath_park + string("park.view"));

            zoomDegree = 20.0;
            dragDegree = 0.8;
            break;

        case 2:
            srcRootPath = files.srcRootPath_chess;
            for (int i = 9; i < 17; i++)
                objs.insert(pair<int, mesh>(i, mesh(srcRootPath + files.oNames[i], res)));

            light.loadLight(files.srcRootPath_chess + string("Chess.light"));
            scene.loadScene(files.srcRootPath_chess + string("Chess.scene"));
            view.loadView(files.srcRootPath_chess + string("Chess.view"));

            zoomDegree = 0.1;
            dragDegree = 50.0;
            break;

        default:
            cout << "Error input" << endl;
            break;
    }

    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("HW1-2");
    glewInit();
    glutDisplayFunc(Display);
    glutReshapeFunc(ReShape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMainLoop();
    return 0;
}

void Display()
{
    //clear the buffer
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
    unsigned int *texObj;
    texObj = new unsigned int[scene.mNumOfTextures];
    glGenTextures(scene.mNumOfTextures, texObj);
    int texObjIndex = 0;

    for (int i = 0; i < scene.mComponents.size(); i++)
    {
        Texture tex = scene.mComponents[i].mTex;
        
        //prepare texture
        int index = texObjIndex;
        FreeImage_Initialise();
        loadTexture(&tex, texObj, &texObjIndex);
        FreeImage_DeInitialise();
        cout << "load texture" << endl;
        //system("pause");

        for (int j = 0; j < scene.mComponents[i].mNumOfModels; j++)
        {
            Model model = scene.mComponents[i].mModels[j];
            cout << "TRS " << files.oNames[model.mObjIndex] << endl;

            if (tex.mType == SINGLE_TEXTURE)
            {
                //glEnable(GL_ALPHA_TEST);
                //glAlphaFunc(GL_GREATER, 0.5f);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texObj[index]);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                cout << "bind tex " << index << endl;
            }

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

            renderObj(objs[model.mObjIndex], texObj, tex.mType, index);
            glPopMatrix();

            if (tex.mType == SINGLE_TEXTURE)
            {
                glDisable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
                cout << "unbind tex " << texObjIndex << endl;
            }
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
            //scene.mModels[selectedObj].mTransfer[X] += (double)((x - last_x) / dragDegree);
            //scene.mModels[selectedObj].mTransfer[Y] -= (double)((y - last_y) / dragDegree);
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
