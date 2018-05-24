#ifndef TRIPLEFLOAT_H
#define TRIPLEFLOAT_H

#include <math.h>
#include <cstdio>
using namespace std;


class TripleFloat{

	protected:
		float x;
		float y;
		float z;

	public:
		TripleFloat();
		TripleFloat(float XX, float YY, float ZZ);
		TripleFloat(const TripleFloat&);
		~TripleFloat();

		TripleFloat& operator= (const TripleFloat&);
		TripleFloat operator+ (const TripleFloat&) const;
		TripleFloat operator- (const TripleFloat&) const;
		TripleFloat operator*(float) const;
		TripleFloat operator/(float) const;
		bool operator==(const TripleFloat&) const;

		TripleFloat cross_product(const TripleFloat&) const;
		float dot_product(const TripleFloat&) const;

		float getX() const;
		float getY() const;
		float getZ() const;

		void setX(float);
		void setY(float);
		void setZ(float);

		void setValues(float, float, float);
		float getLength();

		void printFloats() const;
};

#endif