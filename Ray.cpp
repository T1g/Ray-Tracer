#include "Ray.h"


Ray::Ray(){

}

Ray::Ray(const Point& OO, const Vect& DD){
	
	o = OO;
	d = DD;
}

Ray::Ray(const Ray& old){

	o = Point(old.getO());
	d = Vect(old.getD());

}

Ray::~Ray(){

}

void Ray::setO(const Point& OO){
	o = OO;
}
void Ray::setD(const Vect& DD){
	d = DD;
}

Point Ray::getO() const{
	return Point(o);
}

Vect Ray::getD() const{
	return Vect(d);
}

Point Ray::calculatePoint(float t) const{

	Point output(getO() + (getD() * t));

	//output.printFloats();
	return output;

}