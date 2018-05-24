#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Ray.h"
#include <utility>


class BoundingBox{

	private:
		Point p1;
		Point p2;
		float xLength;
		float yLength;
		float zLength;

	public:
		BoundingBox();
		BoundingBox(const Point&, float, float, float);
		BoundingBox(const Point&, const Point &);
		~BoundingBox();

		BoundingBox& operator= (const BoundingBox&);
		bool operator== (const BoundingBox&);

		bool box_intersect(const BoundingBox &) const;

		bool point_intersect(const Point &) const;

		pair<float,float> ray_intersect(const Ray &);

		Point getP1() const;
		Point getP2() const;
		float getXLength() const;
		float getYLength() const;
		float getZLength() const;

};

#endif