#include "Vect.h"


Vect::Vect() : TripleFloat(){

}

Vect::Vect (const TripleFloat& old){

	x = old.getX();
	y = old.getY();
	z = old.getZ();

}

Vect::Vect(float XX, float YY, float ZZ) : TripleFloat(XX, YY, ZZ){
	x = XX;
	y = YY;
	z = ZZ;
}

Vect::~Vect(){

}

void Vect::normalize(){
	
	float length = sqrt(x*x + y*y + z*z);
	
	x = x/length;
	y = y/length;
	z = z/length;

}

Vect& Vect::operator= (const TripleFloat& old){

	x = old.getX();
	y = old.getY();
	z = old.getZ();

	return *this;

}