#include "BRDF.h"

BRDF::BRDF(){

}

BRDF::BRDF(bool DD, float DV){

	diffuse = DD;
	diffuseValue = DV;

}

BRDF::~BRDF(){

}

//this probably needs more in it than this.
BRDF& BRDF::operator= (const BRDF& old){

	diffuse = old.isDiffuse();
	diffuseValue = old.getDiffuseValue();

	return *this;
}


bool BRDF::isDiffuse() const{

	return diffuse;
}

float BRDF::calculate(const Ray& inRay, const Ray& outRay, const Vect & n) const{

	if(diffuse){
		return diffuseValue;
	}
	else{

		//don't know what to do with this
		float M = 1.0;

		Vect L = inRay.getD();
		Vect V = outRay.getD();
		Vect H = (L + V)/((float)2.0);
		
		float cdiffpi = diffuseValue / ((float) M_PI);
		float shininess = (M + ((float)(8.0))) / ((float)(8.0) * ((float) M_PI));
		float RF = FresnelReflectance(L, H);
		float cosHN = H.dot_product(n);

		return cdiffpi + (shininess * RF * cosHN);
	}
}


float BRDF::getDiffuseValue() const{
	return diffuseValue;
}

float BRDF::FresnelReflectance(const Vect& L, const Vect& H) const{
	float n = 0.5;
	float RF0 = pow(((n - 1.0) / (n + 1.0)), 2);

	return (RF0 + (1.0 - RF0) * pow((1.0 - L.dot_product(H)), 5));
}