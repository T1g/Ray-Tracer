#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape{

	private:
		float r;
		Point center;
		RGBColor color;
		BoundingBox box;
		BRDF brdf;

		virtual void createBoundingBox();

	public:
		Sphere();
		Sphere(float, const Point&, const RGBColor&);
		Sphere(float, const Point&, const RGBColor&, const BRDF&);
		~Sphere();
		virtual float ray_intersection(Ray&, float, ShadeRec&);
		virtual void setColor(const RGBColor&);
		virtual RGBColor getColor() const;
		virtual BoundingBox getBoundingBox() const;
		virtual BRDF getBRDF() const;
		virtual float calcBRDF(const Ray&, const Ray&, const Vect&) const;
};

#endif