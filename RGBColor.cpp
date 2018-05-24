#include "RGBColor.h"


RGBColor::RGBColor(){

}

RGBColor::RGBColor (float RR, float GG, float BB) {

	r = RR;
	g = GG;
	b = BB;

}

RGBColor::RGBColor(const RGBColor& old){

	r = old.r;
	g = old.g;
	b = old.b;

}

RGBColor::~RGBColor(){
	
}



RGBColor& RGBColor::operator= (const RGBColor& old){

	//delete anything if necessary
	r = old.r;
	g = old.g;
	b = old.b;

	return *this;
}

//NOTE: might have to declare new when passing back these things.

RGBColor RGBColor::operator+ (const RGBColor& other){

	RGBColor sum(r+other.r, g+other.g, b+other.b);

	return sum;
}

RGBColor RGBColor::operator- (const RGBColor& other){

	RGBColor diff(r-other.r, g-other.g, b-other.b);

	return diff;
}

RGBColor RGBColor::operator*(float s){

	RGBColor scalar_product(s*r, s*g, s*b);

	return scalar_product;
}

RGBColor RGBColor::operator/(float s){

	RGBColor scalar_divide(r/s, g/s, b/s);

	return scalar_divide;
}


void RGBColor::setValues(float RR, float GG, float BB){
	r = RR;
	g = GG;
	b = BB;
}

void RGBColor::setR(float RR){
	r = RR;
}

void RGBColor::setG(float GG){
	g = GG;
}

void RGBColor::setB(float BB){
	b = BB;
}

float RGBColor::getR() const{
	return r;
}

float RGBColor::getG() const{
	return g;
}

float RGBColor::getB() const{
	return b;
}

void RGBColor::printFloats() const{

	printf("(R: %f, G: %f, B: %f)\n", r, g, b);

}