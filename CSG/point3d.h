#ifndef POINT3D_H
#define POINT3D_H

class Point3D
{
public:
    double x, y, z;
    Point3D(double, double, double);
    Point3D();
    bool operator<(const Point3D&)const;
};

#endif // POINT3D_H
