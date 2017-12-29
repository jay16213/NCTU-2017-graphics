#include "headers.h"

int gw, gh;				//Keep track of window width and height
GLhandleARB	MyShader;
ViewLoader view;
SceneLoader scene;
LightLoader light;
vector<mesh> objs;

int main(int argc, char **argv)
{
    Srcpath *files = new Srcpath();
    for (size_t i = 0; i < files->oNames.size(); i++)
        objs.push_back(mesh(files->oNames[i]));

    view.loadView("view.view");
    scene.loadScene("scene.scene");
    light.loadLight("light.light");

    gw = view.mViewport[2];
    gh = view.mViewport[3];

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Assignment 3");

    glutDisplayFunc(Display);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(Keyboard);

    glewInit();
    if (glewIsSupported("GL_VERSION_2_1"))
        printf("Ready for OpenGL 2.1\n");
    else {
        printf("OpenGL 2.1 not supported\n");
        exit(1);
    }
    if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GL_EXT_geometry_shader4)
        printf("Ready for GLSL - vertex, fragment, and geometry units\n");
    else {
        printf("Not totally ready :( \n");
        exit(1);
    }

    LoadShaders();

    glutMainLoop();
    return 0;
}

void LoadShaders()
{
    MyShader = glCreateProgram();
    if (MyShader != 0)
    {
        ShaderLoad(MyShader, "../shader/vertexShader.vert", GL_VERTEX_SHADER);
        ShaderLoad(MyShader, "../shader/fragShader.frag", GL_FRAGMENT_SHADER);
    }
}

void lighting()
{
    //enable lighting
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

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

void Display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //stretch to screen
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(view.mFovy, view.mAspect, view.mDnear, view.mDfar);
    glViewport(view.mViewport[X], view.mViewport[Y], gw, gh);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        view.mEye[X], view.mEye[Y], view.mEye[Z],     // eye
        view.mVat[X], view.mVat[Y], view.mVat[Z],     // center
        view.mVup[X], view.mVup[Y], view.mVup[Z]      // up
    );

    glUseProgram(MyShader);
    glUniform1i(glGetUniformLocation(MyShader, "lightNumber"), light.mObjLight.size());

    lighting();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    for (int i = 0; i < scene.mComponents.size(); i++)
    {
        for (int j = 0; j < scene.mComponents[i].mNumOfModels; j++)
        {
            Model *m = &scene.mComponents[i].mModels[j];
            glPushMatrix();
            glTranslatef(m->mTransfer[X], m->mTransfer[Y], m->mTransfer[Z]);
            glRotatef(m->mAngle, m->mRotateAxisVec[X], m->mRotateAxisVec[Y], m->mRotateAxisVec[Z]);
            glScalef(m->mScale[X], m->mScale[Y], m->mScale[Z]);

            renderObj(&objs[m->mObjIndex]);
            glPopMatrix();
        }
    }

    glUseProgram(0);
    glutSwapBuffers();
    return;
}

void Keyboard(unsigned char key, int x, int y)
{
    double eye_x = view.mEye[X];
    double eye_z = view.mEye[Z];
    double zoomDegree = 5.0;
    double rotateDegree = M_PI / 180;

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
    }

    glutPostRedisplay();
    return;
}

//GLUT callback fx
// called when window size changes
void changeSize(int w, int h) {
    //stores the width and height
    gw = w;
    gh = h;
}
