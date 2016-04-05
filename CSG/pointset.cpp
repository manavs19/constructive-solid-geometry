#include "pointset.h"
#include <algorithm>
#include <climits>

pointset::pointset()
{
    points.clear();
}

pointset::pointset(set<Point3D> points)
{
    this->points = points;
}

pointset::pointset(pointset& ps) {
    points = ps.points;
}

void pointset::union_(pointset& ps) {
    set<Point3D> voxels;
    set_union(points.begin(),points.end(),ps.points.begin(),ps.points.end(),
                    inserter(voxels,voxels.begin()));
    points = voxels;
}

void pointset::difference_(pointset& ps) {
    set<Point3D> voxels;
    set_difference(points.begin(),points.end(),ps.points.begin(),ps.points.end(),
                    inserter(voxels,voxels.begin()));
    points = voxels;
}

void pointset::intersection_(pointset& ps) {
    set<Point3D> voxels;
    set_intersection(points.begin(),points.end(),ps.points.begin(),ps.points.end(),
                    inserter(voxels,voxels.begin()));
    points = voxels;
}

void pointset::setPoints(set<Point3D> newpoints) {
    points = newpoints;
}

void pointset::translate(double x, double y, double z) {
    set<Point3D> npoints;
    for(set<Point3D>::iterator it = points.begin(); it != points.end(); it++) {
        Point3D p(it->x + x, it->y + y, it->z + z);
        npoints.insert(p);
    }
    points = npoints;
}

Point3D pointset::findMin() {
    double xmin=INT_MAX*1.0, ymin=INT_MAX*1.0, zmin=INT_MAX*1.0;
    for(set<Point3D>::iterator it = points.begin(); it != points.end(); it++) {
        if(xmin > it->x) {
            xmin = it->x;
        }

        if(ymin > it->y) {
            ymin = it->y;
        }

        if(zmin > it->z) {
            zmin = it->z;
        }
    }
    return Point3D(xmin, ymin, zmin);
}

void pointset::scale(double x, double y, double z) {
    Point3D pmin = this->findMin();
    set<Point3D> npoints;
    for(set<Point3D>::iterator it = points.begin(); it != points.end(); it++) {
        Point3D p(it->x*x, it->y*y, it->z*z);
        npoints.insert(p);
    }
    points = npoints;
    this->translate(-pmin.x, -pmin.y, -pmin.z);
}
