#include "UniGrid.h"


UniGrid::UniGrid(){


}


//I BELIEVE THE PROBLEM IS HERE. I THINK THE BOXES MIGHT NOT ACTUALLY BE ALIGNED PROPERLY. MIGHT NEED MIN OFFSET

UniGrid::UniGrid(vector<Shape *> shapes){

	setGiantBox(shapes);
	setLengths(shapes);

	float minX = giantBox.getP1().getX();
	float minY = giantBox.getP1().getY();
	float minZ = giantBox.getP1().getZ();

	grid = new GridBox**[xLength];
	for (int x = 0; x < xLength; x++){
		grid[x] = new GridBox*[yLength];

		for (int y = 0; y < yLength; y++){
			grid[x][y] = new GridBox[zLength];
		}

	}

	for (int x = 0; x < xLength; x++){
		for (int y = 0; y < yLength; y++){
			for (int z = 0; z < zLength; z++){

					//NEW LINES OF CODE MEANT TO FIX OFFSET
					//int currX = (int)(x) + (int)(minX);
					//int currY = (int)(y) + (int)(minY);
					//int currZ = (int)(z) + (int)(minZ);

					//NEW NEW LINES OF CODE MEANT TO FIX THE FIX??? I THINK BOXES MIGHT NEED TO HAVE FLOAT COORDINATES
					float currX = (float)(x) + minX;
					float currY = (float)(y) + minY;
					float currZ = (float)(z) + minZ;

					Point gBoxP1(currX, currY, currZ);
					Point gBoxP2((currX + (float)1.0), (currY + (float)1.0), (currZ + (float)1.0));

					//Point gBoxP1((float)currX, (float)currY, (float)currZ);
					//Point gBoxP2((float)(currX+1), (float)(currY+1), (float)(currZ+1));

				//Point gBoxP1((float)x, (float)y, (float)z);
				//Point gBoxP2((float)(x+1), (float)(y+1), (float)(z+1));

				BoundingBox gBox(gBoxP1, gBoxP2);

				GridBox gridBox(gBox);

				for(int i = 0; i < shapes.size(); i++){
					if(gBox.box_intersect(shapes.at(i)->getBoundingBox())){
						gridBox.push_back(shapes.at(i));
					}
				}

				grid[x][y][z] = gridBox;
			}
		}
	}

}

//not sure if necessary, guess i will find out the hard way
UniGrid::~UniGrid(){

	for (int x = 0; x < xLength; x++){
		for (int y = 0; y < yLength; y++){
			for (int z = 0; z < zLength; z++){

				grid[x][y][z].clear();

			}
//			delete grid[x][y];
		}
//		delete grid[x];
	}
	//delete grid;
}


void UniGrid::setGiantBox(vector<Shape *> shapes){

	float currMinX = 0.0;
	float currMinY = 0.0;
	float currMinZ = 0.0;

	float currMaxX = 0.0;
	float currMaxY = 0.0;
	float currMaxZ = 0.0;

	for(int i = 0; i < shapes.size(); i++){
		currMinX = min(currMinX, shapes.at(i)->getBoundingBox().getP1().getX());
		currMinY = min(currMinY, shapes.at(i)->getBoundingBox().getP1().getY());
		currMinZ = min(currMinZ, shapes.at(i)->getBoundingBox().getP1().getZ());

		currMaxX = max(currMaxX, shapes.at(i)->getBoundingBox().getP2().getX());
		currMaxY = max(currMaxY, shapes.at(i)->getBoundingBox().getP2().getY());
		currMaxZ = max(currMaxZ, shapes.at(i)->getBoundingBox().getP2().getZ());
	}

	Point gp1(currMinX, currMinY, currMinZ);
	printf("giantboxp1\n");
	gp1.printFloats();

	Point gp2(currMaxX, currMaxY, currMaxZ);
	printf("giantboxp2\n");
	gp2.printFloats();

	giantBox = BoundingBox(gp1, gp2);
}

