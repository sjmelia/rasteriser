#ifndef PROJECTION_H
#define PROJECTION_H
#include "matrix4.h"

void perspective(matrix4* projection, double fov, double aspect, double nearplane, double farplane);

#endif
