#include "GridBox.h"


GridBox::GridBox(){

}
GridBox::GridBox(const BoundingBox & BB){
	box = BB;
}
GridBox::~GridBox(){

}


GridBox& GridBox::operator= (const GridBox& old){

	box = old.getBox();
	int length = old.size();
	for(int i = 0; i < length; i++){
		shape_list.push_back(old.at(i));
	}
	//shapes_list = old.shape_list.

	return *this;
}


void GridBox::push_back(Shape * SS){

	shape_list.push_back(SS);

}


Shape * GridBox::at(int i) const{

	return shape_list.at(i);
}

void GridBox::clear(){
	shape_list.clear();
}

BoundingBox GridBox::getBox() const{

	return box;
}

int GridBox::size() const{
	return shape_list.size();
}