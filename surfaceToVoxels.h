#ifndef SURFACETOVOXELS_H
#define SURFACETOVOXELS_H

#endif // SURFACETOVOXELS_H

#include <set>
#include "point3d.h"
#include <string>

struct Point2D
{
    int x;
    int y;
};

//struct Point3D
//{
//    int x;
//    int y;
//    int z;
//    bool operator<(const Point3D &p)const
//    {
//        if(x < p.x)return true;
//        if(x > p.x)return false;

//        //x are equal
//        if(y < p.y)return true;
//        if(y > p.y)return false;

//        //x and y are equal
//        if(z < p.z)return true;
//        return false;
//    }
//};

std::set<Point3D> surfaceToVoxels(std::string filename);
