#ifndef GRIDBOX_H
#define GRIDBOX_H

#include "BoundingBox.h"
#include "Shape.h"
#include <vector>

class GridBox{

	private:
		BoundingBox box;
		vector<Shape *> shape_list;


	public:
		GridBox();
		GridBox(const BoundingBox &);
		~GridBox();

		GridBox& operator=(const GridBox&);

		BoundingBox getBox() const;

		void push_back(Shape *);
		Shape * at(int) const;
		void clear();
		int size() const;
};





#endif