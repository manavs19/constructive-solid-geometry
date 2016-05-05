#ifndef POINTSET_H
#define POINTSET_H
#include <set>
#include "point3d.h"

using namespace std;

class pointset
{
private:
    Point3D findMin();
    Point3D findMax();
public:
    set<Point3D> points;
    void union_(pointset&);
    void difference_(pointset&);
    void intersection_(pointset&);
    void setPoints(set<Point3D>);
    void translate(double, double, double);
    void rotate(double, double, double);
    void resetColor();
    pointset();
    pointset(set<Point3D>);
    pointset(pointset&);
};

#endif // POINTSET_H
