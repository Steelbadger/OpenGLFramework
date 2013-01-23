#include "heightmap.h"



Heightmap::Heightmap(int iterations) :
	totalIterations(iterations)
{
	int requiredSize = 1;
	for (int i = 1; i < iterations; i++) {
		requiredSize *= 2;
	}
	squareSize = requiredSize;

	heightmap.resize(squareSize+1);
	for (int i = 0; i <= squareSize; i++) {
		heightmap[i].resize(squareSize+1);
	}

	for (int i = 0; i <= squareSize; i++) {
		for (int j = 0; j <= squareSize; j++) {
			heightmap[i][j] = 0.0f;
		}
	}


	//  Seed the Map
	heightmap[0][0] = 10.0f;
	heightmap[squareSize][0] = 3.5f;
	heightmap[0][squareSize] = -1.5f;
	heightmap[squareSize][squareSize] = -4.5f;

	GenHeightmap();
}

Heightmap::~Heightmap()
{

}

void Heightmap::GenHeightmap()
{
	int numSquares;
	int step;
	int xLow, xHigh, yLow, yHigh;

	for (int i = 1; i <= totalIterations; i++) {
		numSquares = 1;
		for (int c = 1; c < i; c++) {
			numSquares *= 2;
		}
		step = (heightmap.size()-1)/numSquares;

		for (int j = 0; j < numSquares; j++) {
			for (int k = 0; k < numSquares; k++) {
				xLow = j*step;
				xHigh = (j+1)*step;
				yLow = k*step;
				yHigh = (k+1)*step;
				SquareStep(j*step, (j+1)*step, k*step, (k+1)*step, i);
			}
		}
		for (int j = 0; j < numSquares; j++) {
			for (int k = 0; k < numSquares; k++) {
				DiamondStep(j*step, (j+1)*step, k*step, (k+1)*step, i);
			}
		}
	}
}

void Heightmap::SquareStep(int xLow, int xHigh, int yLow, int yHigh, int iteration)
{
	int xMid = (xHigh-xLow)/2 + xLow;
	int yMid = (yHigh-yLow)/2 + yLow;
	float midHeight = heightmap[xMid][yMid];

	midHeight = (heightmap[xLow][yLow] + heightmap[xLow][yHigh] + heightmap[xHigh][yLow] + heightmap[xHigh][yHigh])/4;

	midHeight += GetRandom(MAGNITUDE/(float(iteration)*float(iteration)*float(iteration)+5));

	heightmap[xMid][yMid] = midHeight;
}

void Heightmap::DiamondStep(int xLow, int xHigh, int yLow, int yHigh, int iteration)
{
	int xMid = (xHigh-xLow)/2 + xLow;
	int yMid = (yHigh-yLow)/2 + yLow;

	float topMidHeight, leftMidHeight, rightMidHeight, bottomMidHeight;

	topMidHeight = heightmap[xMid][yLow];
	bottomMidHeight = heightmap[xMid][yHigh];
	leftMidHeight = heightmap[xLow][yMid];
	rightMidHeight = heightmap[xHigh][yMid];


	if (yLow == 0) {
		topMidHeight = (heightmap[xLow][yLow] + heightmap[xHigh][yLow] + heightmap[xMid][yMid])/3;
	} else {
		topMidHeight = (heightmap[xLow][yLow] + heightmap[xHigh][yLow] + heightmap[xMid][yMid] + heightmap[xMid][2*yLow-yMid])/4;
	}

	if (yHigh == heightmap[0].size()-1) {
		bottomMidHeight = (heightmap[xLow][yHigh] + heightmap[xHigh][yHigh] + heightmap[xMid][yMid])/3;
	} else {
		bottomMidHeight = (heightmap[xLow][yHigh] + heightmap[xHigh][yHigh] + heightmap[xMid][yMid] + heightmap[xMid][2*yHigh-yMid])/4;
	}

	if (xLow == 0) {
		leftMidHeight = (heightmap[xLow][yLow] + heightmap[xLow][yHigh] + heightmap[xMid][yMid])/3;
	} else {
		leftMidHeight = (heightmap[xLow][yLow] + heightmap[xLow][yHigh] + heightmap[xMid][yMid] + heightmap[2*xLow-xMid][yMid])/4;
	}

	if (xHigh == heightmap.size()-1) {
		rightMidHeight = (heightmap[xHigh][yLow] + heightmap[xHigh][yHigh] + heightmap[xMid][yMid])/3;
	} else {
		rightMidHeight = (heightmap[xHigh][yLow] + heightmap[xHigh][yHigh] + heightmap[xMid][yMid] + heightmap[2*xHigh - xMid][yMid])/4;
	}


	topMidHeight += GetRandom(MAGNITUDE/(float(iteration)*float(iteration)*float(iteration)+8));
	bottomMidHeight += GetRandom(MAGNITUDE/(float(iteration)*float(iteration)*float(iteration)+8));
	leftMidHeight += GetRandom(MAGNITUDE/(float(iteration)*float(iteration)*float(iteration)+8));
	rightMidHeight += GetRandom(MAGNITUDE/(float(iteration)*float(iteration)*float(iteration)+8));

	heightmap[xMid][yLow] = topMidHeight;
	heightmap[xMid][yHigh] = bottomMidHeight;
	heightmap[xLow][yMid] = leftMidHeight;
	heightmap[xHigh][yMid] = rightMidHeight;
}

float Heightmap::GetRandom(float range)
{
	int randNum = RAND_MAX;
	do {
		randNum = rand();
	} while (randNum == RAND_MAX);
	randNum = randNum - ((RAND_MAX-1)/2);
	float randomInRange;

	randomInRange = (float(randNum)/float((RAND_MAX-1)/2))*range;

	return randomInRange;
}