#ifndef SHAPE_H
#define SHAPE_H

#include "RGBColor.h"
#include "Vect.h"
#include "Point.h"
#include "Ray.h"
#include "ShadeRec.h"
#include "BoundingBox.h"
#include "BRDF.h"

class Shape{
//	protected:
	//	RGBColor color;
	protected:
		virtual void createBoundingBox() = 0;

	public:
		virtual float ray_intersection(Ray&, float, ShadeRec&) = 0;
		virtual void setColor(const RGBColor&) = 0;
		virtual RGBColor getColor() const = 0;
		virtual BoundingBox getBoundingBox() const = 0;
		virtual BRDF getBRDF() const = 0;
		virtual float calcBRDF(const Ray&, const Ray&, const Vect&) const = 0;
};

#endif