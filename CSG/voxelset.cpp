#include "voxelset.h"
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <QTextStream>
#include <set>
#include <utility>
#include <map>
#include <climits>

using namespace std;
vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while(ss >> tok) {
        internal.push_back(tok);
    }

    return internal;
}


VoxelSet::VoxelSet() : center(0,0,0), eye(0,0,0)
{
    scalex = 10.0;
    scaley = 10.0;
    scalez = 10.0;

}

vector<int> getFace(int a, int b, int c, int d) {
    vector<int> face;
    face.push_back(a);
    face.push_back(b);
    face.push_back(c);
    face.push_back(d);
    return face;
}

void VoxelSet::FromPointset(pointset &ps) {
    vertices.resize(0);
    faces.resize(0);

    for(set<Point3D>::iterator it = ps.points.begin();it!=ps.points.end();++it) {

        double x=it->x, y=it->y, z=it->z;
        int numLines = vertices.size();
        vertices.push_back(Point3D(x-0.5, y-0.5, z+0.5));
        vertices.push_back(Point3D(x-0.5, y-0.5, z-0.5));
        vertices.push_back(Point3D(x-0.5, y+0.5, z+0.5));
        vertices.push_back(Point3D(x-0.5, y+0.5, z-0.5));
        vertices.push_back(Point3D(x+0.5, y-0.5, z+0.5));
        vertices.push_back(Point3D(x+0.5, y-0.5, z-0.5));
        vertices.push_back(Point3D(x+0.5, y+0.5, z+0.5));
        vertices.push_back(Point3D(x+0.5, y+0.5, z-0.5));

        faces.push_back(getFace(numLines, 4+numLines, 6+numLines, 2+numLines));
        faces.push_back(getFace(4+numLines, 5+numLines, 7+numLines, 6+numLines));
        faces.push_back(getFace(1+numLines, 3+numLines, 7+numLines, 5+numLines));
        faces.push_back(getFace(numLines, 2+numLines, 3+numLines, 1+numLines));
        faces.push_back(getFace(2+numLines, 6+numLines, 7+numLines, 3+numLines));
        faces.push_back(getFace(1+numLines, 5+numLines, 4+numLines, numLines));

    }
}

void VoxelSet::ReadFromFile(const char* filename) {
    vertices.resize(0);
    faces.resize(0);
    ifstream fin;
    fin.open(filename, ios::in);

    while(fin) {
        string s;
        getline(fin, s);
        vector<string> tokens = split(s, ' ');
        if(tokens.size() > 0) {
            // cout << tokens[0];
            if(tokens[0].compare("v") == 0) {
                if(tokens.size() != 4){
                    QTextStream(stderr) << "ERROR: Invalid file format 1\n";
                    return;
                }
                double x = atof(tokens[1].c_str()), y = atof(tokens[2].c_str()), z = atof(tokens[3].c_str());
                // cout << tokens[3];
                Point3D p(x, y, z);
                vertices.push_back(p);

            }

            else if(tokens[0].compare("f") == 0) {
                if(tokens.size() < 4){
                    QTextStream(stderr) << "ERROR: Invalid file format 2\n" << tokens.size();
                    return;
                }

                vector<int> face;
                for (int i = 1; i < tokens.size(); ++i)
                {
                    face.push_back(atoi(tokens[i].c_str()) - 1);
                }

                faces.push_back(face);
            }

            else if(tokens[0][0] == '#') {
                //comment line, ignore
            }
            else {
                QTextStream(stderr) << "ERROR: Invalid file format 3\n";
                return;
            }
        }
    }
    fin.close();
}

int findNormalAxis(Point3D p[4])
{
    if(p[0].x==p[1].x && p[1].x==p[2].x && p[2].x==p[3].x)return 1;
    if(p[0].y==p[1].y && p[1].y==p[2].y && p[2].y==p[3].y)return 2;
    return 3;
}

