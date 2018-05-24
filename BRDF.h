#ifndef BRDF_H
#define BRDF_H

#include "Ray.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdio>

using namespace std;


class BRDF{

	private:
		bool diffuse;
		float diffuseValue;
		float FresnelReflectance(const Vect&, const Vect&) const;

	public:
		BRDF();
		BRDF(bool, float);
		~BRDF();
		BRDF& operator= (const BRDF&);
		bool isDiffuse() const;
		float calculate(const Ray&, const Ray&, const Vect&) const;
		float getDiffuseValue() const;

};

#endif