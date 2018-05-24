#include "Plane.h"


Plane::Plane(){
	
}


Plane::Plane(const Vect& NN, const Point& PP, const RGBColor& CC){
	n = Vect(NN);
	p = Point(PP);
	color = RGBColor(CC);

	//createBoundingBox();
}


Plane::~Plane(){

}

//NOTE: DONT ACTUALLY USE BOUNDING BOX STUFF FOR PLANES
void Plane::createBoundingBox(){
	
	Point bP(0.0, 0.0, 0.0);
	box = BoundingBox(bP, 0.0, 0.0, 0.0);
}

float Plane::ray_intersection(Ray & ray, float kEpsilon, ShadeRec& sr){

	Point testP = p - ray.getO();

	float t = (n.dot_product((p - ray.getO()))) / (n.dot_product(ray.getD()));

	if (t > kEpsilon) {
			sr.normal = n;
			sr.local_hit_point = ray.getO() + (ray.getD() * t);
			return t;
	}
	else return -1.0;

}

void Plane::setColor(const RGBColor& CC){
	color = CC;
}

RGBColor Plane::getColor() const{
	return color;
}

BoundingBox Plane::getBoundingBox() const{
	return box;
}

BRDF Plane::getBRDF() const{

	return brdf;
}

float Plane::calcBRDF(const Ray& inRay, const Ray& outRay, const Vect& n) const{
	return brdf.calculate(inRay, outRay, n);
}