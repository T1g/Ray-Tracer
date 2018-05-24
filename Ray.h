#ifndef RAY_H
#define RAY_H

#include "Point.h"
#include "Vect.h"


class Ray{

	private:
		Point o;
		Vect d;
	public:
		Ray();
		Ray(const Point&, const Vect&);
		Ray(const Ray&);
		~Ray();
		Point getO() const;
		Vect getD() const;
		void setO(const Point&);
		void setD(const Vect&);

		Point calculatePoint(float) const;
};

#endif
