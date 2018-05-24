#include "Shape.h"
/*
Shape::Shape(){

}

Shape::Shape(const RGBColor& CC){
	color = CC;
}


Shape::~Shape(){

}

*/

void Shape::createBoundingBox(){

}

void Shape::setColor(const RGBColor& CC){
	//color = CC;
}

RGBColor Shape::getColor() const{
	//return color;
	return RGBColor(0.0, 0.0, 0.0);
}

BoundingBox Shape::getBoundingBox() const{

	return BoundingBox(Point(0.0, 0.0, 0.0), 0.0, 0.0, 0.0);
}


BRDF Shape::getBRDF() const{

	return BRDF(true, 1.0);
}

float Shape::calcBRDF(const Ray& inRay, const Ray& outRay, const Vect& n) const{
	return 0.0;
}

float Shape::ray_intersection(Ray& ray, float kEpsilon, ShadeRec& sr){

	return -1.0;
}