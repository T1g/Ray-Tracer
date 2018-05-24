#include "Sphere.h"

Sphere::Sphere(){
	
}

Sphere::Sphere(float RR, const Point& PP, const RGBColor& CC){
	r = RR;
	center = PP;
	color = CC;
	createBoundingBox();
}

Sphere::Sphere(float RR, const Point& PP, const RGBColor& CC, const BRDF& BB){
	r = RR;
	center = PP;
	color = CC;
	createBoundingBox();
	brdf = BB;
}

Sphere::~Sphere(){

}


void Sphere::createBoundingBox(){

	//center = (X, Y, Z), radius r
	//box point would then be (X - srt(2)*r, Y..., Z...), lenghts would be 2*r


	float offset = sqrtf(3.0) * r;
	float bp1x = center.getX() - offset;
	float bp1y = center.getY() - offset;
	float bp1z = center.getZ() - offset;
	
	float bp2x = center.getX() + offset;
	float bp2y = center.getY() + offset;
	float bp2z = center.getZ() + offset;


	Point boxP1(bp1x, bp1y, bp1z);
	Point boxP2(bp2x, bp2y, bp2z);


	box = BoundingBox(boxP1, boxP2);

}



float Sphere::ray_intersection(Ray& ray, float kEpsilon, ShadeRec& sr){

	float t;

	Vect temp = ray.getO() - center;


	float a = ray.getD().dot_product(ray.getD());
	float b = ((float)2.0 * temp.dot_product(ray.getD()));
	float c = temp.dot_product(temp) - (r * r);
	float disc = b * b - (((float)4.0) * a * c);

	if (disc < 0.0)
		return -1.0;
	else {
		
		float e = sqrtf(disc);
		float denom = (float)(2.0 * a);
		t = (-b - e) / denom;

		if (t > kEpsilon) {
			sr.normal = (temp + (ray.getD() * t))/r;
			sr.local_hit_point = (ray.getO() + (ray.getD() * t));
			return t;
		}

		t = (-b + e) / denom;

		if (t > kEpsilon) {
			sr.normal = (temp + (ray.getD() * t))/r;
			sr.local_hit_point = (ray.getO() + (ray.getD() * t));
			return t;
		}
	}

	return -1.0;

}

void Sphere::setColor(const RGBColor& CC){
	color = CC;
}

RGBColor Sphere::getColor() const{
	return color;
}

BoundingBox Sphere::getBoundingBox() const{
	return box;
}

BRDF Sphere::getBRDF() const{

	return brdf;
}

float Sphere::calcBRDF(const Ray& inRay, const Ray& outRay, const Vect& n) const{
	return brdf.calculate(inRay, outRay, n);
}