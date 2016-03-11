/*
Assignment 3: Back face culling and projection.
zViewer should be positive and greater than the maximum z coordinate in the objfile 
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <climits>
#include <cmath>
#include <map>
#include <set>

using namespace std;

const double PI = atan(1.0)*4;

struct Point3D
{
	double x,y,z;
	bool operator<(const Point3D &p)const
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
};

struct Point2D
{
	double x,y;
};

struct Normal
{
	double a,b,c;
};

struct LineOfSight
{
	double a,b,c;
};

void printVertices(vector<Point3D> vertices)
{
	for(vector<Point3D>::iterator it = vertices.begin() + 1;it!=vertices.end();++it)
  		cout<<it->x<<" "<<it->y<<" "<<it->z<<endl;
}

void printFaces(vector<vector<int> > faces)
{
	for(vector<vector<int> >::iterator it = faces.begin();it!=faces.end();++it)
  	{
  		for(vector<int>::iterator it2 = it->begin();it2!=it->end();++it2)
  			cout<<*it2<<" ";
  		cout<<endl;
  	}
}

int stringToInt(string s)
{
	return atoi(s.c_str());
}

double stringToDouble(string s)
{
	return atof(s.c_str());
}

vector<string> splitLine(string line)
{
	vector<string> ret;
	stringstream ss(line);// Turn the string into a stream.
	string tok;
	  
	while(ss >> tok)
	{
	  ret.push_back(tok);
	}
	return ret;
}

void readObjfile(string filename, vector<Point3D> &vertices, vector<vector<int> > &faces)
{
	/*
	Extract vertices and faces from objfile.
	Faces can be polygons.
	*/
	Point3D p = {0,0,0};//dummy point for proper indexing
	vertices.push_back(p);

	string line;
  	ifstream objfile(filename.c_str());
  	if(objfile.is_open())
  	{
    	while(getline(objfile,line))
    	{
    	  	// cout << line << '\n';
    		if(line.empty())//blank line
    			continue;
    		if(line[0]=='#')//comment line
    			continue;

    		vector<string> tokens = splitLine(line);
    		if(line[0]=='v')//vertex
    		{
    			if(tokens.size()!=4)
    			{
    				cout<<"Invalid obj file"<<endl;
    				exit(0);
    			}
    			Point3D p = {stringToDouble(tokens[1]), stringToDouble(tokens[2]), stringToDouble(tokens[3])};
    			vertices.push_back(p);
    		}
    		else//face
    		{
    			if(tokens.size()<4)
    			{
    				cout<<"Invalid obj file"<<endl;
    				exit(0);
    			}
    			vector<int> face;
    			vector<string>::iterator it = tokens.begin() + 1;//ignore first token
    			for(;it!=tokens.end();++it)
    				face.push_back(stringToInt(*it));
    			faces.push_back(face);
    		}
    	}
    	objfile.close();
  	}
  	else
  	{
  		cout << "Unable to open objfile"<<endl;
  		exit(0);
  	}

  	// printVertices(vertices);
  	// printFaces(faces);
}

//1 for x, 2 for y, 3 for z
int findNormalAxis(Point3D p[4])
{
	if(p[0].x==p[1].x && p[1].x==p[2].x && p[2].x==p[3].x)return 1;
	if(p[0].y==p[1].y && p[1].y==p[2].y && p[2].y==p[3].y)return 2;
	return 3;
}

set<Point3D> findVoxelsOfObject(vector<Point3D> vertices, vector<vector<int> > faces)
{
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

		Point3D center = {x,y,z};
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

set<Point3D> voxelIntersection(set<Point3D> voxels1, set<Point3D> voxels2)
{
	set<Point3D> voxels;
	set_intersection(voxels1.begin(),voxels1.end(),voxels2.begin(),voxels2.end(),
                  	inserter(voxels,voxels.begin()));
	return voxels;
}

set<Point3D> voxelUnion(set<Point3D> voxels1, set<Point3D> voxels2)
{
	set<Point3D> voxels;
	set_union(voxels1.begin(),voxels1.end(),voxels2.begin(),voxels2.end(),
                  	inserter(voxels,voxels.begin()));
	return voxels;
}

set<Point3D> voxelDifference(set<Point3D> voxels1, set<Point3D> voxels2)
{
	set<Point3D> voxels;
	set_difference(voxels1.begin(),voxels1.end(),voxels2.begin(),voxels2.end(),
                  	inserter(voxels,voxels.begin()));
	return voxels;
}

int numLines=0;
ofstream fout;
//Draws a cube corresponding to a voxel
void select(double x, double y, double z)
{
	// cout<<x<<" "<<y<<" "<<z<<endl;

	fout<<"v "<<x-0.5<<" "<<y-0.5<<" "<<z+0.5<<endl;
	fout<<"v "<<x-0.5<<" "<<y-0.5<<" "<<z-0.5<<endl;
	fout<<"v "<<x-0.5<<" "<<y+0.5<<" "<<z+0.5<<endl;
	fout<<"v "<<x-0.5<<" "<<y+0.5<<" "<<z-0.5<<endl;
	fout<<"v "<<x+0.5<<" "<<y-0.5<<" "<<z+0.5<<endl;
	fout<<"v "<<x+0.5<<" "<<y-0.5<<" "<<z-0.5<<endl;
	fout<<"v "<<x+0.5<<" "<<y+0.5<<" "<<z+0.5<<endl;
	fout<<"v "<<x+0.5<<" "<<y+0.5<<" "<<z-0.5<<endl;

	fout<<"f "<<1+numLines<<" "<<5+numLines<<" "<<7+numLines<<" "<<3+numLines<<endl;
	fout<<"f "<<5+numLines<<" "<<6+numLines<<" "<<8+numLines<<" "<<7+numLines<<endl;
	fout<<"f "<<2+numLines<<" "<<4+numLines<<" "<<8+numLines<<" "<<6+numLines<<endl;
	fout<<"f "<<1+numLines<<" "<<3+numLines<<" "<<4+numLines<<" "<<2+numLines<<endl;
	fout<<"f "<<3+numLines<<" "<<7+numLines<<" "<<8+numLines<<" "<<4+numLines<<endl;
	fout<<"f "<<2+numLines<<" "<<6+numLines<<" "<<5+numLines<<" "<<1+numLines<<endl;
	
	numLines+=8;
}


void writeVoxelsToObjFile(set<Point3D> voxels)
{
	fout.open ("output.obj", std::ofstream::out);
	for(set<Point3D>::iterator it = voxels.begin();it!=voxels.end();++it)
		select(it->x, it->y, it->z);
	fout.close();
}


string filename;
void input()
{
	//Input
	cout<<"Enter objfile name:";cin>>filename;
	cout<<endl;
}

int main()
{
	input();

	vector<Point3D> vertices1, vertices2;
	vector<vector<int> > faces1, faces2;
	readObjfile(filename, vertices1, faces1);
	input();
	readObjfile(filename, vertices2, faces2);

	set<Point3D> voxels1, voxels2, voxels;
	voxels1 = findVoxelsOfObject(vertices1, faces1);
	voxels2 = findVoxelsOfObject(vertices2, faces2);

	voxels = voxelIntersection(voxels1, voxels2);
	writeVoxelsToObjFile(voxels);

	
  	return 0;
}