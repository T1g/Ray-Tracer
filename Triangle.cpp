#include "Triangle.h"



Triangle::Triangle(){
	
}


Triangle::Triangle(const Point& PP1, const Point& PP2, const Point& PP3, const RGBColor& CC){
	p1 = PP1;
	p2 = PP2;
	p3 = PP3;
	color = CC;
	
	createBoundingBox();
}


Triangle::~Triangle(){

}

void Triangle::createBoundingBox(){

	float xMin = min(min(p1.getX(), p2.getX()), p3.getX());
	float yMin = min(min(p1.getY(), p2.getY()), p3.getY());
	float zMin = min(min(p1.getZ(), p2.getZ()), p3.getZ());

	float xMax = max(max(p1.getX(), p2.getX()), p3.getX());
	float yMax = max(max(p1.getY(), p2.getY()), p3.getY());
	float zMax = max(max(p1.getZ(), p2.getZ()), p3.getZ());




	if(xMin == xMax){
		float xOffset = xMax + xMin;
		xMin = xMin - xOffset;
		xMax = xMax + xOffset;
	}
	if(yMin == yMax){
		float yOffset = yMax + yMin;
		yMin = yMin - yOffset;
		yMax = yMax + yOffset;
	}
	if(zMin == zMax){
		float zOffset = zMax + zMin;
		zMin = zMin - zOffset;
		zMax = zMax + zOffset;
	}



	Point pp1(xMin, yMin, zMin);
	Point pp2(xMax, yMax, zMax);
	box = BoundingBox(pp1, pp2);

	//box = BoundingBox(p, (xMax - xMin), (yMax - yMin), (zMax - zMin));


}


float Triangle::ray_intersection(Ray& ray, float kEpsilon, ShadeRec& sr){

	Point e1 = p2 - p1;
	Point e2 = p3 - p1;

	Vect h = ray.getD().cross_product(e2);

	float a = e1.dot_product(h);

	if (a > -0.00001 && a < 0.00001)
		return -1.0;

	float f = ((float)1.0)/a;

	Point s = ray.getO() - p1;
	
	float u = f * (s.dot_product(h));

	if (u < 0.0 || u > 1.0){
		return -1.0;
	}

	Point q = s.cross_product(e1);

	float v = f * q.dot_product(ray.getD());

	if (v < 0.0 || (u + v) > 1.0){
		return -1.0;
	}

	float t = f * e2.dot_product(q);

	if (t > kEpsilon){
		sr.normal = (e1.cross_product(e2));
		sr.normal.normalize();
		sr.local_hit_point = (ray.getO() + (ray.getD() * t));
		return t;
	}

	else return -1.0;
}

void Triangle::setColor(const RGBColor& CC){
	color = CC;
}

RGBColor Triangle::getColor() const{
	return color;
}

BoundingBox Triangle::getBoundingBox() const{
	return box;
}

BRDF Triangle::getBRDF() const{

	return brdf;
}

float Triangle::calcBRDF(const Ray& inRay, const Ray& outRay, const Vect& n) const{
	return brdf.calculate(inRay, outRay, n);
}