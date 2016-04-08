#include "createobjects.h"
#include <set>
#include <vector>

using namespace std;

CreateObjects::CreateObjects()
{
}

vector<Point3D> get3DLine(int dx, int dy, int dz) {
    vector<Point3D> line;
    int fxy = 2*dy - dx, fxz = 2*dz - dx;
    int x = 0, y = 0, z = 0;

    Point3D p(x,y,z);
    line.push_back(p);
    // cout << x << " " <<y << " " << z << "\n";
    while(x < dx) {
        x++;
        if(fxy>0) {
            //NE
            y++;
            fxy += 2*(dy-dx);
        }
        else {
            //E
            fxy += 2*dy;
        }

        if(fxz>0) {
            //NE
            z++;
            fxz += 2*(dz-dx);
        }
        else {
            //E
            fxz += 2*dz;
        }

        Point3D p (x,y,z);
        line.push_back(p);
        // cout << x << " " <<y << " " << z << "\n";

    }
    return line;
}

pointset* CreateObjects::createLine(int xs, int ys, int zs, int xe, int ye, int ze) {
    set<Point3D> points;
    int dx = xe-xs, abs_dx = (dx<0)?-dx:dx;
    int dy = ye-ys, abs_dy = (dy<0)?-dy:dy;
    int dz = ze-zs, abs_dz = (dz<0)?-dz:dz;

    int xsign = dx>0?1:-1;
    int ysign = dy>0?1:-1;
    int zsign = dz>0?1:-1;

    if(abs_dx>abs_dy) {

        if(abs_dx>abs_dz) {
            vector<Point3D> line3d = get3DLine(abs_dx, abs_dy, abs_dz);
            for (int i = 0; i < line3d.size(); ++i)
            {
                points.insert(Point3D(xs+xsign*line3d[i].x, ys+ysign*line3d[i].y, zs+zsign*line3d[i].z));
            }
        }
        else{
            vector<Point3D> line3d = get3DLine(abs_dz, abs_dy, abs_dx);
            for (int i = 0; i < line3d.size(); ++i)
            {
                points.insert(Point3D(xs+xsign*line3d[i].z, ys+ysign*line3d[i].y, zs+zsign*line3d[i].x));
            }
        }
    }
    else {

        if(abs_dy > abs_dz) {
            vector<Point3D> line3d = get3DLine(abs_dy, abs_dx, abs_dz);
            for (int i = 0; i < line3d.size(); ++i)
            {
                points.insert(Point3D(xs+xsign*line3d[i].y, ys+ysign*line3d[i].x, zs+zsign*line3d[i].z));
            }
        }
        else {
            vector<Point3D> line3d = get3DLine(abs_dz, abs_dy, abs_dx);
            for (int i = 0; i < line3d.size(); ++i)
            {
                points.insert(Point3D(xs+xsign*line3d[i].z, ys+ysign*line3d[i].y, zs+zsign*line3d[i].x));
            }
        }
    }
    return new pointset(points);
}

pointset CreateObjects::createTriangle(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3) {

}
