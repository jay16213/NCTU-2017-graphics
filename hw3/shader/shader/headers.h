#ifndef HEADERS_H
#define HEADERS_H

#define _USE_MATH_DEFINES
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <math.h>
#include <cmath>
#include <map>
#include "FreeImage.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "ShaderLoader.h"
#include "LightLoader.h"
#include "SceneLoader.h"
#include "ViewLoader.h"
#include "Srcpath.h"
#include "mesh.h"
using namespace std;

#define X 0
#define Y 1
#define Z 2

void Display();
void ReShape(int w, int h);
void objViewTransform(int depth);
void lighting();
void renderObj(mesh *obj);
void Keyboard(unsigned char key, int x, int y);
void changeSize(int w, int h);

void LoadShaders();

#endif
