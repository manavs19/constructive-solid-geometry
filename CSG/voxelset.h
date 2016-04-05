#ifndef VOXELSET_H
#define VOXELSET_H

#include <vector>
#include "point3d.h"
#include "pointset.h"
#include <set>

class VoxelSet
{
public:
    std::vector<Point3D> vertices;
    std::vector<std::vector<int> > faces;
    Point3D center, eye;
    double scalex, scaley, scalez;
    VoxelSet();
    void ReadFromFile(const char*);
    void FromPointset(pointset&);
    set<Point3D> createPointSet();
    void save(const char*);
    void setScale();
};

#endif // VOXELSET_H
