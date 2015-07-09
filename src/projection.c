#include <math.h>
#include "matrix4.h"

void frustum(matrix4* m, double left, double right, double bottom, double top, double znear, double zfar)
{
    m->a1 = (2 * znear) / (right - left);
    m->a3 = (right + left) / (right - left);
    m->b2 = (2 * znear) / (top - bottom);
    m->b3 = (top + bottom) / (top - bottom);
    m->c3 = - ((zfar + znear) / (zfar - znear));
    m->c4 = - ((2 * zfar * znear) / (zfar - znear));
    m->d3 = -1;
}

void perspective(matrix4* projection, double fov, double aspect, double nearplane, double farplane)
{
    double xmin, xmax, ymin, ymax;
    ymax = nearplane * (double)tan(fov * M_PI / 360);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;
    frustum(projection, xmin, xmax, ymin, ymax, nearplane, farplane);
}

