#include "Cylinder.h"
#include "Disk.h"

Cylinder::Cylinder(){
	
}


Cylinder::Cylinder(const Point& CP1, const Point& CP2, float RR, const RGBColor& CC){
	cap1 = CP1;
	cap2 = CP2;
	radius = RR;
	color = CC;

	n = cap2 - cap1;
	n.normalize();

	//createBoundingBox();
}


Cylinder::~Cylinder(){

}


void Cylinder::createBoundingBox(){
	


}

int Cylinder::distance_compare(float * dist_array, int length){

	int closest_index = -1;

	for(int i = 0; i < length; i++){
		if (closest_index == -1){
			if (dist_array[i] != -1.0){
				closest_index = i;
			}
		}
		else if (dist_array[i] != -1.0){
			if (dist_array[i] < dist_array[closest_index]){
				closest_index = i;
			}
		}
	}

	return closest_index;
}

float Cylinder::ray_intersection(Ray& ray, float kEpsilon, ShadeRec& sr){

	Vect v = ray.getD();
	Vect vA = n;

	Point p = ray.getO();
	Point p1 = cap1;
	Point p2 = cap2;
	Point pA = cap1;
	Point deltaP = p - pA;

	Vect tempA = v - (vA * v.dot_product(vA));

	Point tempB = deltaP - (vA * vA.dot_product(deltaP));

	float A = tempA.dot_product(tempA);
	float B = (float)(2.0 * tempA.dot_product(tempB));
	float C = tempB.dot_product(tempB) - (radius * radius);

	float t1 = (-B + sqrt(B*B - (4*A*C)))/(2*A);
	float t2 = (-B - sqrt(B*B - (4*A*C)))/(2*A);

	Point q1 = p + (v*t1);
	Point q2 = p + (v*t2);

	bool t1check = (vA.dot_product(q1 - p1) > 0) && (vA.dot_product(q1 - p2) < 0) && (t1 >= 0.0);
	bool t2check = (vA.dot_product(q2 - p1) > 0) && (vA.dot_product(q2 - p2) < 0) && (t2 >= 0.0);

	Disk dsk3 = Disk(n, cap1, radius, color);
	Disk dsk4 = Disk(n, cap2, radius, color);

	float t3 = dsk3.ray_intersection(ray, kEpsilon, sr);
	float t4 = dsk4.ray_intersection(ray, kEpsilon, sr);

	Point q3 = p + (v * t3);
	Point q4 = p + (v * t4);

	bool t3check = ((q3 - p1).dot_product(q3 - p1) < (radius * radius)) && (t3 >= 0.0);
	bool t4check = ((q4 - p2).dot_product(q4 - p2) < (radius * radius)) && (t4 >= 0.0);

	float tBest = -1.0;

	float * distances = new float[4];
	if (t1check) distances[0] = t1;
	else distances[0] = -1.0;
	if (t2check) distances[1] = t2;
	else distances[1] = -1.0;
	if (t3check) distances[2] = t3;
	else distances[2] = -1.0;
	if (t4check) distances[3] = t4;
	else distances[3] = -1.0;

	int closest_index = distance_compare(distances, 4);
			
	Point projected = Point();
	float closest_distance = distances[closest_index];
	delete distances;

			switch(closest_index){
			case 0:
				sr.local_hit_point = ray.getO() + (ray.getD() * t1);
				projected = sr.local_hit_point - vA*((sr.local_hit_point - p1).dot_product(vA));
				sr.normal = projected - p1;
				sr.normal.normalize();
				return closest_distance;
				break;
			
			case 1:
				sr.local_hit_point = ray.getO() + (ray.getD() * t2);
				projected = sr.local_hit_point - vA*((sr.local_hit_point - p1).dot_product(vA));
				sr.normal = projected - p1;
				sr.normal.normalize();
				return closest_distance;				
				break;
			
			case 2:
				sr.normal = n;
				sr.local_hit_point = (ray.getO() + (ray.getD() * t3));
				return closest_distance;			
				break;

			case 3:
				sr.normal = n;
				sr.local_hit_point = (ray.getO() + (ray.getD() * t4));
				return closest_distance;				
				break;

			default:
				return -1.0;
			}
			

}


void Cylinder::setColor(const RGBColor& CC){
	color = CC;
}

RGBColor Cylinder::getColor() const{
	return color;
}

BoundingBox Cylinder::getBoundingBox() const{
	return box;
}

BRDF Cylinder::getBRDF() const{

	return brdf;
}

float Cylinder::calcBRDF(const Ray& inRay, const Ray& outRay, const Vect& n) const{
	return brdf.calculate(inRay, outRay, n);
}