#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include <vector>

#include <iostream>
#include <fstream>
#include <string>

#include <algorithm>

#include <ctime>

#include <random>
#include <chrono>

#include "EasyBMP.h"

#include "RGBColor.h"
#include "Point.h"
#include "Vect.h"
#include "ShadeRec.h"
#include "Ray.h"

#include "Shape.h"
#include "Disk.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Cylinder.h"

#include "ObjReader.h"

//#include "UniformGrid.h"
#include "UniGrid.h"

#include "BRDF.h"

using namespace std;

float kEpsilon = 0.0;

int hRES = 400;//40;//200;//1366;
int vRES = 224;//22;//112;//768;

int screenWidth = 200;//1366;//40;//200;
int screenHeight = 112;//768;//22;//112;

int multijitter = 4;

RGBColor worldColor = RGBColor(0.0, 0.0, 0.0);


int distance_compare(vector<float> distances){

	int closest_index = -1;

	for(int i = 0; i < distances.size(); i++){
		if (closest_index == -1){
			if (distances.at(i) != -1.0){
				closest_index = i;
			}
		}
		else if (distances.at(i) != -1.0){
			if (distances.at(i) < distances.at(closest_index)){
				closest_index = i;
			}
		}
	}

	return closest_index;
}

Point ** multi_jittered_grid(){

	int subdivide = multijitter;

	Vect d = Vect(0.0, 0.0, 1.0);
	Ray r = Ray();
	
	Point ** rookgrid = new Point*[subdivide];

	for(int i = 0; i < subdivide; i++){
		rookgrid[i] = new Point[subdivide];
	}	

	for (int a = 0; a < subdivide; a ++){
		for(int b = 0; b < subdivide; b ++){

			rookgrid[a][b] = Point();
			float tempX = ((subdivide-a)*((float)1.0/(subdivide*subdivide)));
			float tempY = ((subdivide-b)*((float)1.0/(subdivide*subdivide)));
			float tempZ = 0.0;

			rookgrid[a][b].setValues(tempX, tempY, tempZ);

		}
	}

	for (int a = 0; a < subdivide; a ++){
		float * xCoord = new float[subdivide];
		for(int b = 0; b < subdivide; b ++){
			xCoord[b] = rookgrid[a][b].getX();
		}

		for(int i = 0; i < subdivide; i ++){
			int first = rand()%subdivide;
			int second = rand()%subdivide;
			float temp = xCoord[first];
			xCoord[first] = xCoord[second];
			xCoord[second] = temp;
		}

		for(int c = 0; c < subdivide; c ++){
			rookgrid[a][c].setX(xCoord[c]);
		}
		delete xCoord;
	}

	for (int a = 0; a < subdivide; a ++){
		float * yCoord = new float[subdivide];
		for(int b = 0; b < subdivide; b ++){
			yCoord[b] = rookgrid[a][b].getY();
		}

		for(int i = 0; i < subdivide; i ++){
			int first = rand()%subdivide;
			int second = rand()%subdivide;
			float temp = yCoord[first];
			yCoord[first] = yCoord[second];
			yCoord[second] = temp;
		}

		for(int c = 0; c < subdivide; c ++){
			rookgrid[a][c].setY(yCoord[c]);
		}
		delete yCoord;
	}

	return rookgrid;


}



void drawScreen(RGBColor ** colorArray, const char * filename){

BMP* out = new BMP;

out -> SetSize(hRES, vRES);

for(int i = 0; i < hRES; i++){
	for(int j = 0; j < vRES; j++){
		RGBColor curr = colorArray[i][j];
		(*out)(i, j) -> Red = curr.getR();
		(*out)(i, j) -> Green = curr.getG();
		(*out)(i, j) -> Blue = curr.getB();
	}
}

out->WriteToFile(filename);

delete out;
}

RGBColor trace_ray(vector<Shape *> shapes, Ray r, int maxRecurseDepth, int pathsPerPixel);