set<Point3D> VoxelSet::createPointSet() {
    set<pair<Point3D, int> > faceCenters;
    for(vector<vector<int> >::iterator it = faces.begin();it!=faces.end();++it)
    {
        vector<int> face = *it;
        Point3D corner[4];
        for(int i=0;i<4;++i)
            corner[i] = vertices[face[i]];

        //Find center of face
        double x=0, y=0, z=0;
        for(int i=0;i<4;++i)
        {
            x += corner[i].x;
            y += corner[i].y;
            z += corner[i].z;
        }
        x/=4.0;
        y/=4.0;
        z/=4.0;

        Point3D center(x,y,z);
        int normalAxis = findNormalAxis(corner);
        faceCenters.insert(make_pair(center, normalAxis));
    }

    map<Point3D, int> mp;//map to store number of times a center is seen
    for(set<pair<Point3D, int> >::iterator it = faceCenters.begin();it!=faceCenters.end();++it)
    {
        Point3D voxel = it->first;
        int normalAxis = it->second;
        if(normalAxis==1)
        {
            voxel.x+=0.5;
            //quantize voxel
            if(mp.count(voxel)==0)mp[voxel]=0;
            mp[voxel]++;

            voxel.x-=1;
            //quantize voxel
            if(mp.count(voxel)==0)mp[voxel]=0;
            mp[voxel]++;
        }
        else if(normalAxis==2)
        {
            voxel.y+=0.5;
            //quantize voxel
            if(mp.count(voxel)==0)mp[voxel]=0;
            mp[voxel]++;

            voxel.y-=1;
            //quantize voxel
            if(mp.count(voxel)==0)mp[voxel]=0;
            mp[voxel]++;
        }
        else
        {
            voxel.z+=0.5;
            //quantize voxel
            if(mp.count(voxel)==0)mp[voxel]=0;
            mp[voxel]++;

            voxel.z-=1;
            //quantize voxel
            if(mp.count(voxel)==0)mp[voxel]=0;
            mp[voxel]++;
        }
    }

    set<Point3D> voxels;
    for(map<Point3D, int>::iterator it = mp.begin();it!=mp.end();++it)
    {
        if(it->second == 6)//this voxel came from 6 different faces
            voxels.insert(it->first);
    }

    return voxels;
}

void VoxelSet::save(const char* filename) {

    ofstream fout;
    fout.open (filename, ios::out);
    for(int i=0; i<vertices.size(); i++)
    {
        fout << "v " << vertices[i].x << " " << vertices[i].y<< " " << vertices[i].z<<endl;
    }

    for(int i=0; i<faces.size(); i++) {
        fout << "f ";
        for (int j = 0; j < faces[i].size(); ++j) {
            fout << faces[i][j]+1 << " ";
        }
        fout << endl;
    }
    fout.close();
}

void VoxelSet::setScale() {
    double xmax, ymax, zmax;
    double xmin, ymin, zmin;
    xmax = ymax = zmax = INT_MIN*1.0;
    xmin = ymin = zmin = INT_MAX*1.0;
    for (int i = 0; i < vertices.size(); ++i) {
        if(xmax < vertices[i].x)
            xmax = vertices[i].x;
        if(ymax < vertices[i].y)
            ymax = vertices[i].y;
        if(zmax < vertices[i].z)
            zmax = vertices[i].z;

        if(xmin > vertices[i].x)
            xmin = vertices[i].x;
        if(ymin > vertices[i].y)
            ymin = vertices[i].y;
        if(zmin > vertices[i].z)
            zmin = vertices[i].z;
    }
    scalex = -1;
    if(scalex < xmax - xmin)
        scalex = xmax-xmin;
    if(scalex < ymax - ymin)
        scalex = ymax-ymin;
    if(scalex < zmax - zmin)
        scalex = zmax-zmin;

    if(scalex <= 0)
        scalex = 1;
    scaley = scalez = scalex;

    center.x = (xmax+xmin)/2.0;
    center.y = (ymax+ymin)/2.0;
    center.z = (zmax+zmin)/2.0;

//    QTextStream(stderr) << xmin << " " << ymin << " " << zmin;
//    QTextStream(stderr) << xmax << " " << ymax << " " << zmax;

}
