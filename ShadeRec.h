#ifndef SHADEREC_H
#define SHADEREC_H

#include "Point.h"
#include "Vect.h"
#include "RGBColor.h"


class ShadeRec{

	public:
		bool hit_an_object;
		Point local_hit_point;
		Vect normal;
		RGBColor color;
		ShadeRec();
};

#endif