RGBColor MonteCarlo(vector<Shape *> shapes, Point p, Vect n, int maxRecurseDepth, int pathsPerPixel, int shapeIndex, Ray inRay){

	RGBColor outputColor = RGBColor(0.0, 0.0, 0.0);

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator (seed);
	std::normal_distribution<double> distribution (0.0,1.0);

	for (int i = 0; i < pathsPerPixel; i++){

		Vect randVect;

		float x = (float) distribution(generator);
		float y = (float) distribution(generator);
		float z = (float) distribution(generator);

		randVect = Vect(x, y, z);

		if(randVect.dot_product(n) < 0){
			randVect = Vect(-x, -y, -z);
		}

		randVect.normalize();

		float randCosine = randVect.dot_product(n);

		Ray randRay = Ray(p, randVect);

		Shape * shape = shapes.at(shapeIndex);

		//NOTE: CHANCE OF RAY COLLIDING WITH THE SHAPE ITS SUPPOSED TO START AT THE SURFACE OF.
		//Note entirely sure if BRDF implementation should be here or in trace_ray.
		outputColor = outputColor + ((trace_ray(shapes, randRay, maxRecurseDepth, pathsPerPixel)) * (shape->calcBRDF(inRay, randRay, n)) * randCosine);
		
	}

	//should this be commented out or not? Pretty sure it should be.
	//outputColor = outputColor/((float)pathsPerPixel);

	//need to implement BRDF instead of this.

	return outputColor;
}


RGBColor trace_ray(vector<Shape *> shapes, Ray r, int maxRecurseDepth, int pathsPerPixel){

	vector<float> distances;
	vector<ShadeRec *> shaderecs;

	RGBColor outputColor;

	for(int index = 0; index < shapes.size(); index ++){
		shaderecs.push_back(new ShadeRec());
		Ray copied_ray(r);
		float shape_t = shapes.at(index)->ray_intersection(copied_ray, kEpsilon, *(shaderecs.at(index)));
		distances.push_back(shape_t);
	}
			
	int closest_index = distance_compare(distances);

	RGBColor surfaceColor;
	if(closest_index == -1){
		surfaceColor = worldColor;
	}
	else{
		ShadeRec sr = *shaderecs.at(closest_index);
		surfaceColor = shapes.at(closest_index)->getColor();
	}
	//Problem?
	if(maxRecurseDepth <= 0 || closest_index == -1){
		outputColor = surfaceColor;
	}
	else{
		ShadeRec sr = *shaderecs.at(closest_index);
		RGBColor reflectedColor = MonteCarlo(shapes, sr.local_hit_point, sr.normal, maxRecurseDepth-1, pathsPerPixel, closest_index, r);
		
		//outputColor = (surfaceColor * ((float)0.8)) + (reflectedColor * ((float)0.2));

		outputColor = surfaceColor + reflectedColor;
	}

	for(int index = 0; index < shaderecs.size(); index ++){
		delete shaderecs.at(index);
	}

	shaderecs.clear();
	distances.clear();
		
	return outputColor;
}


