#include "point3d.h"

Point3D::Point3D(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Point3D::Point3D() {
    x=0; y=0; z=0;
}

bool Point3D::operator<(const Point3D &p)const
{
    if(x < p.x)return true;
    if(x > p.x)return false;

    //x are equal
    if(y < p.y)return true;
    if(y > p.y)return false;

    //x and y are equal
    if(z < p.z)return true;
    return false;
}
