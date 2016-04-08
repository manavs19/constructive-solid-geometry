#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <set>

using namespace std;

const int MAGNIFICATION_FACTOR = 1;

void swap(int &a, int &b)
{
	int temp=a;
	a=b;
	b=temp;
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

struct Point2D
{
	int x;
	int y;
};

struct Point3D
{
	int x;
	int y;
	int z;
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

//Ensure ys < ye
vector<Point2D> get2DDSS(int xs, int ys, int xe, int ye)
{
	int i=xs, n=0, d=ye-ys, deltaX=xe-xs;
	if(d<0)d*=-1;
	vector<Point2D> dss;
	for(int y=ys;y<=ye;++y)
	{
		Point2D p = {i, y};
		if(2*n > d)//Right
			p.x++;
		dss.push_back(p);

		n+=deltaX;
		int quotient=0;
		//Avoid division, use repeated subtraction
		if(n>=d)
		{
			while(n>=d)
			{
				quotient++;
				n-=d;
			}
			i+=quotient;
		}
		else if(n<0)
		{
			while(n<0)
			{
				quotient--;
				n+=d;
			}
			i+=quotient;
		}
	}
	return dss;
}

//P1 is lowest(bottom left)
//Finds lowest point in 2D triangle
void findLowestPoint(int &x1, int &y1, int &x2, int &y2, int &x3, int &y3)
{
	if(y2<y1 || (y2==y1 && x2<x1))
	{
		swap(x1,x2);
		swap(y1,y2);
	}
	if(y3<y1 || (y3==y1 && x3<x1))
	{
		swap(x1,x3);
		swap(y1,y3);
	}
}

//P3 is highest(top right)
//Finds highest point in 2D triangle
void findHighestPoint(int &x1, int &y1, int &x2, int &y2, int &x3, int &y3)
{
	if(y1>y3 || (y1==y3 && x1>x3))
	{
		swap(x1,x3);
		swap(y1,y3);
	}
	if(y2>y3 || (y2==y3 && x2>x3))
	{
		swap(x2,x3);
		swap(y2,y3);
	}
}

//Fills 2D triangle by moving along 2D DSS of the edges
//Returns the points
vector<Point2D> fill2DTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	//P1 is lowest, P2 is mid, P3 is top point
	findLowestPoint(x1,y1,x2,y2,x3,y3);
	findHighestPoint(x1,y1,x2,y2,x3,y3);

	vector<Point2D> triangle;
	vector<Point2D> dss1, dss2;
	//Degenerate cases
	if(y1==y2 && y2==y3)//straight line
	{
		for(int x=x1;x<=x3;++x)
		{
			Point2D p = {x,y1};
			triangle.push_back(p);
		}
		return triangle;
	}
	if(y1==y2)//horizontal line at bottom
	{
		dss1 = get2DDSS(x1,y1,x3,y3);
		dss2 = get2DDSS(x2,y2,x3,y3);

		//assert(dss1.size()==dss2.size());

		for(int i=0;i<dss1.size();++i)
		{
			int left = dss1[i].x;
			int right = dss2[i].x;
			int y = dss1[i].y;

			for(int x=left;x<=right;++x)
			{
				Point2D p = {x,y};
				triangle.push_back(p);
			}
		}
		return triangle;
	}
	if(y2==y3)//horizontal line at top
	{
		dss1 = get2DDSS(x1,y1,x2,y2);
		dss2 = get2DDSS(x1,y1,x3,y3);

		//assert(dss1.size()==dss2.size());

		for(int i=0;i<dss1.size();++i)
		{
			int left = dss1[i].x;
			int right = dss2[i].x;
			int y = dss1[i].y;

			for(int x=left;x<=right;++x)
			{
				Point2D p = {x,y};
				triangle.push_back(p);
			}
		}
		return triangle;
	}

	//General case
	//dss1 is left edge
	//dss2 is right edge
	if(x2<=x3)
	{
		dss1 = get2DDSS(x1,y1,x2,y2);//left edge
		dss2 = get2DDSS(x1,y1,x3,y3);//right edge
	}
	else
	{
		dss2 = get2DDSS(x1,y1,x2,y2);//right edge
		dss1 = get2DDSS(x1,y1,x3,y3);//left edge
	}

	int size = min(dss1.size(), dss2.size());
	for(int i=0;i<size;++i)
	{
		int left = dss1[i].x;
		int right = dss2[i].x;
		int y = dss1[i].y;

		for(int x=left;x<=right;++x)
		{
			Point2D p = {x,y};
			triangle.push_back(p);
		}
	}

	if(dss1.size() < dss2.size())
	{
		int save = dss1.size();
		dss1 = get2DDSS(x2,y2,x3,y3);
		//skip first point as that is already covered
		for(int i=1;i<dss1.size();++i)
		{
			int left = dss1[i].x;
			int right = dss2[i+save-1].x;
			int y = dss1[i].y;

			for(int x=left;x<=right;++x)
			{
				Point2D p = {x,y};
				triangle.push_back(p);
			}
		}
	}
	else
	{
		int save = dss2.size();
		dss2 = get2DDSS(x2,y2,x3,y3);
		//skip first point as that is already covered
		for(int i=1;i<dss2.size();++i)
		{
			int left = dss1[i+save-1].x;
			int right = dss2[i].x;
			int y = dss1[i+save-1].y;

			for(int x=left;x<=right;++x)
			{
				Point2D p = {x,y};
				triangle.push_back(p);
			}
		}
	}

	return triangle;
}

bool checkF(int x, int y, int z, int A, int B, int C, int D, int absC)
{
	int temp = 2*(A*x + B*y + C*z + D);
	int left = -absC;
	int right = absC;
	if(left<temp && temp<=right)return true;
	return false;
}

//Assume C is max, so project on xy plane
//coordinates already swapped
//Returns 3D points
vector<Point3D> fill3DTriangle(int x1, int y1, int z1, int x2, int y2, int z2, 
								int x3, int y3, int z3, int A, int B, int C, 
								int D, int absC)
{
	vector<Point3D> ans;
	vector<Point2D> triangle = fill2DTriangle(x1,y1,x2,y2,x3,y3);
	int firstX = triangle[0].x;
	int firstY = triangle[0].y;
	int lastZ = z1;
	if(x2==firstX && y2==firstY)lastZ=z2;
	if(x3==firstX && y3==firstY)lastZ=z3;

	int rowStartZ = lastZ;
	int lastY = firstY;
	for(vector<Point2D>::iterator it = triangle.begin();it!=triangle.end();++it)
	{
		int x = it->x;
		int y = it->y;
		// printf("%d %d\n", it->x, it->y);

		if(y != lastY)//row change
			lastZ = rowStartZ;
		int z;
		if(checkF(x,y,lastZ,A,B,C,D,absC))
			z = lastZ;
		else if(checkF(x,y,lastZ+1,A,B,C,D,absC))
			z = lastZ + 1;
		else
			z = lastZ - 1;
		
		Point3D p = {x,y,z};
		ans.push_back(p);

		if(y != lastY)//row change
			rowStartZ = z;

		lastY = y;
	}
	return ans;
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
  		int lineNum = 0;
    	while(getline(objfile,line))
    	{
    		lineNum++;
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
    				cout<<"Invalid obj file: "<<filename<<" ,lineNum: "<<lineNum<<endl;
    				exit(0);
    			}
    			Point3D p = {MAGNIFICATION_FACTOR*stringToDouble(tokens[1]), MAGNIFICATION_FACTOR*stringToDouble(tokens[2]), MAGNIFICATION_FACTOR*stringToDouble(tokens[3])};
    			vertices.push_back(p);
    		}
    		else if(line[0]=='f')//face
    		{
    			if(tokens.size()<4 || tokens.size()>5)//3 or 4 faces
    			{
    				cout<<"Invalid obj file: "<<filename<<" ,lineNum: "<<lineNum<<endl;
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
  		cout << "Unable to open objfile: "<<filename<<endl;
  		exit(0);
  	}

  	// printVertices(vertices);
  	// printFaces(faces);
}

set<Point3D> handleFace(Point3D p1, Point3D p2, Point3D p3)
{	
	int x1 = p1.x, y1 = p1.y, z1 = p1.z;
	int x2 = p2.x, y2 = p2.y, z2 = p2.z;
	int x3 = p3.x, y3 = p3.y, z3 = p3.z;

	int x21,y21,z21,x31,y31,z31;
	x21=x2-x1;y21=y2-y1;z21=z2-z1;
	x31=x3-x1;y31=y3-y1;z31=z3-z1;

	int A = y21*z31 - y31*z21;int absA = A>=0?A:-A;
	int B = z21*x31 - z31*x21;int absB = B>=0?B:-B;
	int C = x21*y31 - x31*y21;int absC = C>=0?C:-C;

	int D = -(A*x1 + B*y1 + C*z1);
	// printf("%d %d %d %d\n", A,B,C,D);

	set<Point3D> voxels;
	if(absA >= absB && absA >= absC)//absA is max, project on yz plane
	{
		vector<Point3D> triangle = fill3DTriangle(y1,z1,x1,y2,z2,x2,y3,z3,x3,B,C,A,D,absA);
		for(vector<Point3D>::iterator it = triangle.begin();it!=triangle.end();++it)
		{
			Point3D p = {it->z, it->x, it->y};
			voxels.insert(p);
		}
	}
	else if(absB >= absA && absB >= absC)//absB is max, project on xz plane
	{
		vector<Point3D> triangle = fill3DTriangle(x1,z1,y1,x2,z2,y2,x3,z3,y3,A,C,B,D,absB);
		for(vector<Point3D>::iterator it = triangle.begin();it!=triangle.end();++it)
		{
			Point3D p = {it->x, it->z, it->y};
			voxels.insert(p);
		}
	}
	else//absC is max, project on xy plane
	{
		vector<Point3D> triangle = fill3DTriangle(x1,y1,z1,x2,y2,z2,x3,y3,z3,A,B,C,D,absC);
		for(vector<Point3D>::iterator it = triangle.begin();it!=triangle.end();++it)
		{
			Point3D p = {it->x, it->y, it->z};
			voxels.insert(p);
		}
	}
	return voxels;
}

//This the function you need to call to get the voxels from the surface file
set<Point3D> surfaceToVoxels(string filename)
{
	vector<Point3D> vertices;
	vector<vector<int> > faces;
	readObjfile(filename, vertices, faces);

	set<Point3D> voxels;
	for(vector<vector<int> >::iterator it = faces.begin();it!=faces.end();++it)
	{
		vector<int> face = *it;
		if(face.size() == 3)//Triangular surface
		{
			set<Point3D> voxels1 = handleFace(vertices[face[0]], vertices[face[1]], vertices[face[2]]);
			voxels.insert(voxels1.begin(), voxels1.end());
		}
		else//Quadrilateral surface
		{
			set<Point3D> voxels1 = handleFace(vertices[face[0]], vertices[face[1]], vertices[face[2]]);
			set<Point3D> voxels2 = handleFace(vertices[face[2]], vertices[face[3]], vertices[face[0]]);
			voxels.insert(voxels1.begin(), voxels1.end());
			voxels.insert(voxels2.begin(), voxels2.end());
		}			
	}

	// cout<<voxels.size()<<endl;
	return voxels;
}

/*int main()
{
	set<Point3D> voxels = surfaceToVoxels("cube.obj");
	return 0;
}*/