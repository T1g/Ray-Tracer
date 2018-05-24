#ifndef UNIFORMGRID_H
#define UNIFORMGRID_H

#include <vector>
#include <utility>
#include "Shape.h"

class UniformGrid{

	private:

		vector<Shape *> *** grid;
		int xLength;
		int yLength;
		int zLength;

		float minX;
		float minY;
		float minZ;

		Point p1;
		Point p2;


		Point getGiantBoxP1(vector<Shape *>);
		Point getGiantBoxP2(vector<Shape *>);
		void setLengthsAndMin(vector<Shape *>);


	public:

		UniformGrid();
		UniformGrid(vector<Shape *>);
		~UniformGrid();

		pair<vector<vector<Shape *>>, vector<BoundingBox>> ray_traversal(const Ray &);

		vector<Shape *> at(int, int, int);
		vector<Shape *> at(const Point &);

		BoundingBox box_at(const Point &);
};

#endif