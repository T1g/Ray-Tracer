#include "BoundingBox.h"


BoundingBox::BoundingBox(){

}

BoundingBox::BoundingBox(const Point & PP, float XX, float YY, float ZZ){

	p1 = PP;
	xLength = XX;
	yLength = YY;
	zLength = ZZ;
	p2 = Point(PP.getX()+XX, PP.getY()+YY, PP.getZ()+ZZ);
}

BoundingBox::BoundingBox(const Point & PP1, const Point & PP2){

	p1 = PP1;
	p2 = PP2;

	xLength = fabsf(PP2.getX() - PP1.getX());
	yLength = fabsf(PP2.getY() - PP1.getY());
	zLength = fabsf(PP2.getZ() - PP1.getZ());

}

BoundingBox::~BoundingBox(){

}


BoundingBox& BoundingBox::operator= (const BoundingBox& old){

	p1 = old.getP1();
	p2 = old.getP2();
	xLength = old.getXLength();
	yLength = old.getYLength();
	zLength = old.getZLength();

	return *this;
}

bool BoundingBox::operator==(const BoundingBox& other){
	if(!(p1 == other.getP1())) return false;
	if(!(p2 == other.getP2())) return false;

	return true;
}

bool BoundingBox::box_intersect(const BoundingBox & box) const{
	
	Point aMin = p1;
	Point aMax = p2;

	Point bMin = box.getP1();
	Point bMax = box.getP2();

	if (aMax.getX() < bMin.getX()) return false; // a is left of b
	if (aMin.getX() > bMax.getX()) return false; // a is right of b
	if (aMax.getY() < bMin.getY()) return false; // a is above b
	if (aMin.getY() > bMax.getY()) return false; // a is below b
	if (aMax.getZ() < bMin.getZ()) return false; // a is in front of b
	if (aMin.getZ() > bMax.getZ()) return false; // a is behind b

    return true; // boxes overlap

}


bool BoundingBox::point_intersect(const Point & point) const{


/*	printf("point_intersect\n");
	printf("point:\n");
	point.printFloats();

	printf("box p2:\n");
	p2.printFloats();
	printf("box p1:\n");
	p1.printFloats();
*/
	if(point.getX() > p2.getX()) return false;
	if(point.getY() > p2.getY()) return false;
	if(point.getZ() > p2.getZ()) return false;

	if(point.getX() < p1.getX()) return false;
	if(point.getY() < p1.getY()) return false;
	if(point.getZ() < p1.getZ()) return false;

	return true;

}

/*
pair<float,float> BoundingBox::ray_intersect(const Ray & ray){


}
*/

//returns a pair of floats, which are the t for which the ray enters and exits the uniform grid. 
//Could also be used as a box intersection equation, probably.
pair<float,float> BoundingBox::ray_intersect(const Ray & ray){

	pair<float, float> failCase(-1.0, -1.0);

	float tmin = -1.0;
	float tmax = -1.0;

	/*
	printf("ray.getO():\n");
	ray.getO().printFloats();
	printf("ray.getD():\n");
	ray.getD().printFloats();

	printf("p1:\n");
	p1.printFloats();
	printf("p2:\n");
	p2.printFloats();

	*/

	//DIVIDE BY ZERO?!?!?!? WHY DID THE ALGORITHM MAKE THIS POSSIBLE?

	if(ray.getD().getX() != 0.0){

		tmin = (p1.getX() - ray.getO().getX()) / (ray.getD().getX());
		tmax = (p2.getX() - ray.getO().getX()) / (ray.getD().getX());

		//printf("tmin: %f, tmax: %f\n", tmin, tmax);

		if (tmin > tmax){
			float temp = tmin;
			tmin = tmax;
			tmax = temp;
		}

	}




	float tymin = (p1.getY() - ray.getO().getY()) / ray.getD().getY();
	float tymax = (p2.getY() - ray.getO().getY()) / ray.getD().getY();

	if (tymin > tymax){
		float tempy = tymin;
		tymin = tymax;
		tymax = tempy;
	}

	if(ray.getD().getX() != 0.0){

		if ((tmin > tymax) || (tymin > tmax)){
	//		printf("fail case ty!\n");
			if (((int)tmin > (int)tymax) || ((int)tymin > (int)tmax)){
				return failCase;
			}
		}
		if (tymin > tmin){// && tmin != -1.0){ not sure why i thought this was a good idea
			tmin = tymin;
		}
		if (tymax < tmax){// && tmin != -1.0){
			tmax = tymax;
		}
	}
	else{
		if(ray.getD().getY() != 0.0){
			tmin = tymin;
			tmax = tymax;
		}
	}




	float tzmin = (p1.getZ() - ray.getO().getZ()) / ray.getD().getZ();
	float tzmax = (p2.getZ() - ray.getO().getZ()) / ray.getD().getZ();

	if (tzmin > tzmax){
		float tempz = tzmin;
		tzmin = tzmax;
		tzmax = tempz;
	}

	if ((ray.getD().getX() != 0.0) || (ray.getD().getY() != 0.0)){
		if ((tmin > tzmax) || (tzmin > tmax)){
//			printf("fail case tz!\n");
			if (((int)tmin > (int)tzmax) || ((int)tzmin > (int)tmax)){
				return failCase;
			}
		}
		if (tzmin > tmin){ //&& tmin != -1.0){ not sure why i thought this was a good idea
			tmin = tzmin;
		}
		if (tzmax < tmax){ //&& tmin != -1.0){
			tmax = tzmax;
		}
	}

	else{
		if(ray.getD().getZ() != 0.0){
			tmin = tzmin;
			tmax = tzmax;
		}
	}

	//if ((tmin > r.tmax) || (tmax < r.tmin)){
	//	return false;
	//}
	//if (r.tmin < tmin){
	//tMinMax.first = tmin;
	//}
	//if (r.tmax > tmax){
	//tMinMax.second = tmax;
	//}

	//printf("success!!! with box_ray_intersection!\n");

	//printf("tmin: %f, tmax: %f\n", tmin, tmax);

	pair<float, float> tMinMax(tmin, tmax);
	return tMinMax;
}

Point BoundingBox::getP1() const{
	return p1;
}

Point BoundingBox::getP2() const{
	return p2;
}

float BoundingBox::getXLength() const{
	return xLength;
}

float BoundingBox::getYLength() const{
	return yLength;
}

float BoundingBox::getZLength() const{
	return zLength;
}