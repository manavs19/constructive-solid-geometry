#include "pointset.h"
#include <algorithm>
#include <climits>
#include <cmath>

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
        p.color = it->color;
        npoints.insert(p);
    }
    points = npoints;
}

Point3D pointset::findMax() {
    double xmax=INT_MIN*1.0, ymax=INT_MIN*1.0, zmax=INT_MIN*1.0;
    for(set<Point3D>::iterator it = points.begin(); it != points.end(); it++) {
        if(xmax < it->x) {
            xmax = it->x;
        }

        if(ymax < it->y) {
            ymax = it->y;
        }

        if(zmax < it->z) {
            zmax = it->z;
        }
    }
    return Point3D(xmax, ymax, zmax);
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

void pointset::rotate(double tx, double ty, double tz) {
    Point3D pmin = this->findMin();
    Point3D pmax = this->findMax();
    Point3D center((pmin.x+pmax.x)/2, (pmin.y+pmax.y)/2, (pmin.z+pmax.z)/2);
    this->translate(-center.x, -center.y, -center.z);

    set<Point3D> npoints;
    for(set<Point3D>::iterator it = points.begin(); it != points.end(); it++) {
        Point3D p;
        double a = it->y, b = it->z;
        p.y = a*cos(tx)-b*sin(tx);
        p.z = a*sin(tx)+b*cos(tx);

        a = it->x; b = p.z;
        p.x = a*cos(ty)+b*sin(ty);
        p.z = -a*sin(ty)+b*cos(ty);

        a = p.x, b = p.y;
        p.x = a*cos(tz)-b*sin(tz);
        p.y = a*sin(tz)+b*cos(tz);
        p.color = it->color;
        npoints.insert(p);
    }

    points = npoints;

    this->translate(center.x, center.y, center.z);
}
