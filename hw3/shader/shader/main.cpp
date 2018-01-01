#include "headers.h"

int gw, gh;				//Keep track of window width and height
GLhandleARB	MyShader;

Srcpath *files;
ViewLoader view;
SceneLoader scene;
LightLoader light;
vector<mesh> objs;

GLuint texObj[1];
int texObjIndex = 0;

int res;
int level = 0;

double zoomDegree;
double rotateDegree = M_PI / 36;

int main(int argc, char **argv)
{
    files = new Srcpath();
    cout << "Choose the scene(1: subdivision/2: phong shaging): ";

    while (cin >> res)
    {
        if (res == 1)
        {
            objs.push_back(mesh(string("gem/gem.obj"), res));
            view.loadView(files->srcRootPath_sub + string("as3.view"));
            scene.loadScene(files->srcRootPath_sub + string("as3.scene"), res);
            light.loadLight(files->srcRootPath_sub + string("as3.light"));
            double zoomDegree = 0.06;
            break;
        }
        else if (res == 2)
        {
            objs.push_back(mesh(string("phong/box.obj"), res));
            objs.push_back(mesh(string("phong/bunny.obj"), res));
            objs.push_back(mesh(string("phong/venus.obj"), res));
            view.loadView(files->srcRootPath_pho + string("view.view"));
            scene.loadScene(files->srcRootPath_pho + string("scene.scene"), res);
            light.loadLight(files->srcRootPath_pho + string("light.light"));
            double zoomDegree = 5.0;
            break;
        }
        else
        {
            cout << "Invalid choice" << endl;
            continue;
        }
    }
    
    gw = view.mViewport[2];
    gh = view.mViewport[3];

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(view.mViewport[2], view.mViewport[3]);
    glutCreateWindow("Assignment 3");

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

    glutDisplayFunc(Display);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(Keyboard);

    if (res == 1)
    {
        Texture *tex = &scene.mComponents[0].mTex;
        glGenTextures(scene.mNumOfTextures, texObj);
        FreeImage_Initialise();
        loadTexture(tex, texObj, &texObjIndex);
        FreeImage_DeInitialise();
    }

    LoadShaders(res);
    glutMainLoop();

    delete files;
    return 0;
}

void LoadShaders(int res)
{
    MyShader = glCreateProgram();
    if (MyShader != 0)
    {
        if (res == 1)
        {
            ShaderLoad(MyShader, "../shader/vertexShader_s.vert", GL_VERTEX_SHADER);
            ShaderLoad(MyShader, "../shader/geomShader_s.geom", GL_GEOMETRY_SHADER);
            ShaderLoad(MyShader, "../shader/fragShader_s.frag", GL_FRAGMENT_SHADER);
        }
        else
        {
            ShaderLoad(MyShader, "../shader/vertexShader_p.vert", GL_VERTEX_SHADER);
            ShaderLoad(MyShader, "../shader/fragShader_p.frag", GL_FRAGMENT_SHADER);
        }
    }
}

void lighting()
{
    //enable lighting
    glEnable(GL_LIGHTING);
    //glShadeModel(GL_SMOOTH);

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

    if (res == 1)
    {
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texObj[0]);
        glUniform1i(glGetUniformLocation(MyShader, "colorTexture"), 0);
        glUniform1i(glGetUniformLocation(MyShader, "level"), level);
        glUniform1f(glGetUniformLocation(MyShader, "radius"), 1.0f);
        glUniform4f(glGetUniformLocation(MyShader, "Centroid"), 0.0f, 1.0f, 0.0f, 1.0f);
    }

    lighting();

    for (size_t i = 0; i < scene.mComponents.size(); i++)
    {
        Texture *tex = &scene.mComponents[0].mTex;

        for (int j = 0; j < scene.mComponents[i].mNumOfModels; j++)
        {
            Model *m = &scene.mComponents[i].mModels[j];
            glPushMatrix();
            glTranslatef(m->mTransfer[X], m->mTransfer[Y], m->mTransfer[Z]);
            glRotatef(m->mAngle, m->mRotateAxisVec[X], m->mRotateAxisVec[Y], m->mRotateAxisVec[Z]);
            glScalef(m->mScale[X], m->mScale[Y], m->mScale[Z]);

            renderObj(&objs[m->mObjIndex], texObj, tex->mType, texObjIndex);
            glPopMatrix();
        }
    }

    glFlush();
    glUseProgram(0);
    glutSwapBuffers();
    return;
}

void Keyboard(unsigned char key, int x, int y)
{
    double ax = view.mEye[X] - view.mVat[X];
    double ay = view.mEye[Y] - view.mVat[Y];
    double az = view.mEye[Z] - view.mVat[Z];

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
            view.mEye[X] = cos(rotateDegree)*ax - sin(rotateDegree)*az + view.mVat[X];
            view.mEye[Z] = sin(rotateDegree)*ax + cos(rotateDegree)*az + view.mVat[Z];
            view.updateUnitVat();
            break;

        case 'd'://move right (circle the center)   
            view.mEye[X] = cos(-rotateDegree)*ax - sin(-rotateDegree)*az + view.mVat[X];
            view.mEye[Z] = sin(-rotateDegree)*ax + cos(-rotateDegree)*az + view.mVat[Z];
            view.updateUnitVat();
            break;

        case 'f'://move the light position(y--)
            for (size_t i = 0; i < light.mObjLight.size(); i++)
            {
                light.mObjLight[i].mPosition[Y] -= 10;
            }
            break;
        case 'r'://move the light position(y++)
            for (size_t i = 0; i < light.mObjLight.size(); i++)
            {
                light.mObjLight[i].mPosition[Y] += 10;
            }
            break;
        case '0':
            level = 0;
            break;
        case '1':
            level = 1;
            break;
        case '2':
            level = 2;
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
