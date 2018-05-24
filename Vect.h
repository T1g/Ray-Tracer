#ifndef VECT_H
#define VECT_H

#include "TripleFloat.h"


class Vect: public TripleFloat{

	public:
		Vect();
		Vect(const TripleFloat&);
		Vect(float, float, float);
		~Vect();
		void normalize();

		Vect& operator= (const TripleFloat&);
};

#endif