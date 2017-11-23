#ifndef PIPELINE_H
#define PIPELINE_H

#include "headers.h"

void loadPipelineData();
void Display();
void ReShape(int w, int h);
void renderObj(mesh obj);
void objViewTransform();
void lighting();
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);

#endif