void UniGrid::setLengths(vector<Shape *> shapes){

	Point p1 = giantBox.getP1();
	Point p2 = giantBox.getP2();

	xLength = (int)(p2.getX()) - (int)(p1.getX());//+ 10.0);
	yLength = (int)(p2.getY()) - (int)(p1.getY());//+ 10.0);
	zLength = (int)(p2.getZ()) - (int)(p1.getZ());//+ 10.0);

}


vector<GridBox> UniGrid::ray_traversal(const Ray & ray){
	
	vector<GridBox> gridBoxes;

	pair<float,float> gridTminTmax = giantBox.ray_intersect(ray);

	float currTmin = gridTminTmax.first;
	float lastTmax = gridTminTmax.second;

	Point firstPoint = ray.calculatePoint(currTmin);
	Point currPoint = ray.calculatePoint(currTmin);

	Point diffPoint = currPoint - firstPoint;
	Point zeroPoint = Point(0.0, 0.0, 0.0);

	GridBox prevGridBox;
	//need to keep track of two points in addition to the current point, we need to keep some sort of queue,
	//that keeps track of different points or soemthing? we need to keep track of the last point to get
	//added to the queue, in addition to the current point we are testing.

	
	//printf("currTmin: %f, lastTmax: %f\n", currTmin, lastTmax);

	while((currTmin < lastTmax) && point_inside_grid(currPoint)){
		bool flag = false;

		//if (point_inside_grid(currPoint)){
			//printf("currPoint:\n");
			//currPoint.printFloats();
			
			if(diffPoint == zeroPoint){
				diffPoint = (currPoint - firstPoint) /((float)10.0);
			}

			GridBox currGridBox = gridbox_at(currPoint);

			pair<float,float> currT = currGridBox.getBox().ray_intersect(ray);

		/*	if(currTmin != currT.first){
				//printf("WARNING: currTmin and currT.first don't match!\n");
				//printf("currTmin: %f, currT.first: %f\n", currTmin, currT.first);
			}
		*/
			Point testPoint = currPoint;
			GridBox testGridBox = gridbox_at(testPoint);

			//bool testFlag = te;

			while (currTmin == currT.second){

				if(diffPoint == zeroPoint){
					diffPoint = (ray.getD() / (float)(10.0)) * (float)(-1.0);
				}

				//This if statement is preventing an infinite loop for the triangle portion of things. Having said that, it might ruin EVERYTHING.
				//TIME TO FIND OUT I GUESS.

				flag = true;
				//we are infinite looping in the same box apparently!

				testPoint = testPoint + diffPoint;

				if(!(is_incremented(currGridBox, testGridBox))){
				/////////////////////////////////////////////////////////////////SO, basically, this is for cases where it increments by more than just one
																			//////Grid square in the x, y, or z direction. i.e. +1 x, +1 y, +0 z
				Point subPoint = testPoint - currPoint;

					if(subPoint.getX() != 0.0){
						Point xPoint = Point(subPoint.getX(), 0.0, 0.0);
						if(point_inside_grid(currPoint + xPoint)){
							GridBox xGridBox = gridbox_at(currPoint + xPoint);
							pair<float,float> TX = xGridBox.getBox().ray_intersect(ray);
							if(TX.second != currTmin){
								gridBoxes.push_back(xGridBox);
							}
						}
					}
					if(subPoint.getY() != 0.0){
						Point yPoint = Point(0.0, subPoint.getY(), 0.0);
						if(point_inside_grid(currPoint + yPoint)){
							GridBox yGridBox = gridbox_at(currPoint + yPoint);
							pair<float,float> TY = yGridBox.getBox().ray_intersect(ray);
							if(TY.second != currTmin){
								gridBoxes.push_back(yGridBox);
							}
						}
					}
					if(subPoint.getZ() != 0.0){
						Point zPoint = Point(0.0, 0.0, subPoint.getZ());
						if(point_inside_grid(currPoint + zPoint)){
							GridBox zGridBox = gridbox_at(currPoint + zPoint);
							pair<float,float> TZ = zGridBox.getBox().ray_intersect(ray);
							if(TZ.second != currTmin){
								gridBoxes.push_back(zGridBox);
							}
						}
					}
				}
				/////////////////

				if (!point_inside_grid(testPoint)){
					return gridBoxes;
				}

				testGridBox = gridbox_at(testPoint);
				currT = testGridBox.getBox().ray_intersect(ray);

			}

/*
			if(flag){
				if(!(is_incremented(currGridBox, testGridBox))){
				printf("OH GOD DAMMIT!\n");
				}
			}
*/
			gridBoxes.push_back(testGridBox);
			//currGridBox = testGridBox;



			//I changed this from second, which is what it is supposed to be, to first, to see if something different happened.
			currTmin = currT.second;
			currPoint = ray.calculatePoint(currTmin);
		//}
			if(currT.first == currT.second){
				printf("I think the ray just hit a tangential point on the box. I am literally speechless\n");
				//ray.getO().printFloats();
				//return gridBoxes;
			}

	}

	return gridBoxes;
}

