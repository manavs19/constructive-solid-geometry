#ifndef POINTSET_H
#define POINTSET_H
#include <set>
#include "point3d.h"

using namespace std;

class pointset
{
private:
    Point3D findMin();
public:
    set<Point3D> points;
    void union_(pointset&);
    void difference_(pointset&);
    void intersection_(pointset&);
    void setPoints(set<Point3D>);
    void translate(double, double, double);
    void scale(double, double, double);
    pointset();
    pointset(set<Point3D>);
    pointset(pointset&);
};

#endif // POINTSET_H
