#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"


class Cylinder : public Shape{

	private:
		Vect n;
		Point cap1;
		Point cap2;
		float radius;	
		int distance_compare(float *, int);
		RGBColor color;
		BRDF brdf;

		BoundingBox box;
		virtual void createBoundingBox();

	public:
		Cylinder();
		Cylinder(const Point&, const Point&, float, const RGBColor&);
		~Cylinder();
		float ray_intersection(Ray&, float, ShadeRec&);
		void setColor(const RGBColor&);
		RGBColor getColor() const;
		BoundingBox getBoundingBox() const;
		virtual BRDF getBRDF() const;
		virtual float calcBRDF(const Ray&, const Ray&, const Vect&) const;
};

#endif