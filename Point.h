#ifndef POINT_H
#define POINT_H

#include "TripleFloat.h"

class Point: public TripleFloat{

	public:
		Point();
		Point(const TripleFloat&);
		Point(float, float, float);
		~Point();

		Point& operator= (const TripleFloat&);

};

#endif