#include "TripleFloat.h"

using namespace std;

TripleFloat::TripleFloat(){

}


TripleFloat::TripleFloat(float XX, float YY, float ZZ) {

	x = XX;
	y = YY;
	z = ZZ;

}

TripleFloat::TripleFloat(const TripleFloat& old){

	x = old.x;
	y = old.y;
	z = old.z;

}

TripleFloat::~TripleFloat(){
	
}



TripleFloat& TripleFloat::operator= (const TripleFloat& old){

	//delete anything if necessary
	x = old.getX();
	y = old.getY();
	z = old.getZ();

	return *this;
}

//NOTE: might have to declare new when passing back these things.

TripleFloat TripleFloat::operator+ (const TripleFloat& other) const{

	TripleFloat sum(x+other.getX(), y+other.getY(), z+other.getZ());

	return sum;
}

TripleFloat TripleFloat::operator- (const TripleFloat& other) const{

	TripleFloat diff(x - other.getX(), y-other.getY(), z-other.getZ());

	return diff;
}

TripleFloat TripleFloat::operator*(float s) const{

	TripleFloat scalar_product(s*x, s*y, s*z);

	return scalar_product;
}

TripleFloat TripleFloat::operator/(float s) const{

	TripleFloat scalar_divide(x/s, y/s, z/s);

	return scalar_divide;
}

bool TripleFloat::operator==(const TripleFloat& other) const{

	if(x != other.getX()) return false;
	if(y != other.getY()) return false;
	if(z != other.getZ()) return false;

	return true;
}

TripleFloat TripleFloat::cross_product(const TripleFloat& other) const{

	float newX = (y * other.getZ()) - (z * other.getY());
	float newY = (z * other.getX()) - (x * other.getZ());
	float newZ = (x * other.getY()) - (y * other.getX());

	TripleFloat cross(newX, newY, newZ);

	return cross;
}

float TripleFloat::dot_product(const TripleFloat& other) const{
	return (x * other.getX()) + (y * other.getY()) + (z * other.getZ());
}





float TripleFloat::getLength(){
	return sqrtf(x*x + y*y + z*z);
}

void TripleFloat::setValues(float XX, float YY, float ZZ){
	x = XX;
	y = YY;
	z = ZZ;
}

void TripleFloat::setX(float XX){
	x = XX;
}

void TripleFloat::setY(float YY){
	y = YY;
}

void TripleFloat::setZ(float ZZ){
	z = ZZ;
}

float TripleFloat::getX() const{
	return x;
}

float TripleFloat::getY() const{
	return y;
}

float TripleFloat::getZ() const{
	return z;
}

void TripleFloat::printFloats() const{

	printf("(X: %f, Y: %f, Z: %f)\n", x, y, z);

}