void projection(vector<Shape *> shapes, Vect light, bool perspective, bool jittered, const char * filename){

	float zero = 0.0;

	RGBColor ** window;

	window = new RGBColor*[hRES];
	for(int i = 0; i < hRES; i++){
		window[i] = new RGBColor[vRES];
	}	

	for(int j = 0; j < hRES; j++){
		for(int k = 0; k < vRES; k++){

			//printf("attempt for (%d, %d)\n", j, k);

			window[j][k] = RGBColor(0.0, 0.0, 0.0);
			
			
			int jitterCount = 1;
			Point ** rook_grid;

			if(jittered){
				jitterCount = multijitter;
				rook_grid = multi_jittered_grid();
			}


			for(int a = 0; a < jitterCount; a++){
				for(int b = 0; b < jitterCount; b++){						

					Ray r = Ray();
					Point ray_O = Point();
					Vect ray_D = Vect();

					if(perspective){
						float ray_OX = (float)(screenWidth/(float)2.0);
						float ray_OY = (float)(screenHeight/(float)2.0);
						float ray_OZ = -100.0;
						ray_O = Point(ray_OX, ray_OY, ray_OZ);

						Point p = Point();
						if(jittered){
							p.setX((float)j + (a*((float)1.0/multijitter))+rook_grid[a][b].getX());
							p.setY((float)k + (b*((float)1.0/multijitter))+rook_grid[a][b].getY());
							p.setZ(0.0);
						}
						else{
							p.setX((float)j);
							p.setY((float)k);
							p.setZ(0.0);
						}
						p.setX(p.getX() * ((float)screenWidth/(float)hRES));
						p.setY(p.getY() * ((float)screenHeight/(float)vRES));

						ray_D = Vect(p - r.getO());
						ray_D.normalize();

						r.setO(ray_O);
						r.setD(ray_D);

					}
					else{

						ray_D = Vect(0.0, 0.0, 1.0);

						float ray_OX;
						float ray_OY;
						float ray_OZ;

						if(jittered){
							ray_OX = (float)j + (a*((float)1.0/multijitter))+rook_grid[a][b].getX();
							ray_OY = (float)k + (b*((float)1.0/multijitter))+rook_grid[a][b].getY();
							ray_OZ = 0.0;
						}
						else{
							ray_OX = (float)j;
							ray_OY = (float)k;
							ray_OZ = 0.0;
						}

						ray_OX = ray_OX * ((float)screenWidth/(float)hRES);
						ray_OY = ray_OY * ((float)screenHeight/(float)vRES);

						ray_O = Point(ray_OX, ray_OY, ray_OZ);

						r.setO(ray_O);
						r.setD(ray_D);
	
					}

					vector<float> distances;
					vector<ShadeRec *> shaderecs;

					for(int index = 0; index < shapes.size(); index ++){
						shaderecs.push_back(new ShadeRec());
						Ray copied_ray(r);
						float shape_t = shapes.at(index)->ray_intersection(copied_ray, kEpsilon, *(shaderecs.at(index)));
						distances.push_back(shape_t);
					}
			
					int closest_index = distance_compare(distances);

					if(closest_index == -1){
						printf("attempt for (%d, %d)\n", j, k);
						printf("WHIF!\n");
						window[j][k] = window[j][k] + RGBColor(255.0, 255.0, 255.0);
					}
					else{
						printf("attempt for (%d, %d)\n", j, k);
						printf("HIT!\n");

						ShadeRec sr = *shaderecs.at(closest_index);
						
						RGBColor closest_color = shapes.at(closest_index)->getColor();

						window[j][k] = window[j][k] + (closest_color * max(zero, light.dot_product(sr.normal)));
					}

					
					for(int index = 0; index < shaderecs.size(); index ++){
						delete shaderecs.at(index);
					}

					shaderecs.clear();
					distances.clear();

					//delete distances;
					}
			}

					window[j][k] = window[j][k] / (float)(jitterCount*jitterCount);

					
					if(jittered){
						for(int i = 0; i < multijitter; i++){
							delete rook_grid[i];
						}	
						delete rook_grid;
					}
					
			}
			
		}

	drawScreen(window, filename);

	/*
	for(int i = 0; i < hRES; i++){
		delete window[i];
	}
	delete window;
	*/
}


