#ifndef OBJREADER_H
#define OBJREADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Triangle.h"
using namespace std;


//NOTE: this uses code recycled from a previous course, which is why some of the naming conventions are different
class ObjReader{

	private:
		vector<Triangle> tri_vector;
		vector<Point> p_vector;

	public:

		void read_file(const char *, const RGBColor);
		void read_file(const char *, const RGBColor, vector<Shape *> &, float, float, float);

};

#endif