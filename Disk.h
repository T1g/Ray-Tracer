#include "Shape.h"


class Disk : public Shape{

	private:
		Vect n;
		Point p;
		float r;
		float intersection_helper(Ray&, float, ShadeRec&);
		RGBColor color;
		BRDF brdf;

		BoundingBox box;
		virtual void createBoundingBox();

	public:
		Disk();
		Disk(const Vect&, const Point&, float, const RGBColor&);
		~Disk();
		virtual float ray_intersection(Ray&, float, ShadeRec&);
		void setColor(const RGBColor&);
		virtual RGBColor getColor() const;
		virtual BoundingBox getBoundingBox() const;
		virtual BRDF getBRDF() const;
		virtual float calcBRDF(const Ray&, const Ray&, const Vect&) const;

};