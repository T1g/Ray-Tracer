#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"

class Triangle : public Shape{

	private:
		Point p1;
		Point p2;
		Point p3;
		RGBColor color;
		BoundingBox box;
		BRDF brdf;

		void createBoundingBox();

	public:
		Triangle();
		Triangle(const Point&, const Point&, const Point&, const RGBColor&);
		~Triangle();
		virtual float ray_intersection(Ray&, float, ShadeRec&);
		virtual void setColor(const RGBColor&);
		virtual RGBColor getColor() const;
		virtual BoundingBox getBoundingBox() const;
		virtual BRDF getBRDF() const;
		virtual float calcBRDF(const Ray&, const Ray&, const Vect&) const;

};

#endif