#include "headers.h"

SceneLoader scene;
ViewLoader view;
LightLoader light;
map<int, mesh> objs;

double zoomDegree = 0.0, dragDegree = 0.0, rotateDegree = M_PI / 180;
int selectedObj = -1;

int numOfTextures;
unsigned int *texObjs;

int main(int argc, char **argv)
{   
    Srcpath files;
    int res = 0;
    printf("Please input the scene you want to test(1: Park; 2: Chess): ");
    cin >> res;

    switch (res)
    {
        case 1:
            for (int i = 0; i < 9; i++)
                objs.insert(pair<int, mesh>(i, files.srcRootPath_park + files.fNames[i]));
            light.loadLight(files.srcRootPath_park + string("park.light"));
            scene.loadScene(files.srcRootPath_park + string("park.scene"));
            view.loadView(files.srcRootPath_park + string("park.view"));
            numOfTextures = 15;
            texObjs = new unsigned int[numOfTextures];
            FreeImage_Initialise();
            zoomDegree = 20.0;
            dragDegree = 0.8;
            break;

        case 2:
            for (int i = 9; i < 17; i++)
                objs.insert(pair<int, mesh>(i, files.srcRootPath_chess + files.fNames[i]));
            light.loadLight(files.srcRootPath_chess + string("Chess.light"));
            scene.loadScene(files.srcRootPath_chess + string("Chess.scene"));
            view.loadView(files.srcRootPath_chess + string("Chess.view"));
            zoomDegree = 0.1;
            dragDegree = 50.0;
            break;

        default:
            cout << "Error input" << endl;
            exit(0);
            break;
    }

    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("HW1-2");
    glutDisplayFunc(Display);
    glutReshapeFunc(ReShape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMainLoop();
    return 0;
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
            scene.mObjects[i].mTransfer[X],
            scene.mObjects[i].mTransfer[Y],
            scene.mObjects[i].mTransfer[Z]
        );
        glRotatef(
            scene.mObjects[i].mRotate.mAngle,
            scene.mObjects[i].mRotate.mAxisVec[X],
            scene.mObjects[i].mRotate.mAxisVec[Y],
            scene.mObjects[i].mRotate.mAxisVec[Z]
        );
        glScalef(
            scene.mObjects[i].mScale[X],
            scene.mObjects[i].mScale[Y],
            scene.mObjects[i].mScale[Z]
        );
        renderObj(objs[scene.mObjects[i].mId], i);
        glPopMatrix();
    }
}

void renderObj(mesh obj, int index)
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

        switch (scene.mObjects[index].mTextureType)
        {
            case NO_TEXTURE:
                break;
            case SINGLE_TEXTURE:
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texObjs[scene.mObjects[index].mTexObjIndex]);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                break;
            case MULTI_TEXTURE:
                glActiveTexture(GL_TEXTURE0);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texObjs[scene.mObjects[index].mTexObjIndex]);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
                glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

                glActiveTexture(GL_TEXTURE1);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texObjs[scene.mObjects[index].mTexObjIndex]);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
                glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
                break;
            case CUBE_MAP:
                glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
                glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
                glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
                glEnable(GL_TEXTURE_GEN_S);
                glEnable(GL_TEXTURE_GEN_T);
                glEnable(GL_TEXTURE_GEN_R);
                glEnable(GL_TEXTURE_CUBE_MAP);
                glBindTexture(GL_TEXTURE_CUBE_MAP, texObjs[scene.mObjects[index].mTexObjIndex]);
                break;
        }

        glBegin(GL_TRIANGLES);
        for (size_t j = 0; j<3; ++j)
        {
            //textex corrd. object->tList[object->faceList[i][j].t].ptr
            if (scene.mObjects[index].mTextureType == SINGLE_TEXTURE)
            {
                glTexCoord2fv(obj.tList[obj.faceList[i][j].t].ptr);
            }
            else if (scene.mObjects[index].mTextureType == MULTI_TEXTURE)
            {
                for (int k = 0; k < scene.mTexFiles[scene.mObjects[index].mTexObjIndex].mNumOfFiles; k++)
                    glMultiTexCoord2fv(GL_TEXTURE0 + k, obj.tList[obj.faceList[i][j].t].ptr);             
            }
            
            glNormal3fv(obj.nList[obj.faceList[i][j].n].ptr);
            glVertex3fv(obj.vList[obj.faceList[i][j].v].ptr);
        }
        glEnd();
    }
}

void loadTexture(int texObjIndex, int textureType)
{
    FIBITMAP *pImg;
    FIBITMAP *p32BitsImg;
    int width;
    int height;

    switch (textureType)
    {
        case SINGLE_TEXTURE:
            glBindTexture(GL_TEXTURE_2D, texObjs[texObjIndex]);

            pImg = FreeImage_Load(
                FreeImage_GetFileType(scene.mTexFiles[texObjIndex].mFiles[0].c_str(), 0),
                scene.mTexFiles[texObjIndex].mFiles[0].c_str()
            );
            p32BitsImg = FreeImage_ConvertTo32Bits(pImg);
            width = FreeImage_GetWidth(p32BitsImg);
            height = FreeImage_GetHeight(p32BitsImg);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                GL_BGRA, GL_UNSIGNED_BYTE, (void *) FreeImage_GetBits(p32BitsImg));
            
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.5f);

            glGenerateMipmap(GL_TEXTURE_2D);
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

            FreeImage_Unload(p32BitsImg);
            FreeImage_Unload(pImg);
            break;

        case MULTI_TEXTURE:
            glBindTexture(GL_TEXTURE_2D, texObjs[texObjIndex]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            for (size_t i = 0; i < scene.mTexFiles[texObjIndex].mFiles.size(); i++)
            {
                pImg = FreeImage_Load(
                    FreeImage_GetFileType(scene.mTexFiles[texObjIndex].mFiles[i].c_str(), 0),
                    scene.mTexFiles[texObjIndex].mFiles[i].c_str()
                    );
                p32BitsImg = FreeImage_ConvertTo32Bits(pImg);
                width = FreeImage_GetWidth(p32BitsImg);
                height = FreeImage_GetHeight(p32BitsImg);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                    GL_BGRA, GL_UNSIGNED_BYTE, (void *)FreeImage_GetBits(p32BitsImg));
                FreeImage_Unload(p32BitsImg);
                FreeImage_Unload(pImg);
            }
            glGenerateMipmap(GL_TEXTURE_2D);
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
            break;

        case CUBE_MAP:
            glBindTexture(GL_TEXTURE_CUBE_MAP, texObjs[texObjIndex]);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            for (size_t i = 0; i < scene.mTexFiles[texObjIndex].mFiles.size(); i++)
            {
                pImg = FreeImage_Load(
                    FreeImage_GetFileType(scene.mTexFiles[texObjIndex].mFiles[i].c_str(), 0),
                    scene.mTexFiles[texObjIndex].mFiles[i].c_str()
                    );
                p32BitsImg = FreeImage_ConvertTo32Bits(pImg);
                width = FreeImage_GetWidth(p32BitsImg);
                height = FreeImage_GetHeight(p32BitsImg);
                
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0,
                    GL_BGRA, GL_UNSIGNED_BYTE, (void *)FreeImage_GetBits(p32BitsImg));

                FreeImage_Unload(p32BitsImg);
                FreeImage_Unload(pImg);
            }
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
            break;
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
            scene.mObjects[selectedObj].mTransfer[X] += (double)((x - last_x) / dragDegree);
            scene.mObjects[selectedObj].mTransfer[Y] -= (double)((y - last_y) / dragDegree);
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
