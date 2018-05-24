#ifndef PLANE_H
#define PLANE_H

#include "Shape.h"


class Plane : public Shape{

	private:
		Vect n;
		Point p;
		RGBColor color;
		BoundingBox box;
		BRDF brdf;

		virtual void createBoundingBox();
	public:
		Plane();
		Plane(const Vect&, const Point&, const RGBColor&);
		~Plane();
		virtual float ray_intersection(Ray&, float, ShadeRec&);
		virtual void setColor(const RGBColor&);
		virtual RGBColor getColor() const;
		BoundingBox getBoundingBox() const;
		virtual BRDF getBRDF() const;
		virtual float calcBRDF(const Ray&, const Ray&, const Vect&) const;
};

#endif