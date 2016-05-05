#ifndef CREATEOBJECTS_H
#define CREATEOBJECTS_H

#include "pointset.h"
#include "point3d.h"

class CreateObjects
{
public:
    static pointset* createLine(int, int, int, int, int, int);
    static pointset createTriangle(int, int, int, int, int, int, int, int, int);
    static pointset createCylinder(int, int, Point3D);
    CreateObjects();
};

#endif // CREATEOBJECTS_H