void uniform_grid_projection(UniGrid uniGrid, Vect light, bool perspective, bool jittered, const char * filename){
	float zero = 0.0;

	RGBColor ** window;

	window = new RGBColor*[hRES];
	for(int i = 0; i < hRES; i++){
		window[i] = new RGBColor[vRES];
	}	

	for(int j = 0; j < hRES; j++){
		for(int k = 0; k < vRES; k++){

			//printf("attempt for (%d, %d)\n", j, k);

			window[j][k] = RGBColor(0.0, 0.0, 0.0);
			
			
			int jitterCount = 1;
			Point ** rook_grid;

			if(jittered){
				jitterCount = multijitter;
				rook_grid = multi_jittered_grid();
			}


			for(int a = 0; a < jitterCount; a++){
				for(int b = 0; b < jitterCount; b++){						

					Ray r = Ray();
					Point ray_O = Point();
					Vect ray_D = Vect();

					if(perspective){
						float ray_OX = (float)(screenWidth/(float)2.0);
						float ray_OY = (float)(screenHeight/(float)2.0);
						float ray_OZ = -100.0;
						ray_O = Point(ray_OX, ray_OY, ray_OZ);

						Point p = Point();
						if(jittered){
							p.setX((float)j + (a*((float)1.0/multijitter))+rook_grid[a][b].getX());
							p.setY((float)k + (b*((float)1.0/multijitter))+rook_grid[a][b].getY());
							p.setZ(0.0);
						}
						else{
							p.setX((float)j);
							p.setY((float)k);
							p.setZ(0.0);
						}
						p.setX(p.getX() * ((float)screenWidth/(float)hRES));
						p.setY(p.getY() * ((float)screenHeight/(float)vRES));

						ray_D = Vect(p - r.getO());
						ray_D.normalize();

						r.setO(ray_O);
						r.setD(ray_D);

					}
					else{

						ray_D = Vect(0.0, 0.0, 1.0);

						float ray_OX;
						float ray_OY;
						float ray_OZ;

						if(jittered){
							ray_OX = (float)j + (a*((float)1.0/multijitter))+rook_grid[a][b].getX();
							ray_OY = (float)k + (b*((float)1.0/multijitter))+rook_grid[a][b].getY();
							ray_OZ = -100.0;
						}
						else{
							ray_OX = (float)j;
							ray_OY = (float)k;
							ray_OZ = -100.0;
						}

						ray_OX = ray_OX * ((float)screenWidth/(float)hRES);
						ray_OY = ray_OY * ((float)screenHeight/(float)vRES);

						ray_O = Point(ray_OX, ray_OY, ray_OZ);

						r.setO(ray_O);
						r.setD(ray_D);
	
					}


					bool printflag = true;
					if(j == 1 && k == 1){
						printf("BREAKPOINT!!\n");
						printflag = true;
					}

					//NOTE: MAKE SURE YOU INTERSECT A SHAPE WITH THE PROPER BOX

					vector<float> distances;
					vector<ShadeRec *> shaderecs;
					vector<GridBox> gridBoxes = uniGrid.ray_traversal(r);

					vector<Shape *> final_shapes;

					//printf("shapes_lists.size(): %d\n", shapes_lists.size());

					
					for(int listIndex = 0; listIndex < gridBoxes.size(); listIndex++){
						GridBox currGridBox = gridBoxes.at(listIndex);
						//vector<Shape *> shapes = shapes_lists.at(listIndex);
						//BoundingBox box = box_list.at(listIndex);

						for(int index = 0; index < currGridBox.size(); index ++){
							
							ShadeRec * currShadeRec = new ShadeRec();

							float shape_t = currGridBox.at(index)->ray_intersection(r, kEpsilon, *currShadeRec);
							
							
							if(shape_t == -1.0){
								delete currShadeRec;
							}							
							//supposed to make sure that intersections only apply to the box they happen in
							else if(currGridBox.getBox().point_intersect(r.calculatePoint(shape_t))) {
								//printf("point_interesect if statement thing succeeded!\n");
								//The white points are happening because this is doing the OPPOSITE of what i want it to do. It is causing the whiff apparently.
								final_shapes.push_back(currGridBox.at(index));
								distances.push_back(shape_t);
								shaderecs.push_back(currShadeRec);
							}
							else{
								//printf("point_interesect if statement thing FAILED!\n");
								delete currShadeRec;
							}
						}
					}


					int closest_index = distance_compare(distances);

					if(closest_index == -1){
						if(printflag){
							printf("attempt for (%d, %d)\n", j, k);
							printf("WHIF!\n");
						}
						window[j][k] = window[j][k] + RGBColor(255.0, 255.0, 255.0);
					}
					else{
						if(printflag){
							printf("attempt for (%d, %d)\n", j, k);
							printf("HIT!\n");
						}
						ShadeRec sr = *shaderecs.at(closest_index);
						
						RGBColor closest_color = final_shapes.at(closest_index)->getColor();

						window[j][k] = window[j][k] + (closest_color * max(zero, light.dot_product(sr.normal)));
					}

					
					for(int index = 0; index < shaderecs.size(); index ++){
						delete shaderecs.at(index);
					}

					shaderecs.clear();
					distances.clear();

					//delete distances;
					}
			}







					window[j][k] = window[j][k] / (float)(jitterCount*jitterCount);

					
					if(jittered){
						for(int i = 0; i < multijitter; i++){
							delete rook_grid[i];
						}	
						delete rook_grid;
					}
					
			}
			
		}

	drawScreen(window, filename);

	/*
	for(int i = 0; i < hRES; i++){
		delete window[i];
	}
	delete window;
	*/
}


