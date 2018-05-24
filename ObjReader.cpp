#include "ObjReader.h"

//load and parse the object file, assigning all triangles the color given

void ObjReader::read_file(const char * filename, const RGBColor color){

	p_vector.clear();
	tri_vector.clear();

	char type;
	float x, y, z;

	std::ifstream infile(filename);

	while(infile >> type >> x >> y >> z){

		printf("type: %c, X: %f, Y: %f, Z: %f\n", type, x, y, z);

		if(type == 'v'){
			Point p = Point(x, y, z);
			p_vector.push_back(p);
		}
		else if(type == 'f'){

			int a = ((int)x)-1;
			int b = ((int)y)-1;
			int c = ((int)z)-1;

			Point p1 = p_vector.at(a);
			Point p2 = p_vector.at(b);
			Point p3 = p_vector.at(c);

			Triangle tri(p1, p2, p3, color);

			tri_vector.push_back(tri);

		}


	}



}

void ObjReader::read_file(const char * filename, const RGBColor color, vector<Shape *> & shapes, float x_offset, float y_offset, float z_offset){

	char type;
	float x, y, z;

	std::ifstream infile(filename);

	while(infile >> type >> x >> y >> z){

		//printf("type: %c, X: %f, Y: %f, Z: %f\n", type, x, y, z);

		if(type == 'v'){
			Point p = Point(x+x_offset, y+y_offset, z+z_offset);
			p_vector.push_back(p);
		}
		else if(type == 'f'){

			int a = ((int)x)-1;
			int b = ((int)y)-1;
			int c = ((int)z)-1;

			Point p1 = p_vector.at(a);
			Point p2 = p_vector.at(b);
			Point p3 = p_vector.at(c);

			Triangle tri(p1, p2, p3, color);

			Shape * shape = new Triangle(p1, p2, p3, color);

			tri_vector.push_back(tri);

			shapes.push_back(shape);

		}


	}




}