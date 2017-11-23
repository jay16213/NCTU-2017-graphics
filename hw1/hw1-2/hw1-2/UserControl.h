#ifndef USER_CONTROL_H
#define USER_CONTROL_H

#include "ViewLoader.h"
#include "SceneLoader.h"
#include "glut.h"
#include <cmath>

#define X 0
#define Y 1
#define Z 2

class UserControl
{
public:
    UserControl();
    ~UserControl();

    void init(
        ViewLoader *view,
        SceneLoader *scene,
        double zoomDegree,
        double rotateDegree,
        double dragDegree
    );
    void keyboard(unsigned char key, int x, int y);
    void mouse(int button, int state, int x, int y);

private:
    ViewLoader *mView;
    SceneLoader *mScene;
    int mSelectedObj;
    double mZoomDegree;
    double mRotateDegree;
    double mDragDegree;
};

#endif