void path_tracing_projection(vector<Shape *> shapes, bool perspective, bool jittered, const char * filename, int maxRecurseDepth, int pathsPerPixel){

	float zero = 0.0;

	RGBColor ** window;

	window = new RGBColor*[hRES];
	for(int i = 0; i < hRES; i++){
		window[i] = new RGBColor[vRES];
	}	

	for(int j = 0; j < hRES; j++){
		for(int k = 0; k < vRES; k++){

			window[j][k] = RGBColor(0.0, 0.0, 0.0);
			
			
			int jitterCount = 1;
			Point ** rook_grid;

			if(jittered){
				jitterCount = multijitter;
				rook_grid = multi_jittered_grid();
			}


			for(int a = 0; a < jitterCount; a++){
				for(int b = 0; b < jitterCount; b++){						

					Ray r = Ray();
					Point ray_O = Point();
					Vect ray_D = Vect();

					if(perspective){
						float ray_OX = (float)(screenWidth/(float)2.0);
						float ray_OY = (float)(screenHeight/(float)2.0);
						float ray_OZ = -100.0;
						ray_O = Point(ray_OX, ray_OY, ray_OZ);

						Point p = Point();
						if(jittered){
							p.setX((float)j + (a*((float)1.0/multijitter))+rook_grid[a][b].getX());
							p.setY((float)k + (b*((float)1.0/multijitter))+rook_grid[a][b].getY());
							p.setZ(0.0);
						}
						else{
							p.setX((float)j);
							p.setY((float)k);
							p.setZ(0.0);
						}
						p.setX(p.getX() * ((float)screenWidth/(float)hRES));
						p.setY(p.getY() * ((float)screenHeight/(float)vRES));

						ray_D = Vect(p - r.getO());
						ray_D.normalize();

						r.setO(ray_O);
						r.setD(ray_D);

					}
					else{

						ray_D = Vect(0.0, 0.0, 1.0);

						float ray_OX;
						float ray_OY;
						float ray_OZ;

						if(jittered){
							ray_OX = (float)j + (a*((float)1.0/multijitter))+rook_grid[a][b].getX();
							ray_OY = (float)k + (b*((float)1.0/multijitter))+rook_grid[a][b].getY();
							ray_OZ = 0.0;
						}
						else{
							ray_OX = (float)j;
							ray_OY = (float)k;
							ray_OZ = 0.0;
						}

						ray_OX = ray_OX * ((float)screenWidth/(float)hRES);
						ray_OY = ray_OY * ((float)screenHeight/(float)vRES);

						ray_O = Point(ray_OX, ray_OY, ray_OZ);

						
						r.setO(ray_O);
						r.setD(ray_D);
	
					}

					//This is the new function. It takes a ray and does the tracing stuff, and returns a color.
					window[j][k] = trace_ray(shapes, r, maxRecurseDepth, pathsPerPixel);

					/*
					if(window[j][k].getR() > 255.0 || window[j][k].getG() > 255.0 || window[j][k].getB() > 255.0){
						printf("overflow of color at %d, %d\n", j, k);
						window[j][k].printFloats();
					}
					*/

					if(window[j][k].getR() > 255.0){
						window[j][k].setR(255.0);
					}
					if(window[j][k].getG() > 255.0){
						window[j][k].setG(255.0);
					}
					if(window[j][k].getB() > 255.0){
						window[j][k].setB(255.0);
					}
					//printf("made it out of the maze! %d, %d\n", j, k);
					//window[j][k].printFloats();
					
					}
			}

					window[j][k] = window[j][k] / (float)(jitterCount*jitterCount);

					
					if(jittered){
						for(int i = 0; i < multijitter; i++){
							delete rook_grid[i];
						}	
						delete rook_grid;
					}
					
			}
			
		}

	drawScreen(window, filename);

	/*
	for(int i = 0; i < hRES; i++){
		delete window[i];
	}
	delete window;
	*/
}