GridBox UniGrid::gridbox_at(const Point & PP){

	float minX = giantBox.getP1().getX();
	float minY = giantBox.getP1().getY();
	float minZ = giantBox.getP1().getZ();

	//int currX = (int)(PP.getX()) - (int)(minX);
	//int currY = (int)(PP.getY()) - (int)(minY);
	//int currZ = (int)(PP.getZ()) - (int)(minZ);

	//printf("Gridbox at Point:\n");
	//PP.printFloats();

	int currX = (int)(PP.getX() - minX);
	int currY = (int)(PP.getY() - minY);
	int currZ = (int)(PP.getZ() - minZ);

	//printf("yields Gridbox at:\n");
	//Point gp((float)currX, (float)(currY), (float)currZ);
	//gp.printFloats();

	return grid[currX][currY][currZ];
}

bool UniGrid::point_inside_grid(const Point & PP) const{

	float minX = giantBox.getP1().getX();
	float minY = giantBox.getP1().getY();
	float minZ = giantBox.getP1().getZ();

	//int currX = (int)(PP.getX()) - (int)(minX);
	//int currY = (int)(PP.getY()) - (int)(minY);
	//int currZ = (int)(PP.getZ()) - (int)(minZ);

	int currX = (int)(PP.getX() - minX);
	int currY = (int)(PP.getY() - minY);
	int currZ = (int)(PP.getZ() - minZ);

	//float currX = PP.getX() - minX;
	//float currY = PP.getY() - minY;
	//float currZ = PP.getZ() - minZ;

	if(currX < 0) return false;
	if(currX >= xLength) return false;
	if(currY < 0) return false;
	if(currY >= yLength) return false;
	if(currZ < 0) return false;
	if(currZ >= zLength) return false;



	return true;
}

bool UniGrid::is_incremented(const GridBox & GG1, const GridBox & GG2) const{


	Point diffPoint = GG1.getBox().getP1() - GG2.getBox().getP1();
	Point xPoint1(1.0, 0.0, 0.0);
	Point xPoint2(-1.0, 0.0, 0.0);
	Point yPoint1(0.0, 1.0, 0.0);
	Point yPoint2(0.0, -1.0, 0.0);
	Point zPoint1(0.0, 0.0, 1.0);
	Point zPoint2(0.0, 0.0, -1.0);


	if(diffPoint == xPoint1) return true;
	if(diffPoint == xPoint2) return true;
	if(diffPoint == yPoint1) return true;
	if(diffPoint == yPoint2) return true;
	if(diffPoint == zPoint1) return true;
	if(diffPoint == zPoint2) return true;

	if(fabsf(diffPoint.getX()) > 1.0){
		printf("WHXXXXX\n");
	}
	if(fabsf(diffPoint.getY()) > 1.0){
		printf("WHYYYYY\n");
	}
	if(fabsf(diffPoint.getZ()) > 1.0){
		printf("WHZZZZZ\n");
	}


	return false;
}