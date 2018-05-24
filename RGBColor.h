#ifndef RGBCOLOR_H
#define RGBCOLOR_H

#include <iostream>
#include <math.h>
using namespace std;


class RGBColor{

	private:
		float r;
		float g;
		float b;

	public:
		RGBColor();
		RGBColor(float RR, float GG, float BB);
		RGBColor(const RGBColor&);
		~RGBColor();

		RGBColor& operator= (const RGBColor&);
		RGBColor operator+ (const RGBColor&);
		RGBColor operator- (const RGBColor&);
		RGBColor operator*(float);
		RGBColor operator/(float);

		float getR() const;
		float getG() const;
		float getB() const;

		void setR(float);
		void setG(float);
		void setB(float);

		void setValues(float, float, float);

		void printFloats() const;
};

#endif