void mp1_projection(){
	vector<Shape *> shapes;

	RGBColor s_color(255.0, 0.0, 0.0);
	Point s_center(10.0, 10.0, 100.0);
	Shape * s = new Sphere(50.0, s_center, s_color); 

	RGBColor pl_color(0.0, 0.0, 255.0);
	Vect pl_n(0.0, -1.0, 0.0);
	Point pl_p(100.0, 100.0, 100.0);
	Shape * pl = new Plane(pl_n, pl_p, pl_color);

	RGBColor tri_color(0.0, 255.0, 0.0);
	Point tri_p1(10.0, 20.0, 30.0);
	Point tri_p2(20.0, 20.0, 30.0);
	Point tri_p3(0.0, 10.0, 30.0);
	Shape * tri = new Triangle(tri_p1, tri_p2, tri_p3, tri_color);

	RGBColor dsk_color = RGBColor(255.0, 255.0, 0.0);
	Vect dsk_n(-1.0, 0.0, -1.0);
	dsk_n.normalize();
	Point dsk_p(70.0, 20.0, 10.0);
	Shape * dsk = new Disk(dsk_n, dsk_p, 20.0, dsk_color);

	RGBColor cylinder_color = RGBColor(0.0, 255.0, 255.0);
	Point cyl_cap1(20.0, 70.0, 10.0);
	Point cyl_cap2(60.0, 70.0, 10.0);
	Shape * cyl = new Cylinder(cyl_cap1, cyl_cap2, 15.0, cylinder_color);

	Vect light = Vect(-1.0, -1.0, -1.0);
	light.normalize();

	shapes.push_back(s);
	shapes.push_back(pl);
	shapes.push_back(tri);
	shapes.push_back(dsk);
	shapes.push_back(cyl);

	projection(shapes, light, false, false, "new_orthographic_2.bmp");
	//projection(shapes, light, false, true, "new_orthographicjittered.bmp");
	//projection(shapes, light, true, false, "new_perspective.bmp");
	//projection(shapes, light, true, true, "new_prespectivejittered.bmp");
}

void mp2_mesh_projection(){

	ObjReader objreader;

	RGBColor tri_color(0.0, 255.0, 0.0);

	vector<Shape *> shapes;

	objreader.read_file("teapot_0.obj", tri_color, shapes, 20.0, 10.0, 20.0);

	Vect light = Vect(-1.0, -1.0, -1.0);
	light.normalize();

	printf("shapes size after: %d\n", shapes.size());

	projection(shapes, light, false, false, "teapot_projection.bmp");


}

void mp2_triangle_grid_projection(){

	ObjReader objreader;

	RGBColor tri_color(0.0, 255.0, 0.0);

	vector<Shape *> shapes;

	objreader.read_file("teapot_0.obj", tri_color, shapes, 20.0, 10.0, 20.0);

	//UniGrid uniGrid(shapes);

	Vect light = Vect(-1.0, -1.0, -1.0);
	light.normalize();

	printf("shapes size after: %d\n", shapes.size());

	//uniform_grid_projection(uniGrid, light, false, false, "teapot_persp_projection_uniform_grid_small.bmp");

	projection(shapes, light, false, false, "teapot_projection_uniform_small.bmp");
}

//creates a bunch of spheres that move diagonally away from the view plane.
//assumes that screen width and height are at least 5.0
vector<Shape *> generate_n_spheres(int n){

	vector <Shape *> sphere_list;
	float xEdge = screenWidth - 5.0;
	float yEdge = screenHeight - 5.0;
	float xMid = (screenWidth/(float)2.0);
	float yMid = (screenHeight/(float)2.0);

	for(int i = 0, j = 0; i < n; i++, ((j++)%8)){

		if(j == 0){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(5.0, 5.0, ((float)2.0 * (float) n));
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}
		else if(j == 1){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(5.0, yEdge, ((float)2.0 * (float) n));
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}
		else if(j == 2){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(xEdge, 5.0, ((float)2.0 * (float) n));
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}
		else if(j == 3){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(xEdge, yEdge, ((float)2.0 * (float) n));
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}
		else if(j == 4){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(5.0, yMid, ((float)2.0 * (float) n));
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}
		else if(j == 5){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(xMid, 5.0, ((float)2.0 * (float) n));
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}
		else if(j == 6){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(xEdge, yMid, ((float)2.0 * (float) n));
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}
		else if(j == 7){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(xMid, yEdge, ((float)2.0 * (float) n));
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}

	}

	return sphere_list;
}

