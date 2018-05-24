#include "Point.h"

Point::Point() : TripleFloat(){

}

Point::Point (const TripleFloat& old){

	x = old.getX();
	y = old.getY();
	z = old.getZ();

}

Point::Point(float XX, float YY, float ZZ) : TripleFloat(XX, YY, ZZ){
	x = XX;
	y = YY;
	z = ZZ;
}

Point::~Point(){

}

Point& Point::operator= (const TripleFloat& old){

	x = old.getX();
	y = old.getY();
	z = old.getZ();

	return *this;

}