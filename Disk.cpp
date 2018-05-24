#include "Disk.h"

Disk::Disk(){
	
}


Disk::Disk(const Vect& NN, const Point& PP, float RR, const RGBColor& CC){
	n = NN;
	p = PP;
	r = RR;
	color = CC;

	//createBoundingBox();
}


Disk::~Disk(){

}

void Disk::createBoundingBox(){



}

float Disk::intersection_helper(Ray& ray, float kEpsilon, ShadeRec& sr){

	float t = n.dot_product(p - ray.getO()) / (n.dot_product(ray.getD()));
	
	if (t > kEpsilon) {
			sr.normal = n;
			sr.local_hit_point = ray.getO() + (ray.getD() * t);
			return t;
	}
	else return -1.0;

}


float Disk::ray_intersection(Ray& ray, float kEpsilon, ShadeRec& sr){

	float t = 0;

	t = intersection_helper(ray, kEpsilon, sr);

	if (t > kEpsilon) { 
		Point temp = (ray.getO() + (ray.getD() * t));
		Point v = temp - p;
		float d2 = v.dot_product(v);
		if(sqrtf(d2) <= r){
			sr.normal = n;
			sr.local_hit_point = (ray.getO() + (ray.getD() * t));
			return t;
		}
		else return -1.0;
	}
	return -1.0;

}


void Disk::setColor(const RGBColor& CC){
	color = CC;
}

RGBColor Disk::getColor() const{
	return color;
}

BoundingBox Disk::getBoundingBox() const{
	return box;
}

BRDF Disk::getBRDF() const{

	return brdf;
}

float Disk::calcBRDF(const Ray& inRay, const Ray& outRay, const Vect& n) const{
	return brdf.calculate(inRay, outRay, n);
}