vector<Shape *> generate_n_spheres_the_sequel(int n){
	vector <Shape *> sphere_list;
	float xEdge = screenWidth - 5.0;
	float yEdge = screenHeight - 5.0;
	float xMid = (screenWidth/(float)2.0);
	float yMid = (screenHeight/(float)2.0);

	for(int i = 0, j = 0; i < n; i++, ((j++)%8)){

		if(j == 0){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(5.0, 5.0, 20.0);
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}
		else if(j == 1){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(5.0, yEdge, 20.0);
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}
		else if(j == 2){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(xEdge, 5.0, 20.0);
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}
		else if(j == 3){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(xEdge, yEdge, 20.0);
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}
		else if(j == 4){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(5.0, yMid, 20.0);
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}
		else if(j == 5){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(xMid, 5.0, 20.0);
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}
		else if(j == 6){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(xEdge, yMid, 20.0);
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}
		else if(j == 7){
			RGBColor s_color(255.0, 0.0, 0.0);
			Point s_center(xMid, yEdge, 20.0);
			sphere_list.push_back(new Sphere(1.0, s_center, s_color)); 
		}

	}

	return sphere_list;
}

vector<Shape *> generate_screen_of_spheres(){
	vector <Shape *> sphere_list;
	float xEdge = screenWidth - 5.0;
	float yEdge = screenHeight - 5.0;

	for(int x = 5.0; x < xEdge; x+=3.0){
		for(int y = 5.0; y < yEdge; y+=3.0){
			RGBColor s_color(0.0, 0.0, 255.0);
			Point s_center(x, y, 20.0);
			sphere_list.push_back(new Sphere(1.0, s_center, s_color));
		}
	}



	return sphere_list;
}

void mp2_many_spheres_projection(int n){

	vector<Shape *> shapes = generate_n_spheres_the_sequel(n);

	Vect light = Vect(-1.0, -1.0, -1.0);
	light.normalize();

	UniGrid uniGrid(shapes);

	printf("shapes size after: %d\n", shapes.size());

	uniform_grid_projection(uniGrid, light, false, false, "test_many_sphere_persp_grid_small_1000.bmp");
}

void mp2_screen_of_spheres_projection(){

	vector<Shape *> shapes = generate_screen_of_spheres();

	Vect light = Vect(-1.0, -1.0, -1.0);
	light.normalize();

	UniGrid uniGrid(shapes);

	printf("shapes size after: %d\n", shapes.size());

	uniform_grid_projection(uniGrid, light, false, false, "screen_of_spheres.bmp");
}

void test_sphere_grid(){

	vector<Shape *> shapes;

	RGBColor s1_color(255.0, 0.0, 0.0);
	Point s1_center(10.0, 10.0, 20.0);
	Shape * s1 = new Sphere(25.0, s1_center, s1_color); 

	RGBColor s2_color(0.0, 255.0, 0.0);
	Point s2_center(50.0, 50.0, 20.0);
	Shape * s2 = new Sphere(25.0, s2_center, s2_color); 

	RGBColor s3_color(0.0, 0.0, 255.0);
	Point s3_center(90.0, 90.0, 20.0);
	Shape * s3 = new Sphere(25.0, s3_center, s3_color); 

	Vect light = Vect(-1.0, -1.0, -1.0);
	light.normalize();

	shapes.push_back(s1);
	shapes.push_back(s2);
	shapes.push_back(s3);

	UniGrid uniGrid(shapes);

	printf("shapes size after: %d\n", shapes.size());

	uniform_grid_projection(uniGrid, light, true, false, "test_sphere_persp_grid.bmp");

	//uniform_grid_projection(uniGrid, light, false, false, "test_sphere_ortho_grid.bmp");

	//projection(shapes, light, true, false, "test_sphere_persp_no_grid.bmp");

}

