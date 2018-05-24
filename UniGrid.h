#ifndef UNIGRID_H
#define UNIGRID_H

#include <utility>
#include "GridBox.h"


class UniGrid{

	private:

		GridBox *** grid;

		int xLength;
		int yLength;
		int zLength;

		BoundingBox giantBox;

		void setLengths(vector<Shape *>);
		void setGiantBox(vector<Shape *>);


	public:

		UniGrid();
		UniGrid(vector<Shape *>);
		~UniGrid();

		vector<GridBox> ray_traversal(const Ray &);
		GridBox gridbox_at(const Point &);

		bool point_inside_grid(const Point &) const;
		bool is_incremented(const GridBox &, const GridBox &) const;

};

#endif