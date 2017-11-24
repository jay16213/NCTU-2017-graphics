#ifndef HEADERS_H
#define HEADERS_H

#define _USE_MATH_DEFINES
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <math.h>
#include <map>
#include <cmath>
#include "glew.h"
#include "glut.h"
#include "FreeImage.h"
#include "mesh.h"
#include "ViewLoader.h"
#include "LightLoader.h"
#include "SceneLoader.h"
#include "pipeline.h"
#include "Srcpath.h"
using namespace std;

#define X 0
#define Y 1
#define Z 2

void Display();
void ReShape(int w, int h);
void renderObj(mesh obj, int index);
void objViewTransform();
void lighting();
void loadTexture();
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);

#endif