void test_triangle_grid(){
	vector<Shape *> shapes;

	//I THINK THESE COULD BE WHIFFING PARTIALLY BECAUSE THE EYE POINT JUST SIMPLY CAN'T SEE THEM? WORTH INVESTIGATING AFTER POOL TIMES

	RGBColor t1_color(255.0, 0.0, 0.0);
	Point t1_p1(10.0, 10.0, 20.0);
	Point t1_p2(50.0, 10.0, 20.0);
	Point t1_p3(10.0, 50.0, 20.0);
	Shape * t1 = new Triangle(t1_p1, t1_p2, t1_p3, t1_color); 

	RGBColor t2_color(0.0, 255.0, 0.0);
	Point t2_p1(60.0, 60.0, 20.0);
	Point t2_p2(100.0, 60.0, 20.0);
	Point t2_p3(60.0, 100.0, 20.0);
	Shape * t2 = new Triangle(t2_p1, t2_p2, t2_p3, t2_color); 

	RGBColor t3_color(0.0, 0.0, 255.0);
	Point t3_p1(110.0, 110.0, 20.0);
	Point t3_p2(150.0, 110.0, 20);
	Point t3_p3(110.0, 150.0, 20.0);
	Shape * t3 = new Triangle(t3_p1, t3_p2, t3_p3, t3_color); 

	Vect light = Vect(-1.0, -1.0, -1.0);
	light.normalize();

	shapes.push_back(t1);
	shapes.push_back(t2);
	shapes.push_back(t3);

	UniGrid uniGrid(shapes);

	printf("shapes size after: %d\n", shapes.size());

	uniform_grid_projection(uniGrid, light, false, false, "test_triangle_ortho_grid_eh.bmp");

	//uniform_grid_projection(uniGrid, light, false, false, "test_triangle_ortho_grid.bmp");

	//projection(shapes, light, true, false, "test_triangle_persp_no_grid.bmp");
}


void mp3_two_different_spheres(){
		vector<Shape *> shapes;

	RGBColor s1_color(255.0, 0.0, 0.0);
	Point s1_center(49.0, 50.0, 20.0);
	BRDF s1_brdf(false, 0.2);
	Shape * s1 = new Sphere(25.0, s1_center, s1_color, s1_brdf); 

	RGBColor s2_color(0.0, 0.0, 255.0);
	Point s2_center(100.0, 50.0, 20.0);
	BRDF s2_brdf(true, 0.5);
	Shape * s2 = new Sphere(25.0, s2_center, s2_color, s2_brdf); 

	shapes.push_back(s1);
	shapes.push_back(s2);

	path_tracing_projection(shapes, false, false, "path_traced_orthographic_2_8.bmp", 2, 8);
	path_tracing_projection(shapes, false, false, "path_traced_orthographic_4_8.bmp", 4, 8);
	//path_tracing_projection(shapes, false, false, "path_traced_orthographic_6_15.bmp", 6, 8);

}

void mp3_projection(int maxRecurseDepth, int pathsPerPixel){

	vector<Shape *> shapes;

	RGBColor s1_color(255.0, 0.0, 0.0);
	Point s1_center(10.0, 10.0, 20.0);
	BRDF s1_brdf(true, 0.3);
	Shape * s1 = new Sphere(25.0, s1_center, s1_color, s1_brdf); 

	RGBColor s2_color(0.0, 255.0, 0.0);
	Point s2_center(50.0, 50.0, 20.0);
	BRDF s2_brdf(true, 0.4);
	Shape * s2 = new Sphere(25.0, s2_center, s2_color, s2_brdf); 

	RGBColor s3_color(0.0, 0.0, 255.0);
	Point s3_center(90.0, 90.0, 20.0);
	BRDF s3_brdf(true, 0.2);
	Shape * s3 = new Sphere(25.0, s3_center, s3_color, s3_brdf); 

	shapes.push_back(s1);
	shapes.push_back(s2);
	shapes.push_back(s3);

	path_tracing_projection(shapes, false, false, "path_traced_diffuse_orthographic.bmp", maxRecurseDepth, pathsPerPixel);

}

int main(){

	time_t start = 0;
	time_t end = 0;
	time_t elapsed = 0;


	start = time(NULL);
	mp1_projection();

	//mp2_mesh_projection();

	//test_sphere_grid();
	//test_triangle_grid();

	//mp2_triangle_grid_projection();

	//mp2_many_spheres_projection(10000);

	//mp2_screen_of_spheres_projection();

	//mp3_projection(3, 30);

	//mp3_two_different_spheres();

	end = time(NULL);

	elapsed = start - end;

	printf("time elapsed = %d\n", elapsed);
	printf("time elapsed = %f\n", elapsed);
	printf("done!\n");

}


