#include "UniformGrid.h"


UniformGrid::UniformGrid(){


}


UniformGrid::UniformGrid(vector<Shape *> shapes){

	setLengthsAndMin(shapes);

	grid = new vector<Shape *>**[xLength];
	for (int x = 0; x < xLength; x++){
		grid[x] = new vector<Shape *>*[yLength];

		for (int y = 0; y < yLength; y++){
			grid[x][y] = new vector<Shape *>[zLength];
		}

	}

	for (int x = 0; x < xLength; x++){
		for (int y = 0; y < yLength; y++){
			for (int z = 0; z < zLength; z++){

				Point gridBoxP1((float)x, (float)y, (float)z);
				Point gridBoxP2((float)(x+1), (float)(y+1), (float)(z+1));

				BoundingBox gridBox(gridBoxP1, gridBoxP2);

				for(int i = 0; i < shapes.size(); i++){

					if(gridBox.box_intersect(shapes.at(i)->getBoundingBox())){
						grid[x][y][z].push_back(shapes.at(i));
					}
				}

			}
		}
	}

}

//not sure if necessary, guess i will find out the hard way
UniformGrid::~UniformGrid(){

	for (int x = 0; x < xLength; x++){
		for (int y = 0; y < yLength; y++){
			for (int z = 0; z < zLength; z++){

				grid[x][y][z].clear();

			}
			delete grid[x][y];
		}
		delete grid[x];
	}
	//delete grid;
}


Point UniformGrid::getGiantBoxP1(vector<Shape *> shapes){

	float currMinX = 0.0;
	float currMinY = 0.0;
	float currMinZ = 0.0;

	for(int i = 0; i < shapes.size(); i++){
		currMinX = min(currMinX, shapes.at(i)->getBoundingBox().getP1().getX());
		currMinY = min(currMinX, shapes.at(i)->getBoundingBox().getP1().getY());
		currMinZ = min(currMinX, shapes.at(i)->getBoundingBox().getP1().getZ());
	}

	Point output(currMinX, currMinY, currMinZ);

	printf("giantboxp1\n");
	output.printFloats();

	return output;

}

Point UniformGrid::getGiantBoxP2(vector<Shape *> shapes){

	float currMaxX = 0.0;
	float currMaxY = 0.0;
	float currMaxZ = 0.0;

	for(int i = 0; i < shapes.size(); i++){
		currMaxX = max(currMaxX, shapes.at(i)->getBoundingBox().getP2().getX());
		currMaxY = max(currMaxX, shapes.at(i)->getBoundingBox().getP2().getY());
		currMaxZ = max(currMaxX, shapes.at(i)->getBoundingBox().getP2().getZ());
	}

	Point output(currMaxX, currMaxY, currMaxZ);

	printf("giantboxp2\n");
	output.printFloats();

	return output;

}


void UniformGrid::setLengthsAndMin(vector<Shape *> shapes){

	p1 = getGiantBoxP1(shapes);
	p2 = getGiantBoxP2(shapes);

	xLength = (int)(p2.getX() - p1.getX() );//+ 10.0);
	yLength = (int)(p2.getY() - p1.getY() );//+ 10.0);
	zLength = (int)(p2.getZ() - p1.getZ() );//+ 10.0);

	//minX = (p1.getX() );//- 5.0);
	//minY = (p1.getY() );//- 5.0);
	//minZ = (p1.getZ() );//- 5.0);
	

}

//MAKE SURE THAT WHEN RAY INTERSECTS TRIANGLE, THE INTERSECTION OCCURS IN THE CURRENT BOUNDING BOX

pair<vector<vector<Shape *>>, vector<BoundingBox>> UniformGrid::ray_traversal(const Ray & ray){
	
	vector<vector<Shape *>> boxshapes;
	vector<BoundingBox> boxes;

	BoundingBox big_box(p1, p2);


	//printf();
	pair<float,float> gridTminTmax = big_box.ray_intersect(ray);

	float currTmin = gridTminTmax.first;
	float lastTmax = gridTminTmax.second;

	Point currPoint = ray.calculatePoint(currTmin);
	
	//printf("currTmin: %f, lastTmax: %f\n", currTmin, lastTmax);
	

	while(currTmin < lastTmax){

		printf("currPoint:\n");
		currPoint.printFloats();

		boxshapes.push_back(at(currPoint));

		BoundingBox currBox = box_at(currPoint);

		boxes.push_back(currBox);

		pair<float,float> currT = currBox.ray_intersect(ray);

		if(currTmin != currT.first){
			printf("WARNING: currTmin and currT.first don't match!\n");
			printf("currTmin: %f, currT.first: %f\n", currTmin, currT.first);
		}

		currTmin = currT.second;
		currPoint = ray.calculatePoint(currTmin);

	}

	pair<vector<vector<Shape *>>, vector<BoundingBox>> output(boxshapes, boxes);

	return output;
}


//WARNING: THIS COULD REALLY BACKFIRE IF I MESSED UP EVEN A LITTLE
//currently, i am subtracting the minimum, which should make sure it checks at 0. this might be wrong though
//XX, YY, and ZZ should be based on the ray Coordinates
vector<Shape *> UniformGrid::at(int XX, int YY, int ZZ){

	int currX = XX - minX;
	int currY = YY - minY;
	int currZ = ZZ - minZ;

//	if(currX < 0){
//		base case?
//	}


	return grid[currX][currY][currZ];
}


vector<Shape *> UniformGrid::at(const Point & PP){

	int currX = (int)(PP.getX());// - minX);
	int currY = (int)(PP.getY());// - minY);
	int currZ = (int)(PP.getZ());// - minZ);

//	if(currX < 0){
//		base case?
//	}


	return grid[currX][currY][currZ];
}

BoundingBox UniformGrid::box_at(const Point & PP){

	int currX = (int)(PP.getX());// - minX);
	int currY = (int)(PP.getY());// - minY);
	int currZ = (int)(PP.getZ());// - minZ);

	Point boxP = Point((float)currX, (float)currY, (float)currZ);

	boxP.printFloats();

	BoundingBox box(boxP, 1.0, 1.0, 1.0);

	/*
	printf("box p2:\n");
	box.getP2().printFloats();
	printf("box p1:\n");
	box.getP1().printFloats();
	*/

	return box;
}