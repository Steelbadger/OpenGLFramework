#include "heightmap.h"
#include "myvector4.h"



Heightmap::Heightmap(int iterations) :
	totalIterations(iterations),
	roughness(0.2f)
{

}

void Heightmap::Initialise()
{
	int requiredSize = 1;
	for (int i = 1; i < totalIterations; i++) {
		requiredSize *= 2;
	}
	squareSize = requiredSize;

	heightmap.resize(squareSize+1);
	normalmap.resize(squareSize+1);
	for (int i = 0; i <= squareSize; i++) {
		heightmap[i].resize(squareSize+1);
		normalmap[i].resize(squareSize+1);
	}

	for (int i = 0; i <= squareSize; i++) {
		for (int j = 0; j <= squareSize; j++) {
			heightmap[i][j] = 0.0f;
			normalmap[i][j] = Vector3(0.0f, 0.0f, 0.0f);
		}
	}


	//  Seed the Map
	heightmap[0][0] = 10.0f;
	heightmap[squareSize][0] = 3.5f;
	heightmap[0][squareSize] = -1.5f;
	heightmap[squareSize][squareSize] = -4.5f;

	GenHeightmap();
	GenNormals();

}

Heightmap::~Heightmap()
{

}

float Heightmap::GetFloatHeight(float x, float y)
{
	int xLow = floor(x);
	int xHigh = ceil(x);
	int yLow = floor(y);
	int yHigh = ceil(y);

	if (xLow < 0.0f || yLow < 0.0f || xHigh > heightmap.size()-1 || yHigh > heightmap.size()-1) {
		return 0.0f;
	}

	if (xLow == xHigh && yLow == yHigh) {
		return heightmap[xLow][yLow];
	} else if (xLow == xHigh) {
		return ((y-yLow)*heightmap[xLow][yLow]+(yHigh-y)*heightmap[xLow][yHigh]);
	} else if (yLow == yHigh) {
		return ((x-xLow)*heightmap[xLow][yLow]+(xHigh-x)*heightmap[xHigh][yLow]);
	}

	Vector4 A, B, C;

	if ((y-yLow) > 1.0f-(x-xLow)) {
		A = Vector3(xLow, heightmap[xLow][yLow], yLow);
		B = Vector3(xHigh, heightmap[xHigh][yHigh], yHigh);
		C = Vector3(xHigh, heightmap[xHigh][yLow], yLow);
	} else {
		A = Vector3(xLow, heightmap[xLow][yLow], yLow);
		B = Vector3(xLow, heightmap[xLow][yHigh], yHigh);
		C = Vector3(xHigh, heightmap[xHigh][yHigh], yHigh);		
	}

	Vector4 AB = B - A;
	Vector4 AC = C - A;

	Vector4 normal = AC.Cross(AB);
	normal.NormaliseSelf();

	Vector4 lookUp(0.0f, 1.0f, 0.0f, 1.0f);

	Vector4 diff = A - Vector4(x, 0.0f, y, 1.0f);

	float height = diff.Dot3(normal)/lookUp.Dot3(normal);

	return height;

}



float Heightmap::GetRelativeFloatHeight(float x, float y)
{
	if (x > 1.0f || y > 1.0f || x < 0.0f || y < 0.0f) {
		return 0.0f;
	}

	x = x*(heightmap.size()-1);
	y = y*(heightmap.size()-1);


	int xLow = floor(x);
	int xHigh = ceil(x);
	int yLow = floor(y);
	int yHigh = ceil(y);

	Vector4 A, B, C;

	if (y > 1.0f-x) {
		A = Vector3(xLow, heightmap[xLow][yLow], yLow);
		B = Vector3(xHigh, heightmap[xHigh][yHigh], yHigh);
		C = Vector3(xHigh, heightmap[xHigh][yLow], yLow);
	} else {
		A = Vector3(xLow, heightmap[xLow][yLow], yLow);
		B = Vector3(xLow, heightmap[xLow][yHigh], yHigh);
		C = Vector3(xHigh, heightmap[xHigh][yHigh], yHigh);		
	}

	if (xLow == xHigh && yLow == yHigh) {
		return heightmap[xLow][yLow];
	} else if (xLow == xHigh) {
		return ((y-yLow)*heightmap[xLow][yLow]+(yHigh-y)*heightmap[xLow][yHigh]);
	} else if (yLow == yHigh) {
		return ((x-xLow)*heightmap[xLow][yLow]+(xHigh-x)*heightmap[xHigh][yLow]);
	}

	Vector4 AB = B - A;
	Vector4 AC = C - A;

	Vector4 normal = AC.Cross(AB);
	normal.NormaliseSelf();

	Vector4 lookUp(0.0f, 1.0f, 0.0f, 1.0f);

	Vector4 diff = A - Vector4(x, 0.0f, y, 1.0f);

	float height = diff.Dot3(normal)/lookUp.Dot3(normal);

	return height;
}

void Heightmap::GenHeightmap()
{
	int numSquares;
	int step;
	int xLow, xHigh, yLow, yHigh;

	for (int i = 1; i < totalIterations; i++) {
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

void Heightmap::GenNormals()
{																											//	o-----------o			o
	for(int i = 0; i < heightmap.size(); i++) {																//	|	.	2	|	.		
		for(int j = 0; j < heightmap.size(); j++) {															//	|	1	.	|	3	.	
			int numNormals =  0;																			//	o-----------o-----------o
																											//		.	6	|	.	4	|
			if ((i > 0) && (j > 0)) {																		//			.	|	5	.	|
				normalmap[i][j] += CalcNormal	(Vector3(float(i-1), heightmap[i-1][j-1], float(j-1))		//	o			o-----------o
												,Vector3(float(i), heightmap[i][j], float(j))				//
												,Vector3(float(i-1), heightmap[i-1][j], float(j)));			//	Triangle 1

				normalmap[i][j] += CalcNormal	(Vector3(float(i-1), heightmap[i-1][j-1], float(j-1))
												,Vector3(float(i), heightmap[i][j-1], float(j-1))				//  Triangle 2
												,Vector3(float(i), heightmap[i][j], float(j)));
				numNormals += 2;
			}

			if (i < heightmap.size()-1 && j > 0) {
				normalmap[i][j] += CalcNormal	(Vector3(float(i), heightmap[i][j-1], float(j-1))
												,Vector3(float(i+1), heightmap[i+1][j], float(j))				//  Triangle 3
												,Vector3(float(i), heightmap[i][j], float(j)));

				numNormals++;
			}

			if (i < heightmap.size()-1 && j < heightmap.size()-1) {
				normalmap[i][j] += CalcNormal	(Vector3(float(i), heightmap[i][j], float(j))
												,Vector3(float(i+1), heightmap[i+1][j], float(j))			//  Triangle 4
												,Vector3(float(i+1), heightmap[i+1][j+1], float(j+1)));

				normalmap[i][j] += CalcNormal	(Vector3(float(i), heightmap[i][j], float(j))
												,Vector3(float(i+1), heightmap[i+1][j+1], float(j+1))		//  Triangle 5
												,Vector3(float(i), heightmap[i][j+1], float(j+1)));
				numNormals += 2;
			}

			if (i > 0 && j < heightmap.size()-1) {
				normalmap[i][j] += CalcNormal	(Vector3(float(i-1), heightmap[i-1][j], float(j))
												,Vector3(float(i), heightmap[i][j], float(j))				//  Triangle 6
												,Vector3(float(i), heightmap[i][j+1], float(j+1)));
				numNormals++;
			}

			normalmap[i][j] = Vector4(Vector4(normalmap[i][j])/numNormals).Normalise();
		}
	}
}

Vector3 Heightmap::CalcNormal(Vector3 pointA, Vector3 pointB, Vector3 pointC)
{
	Vector4 A(pointA);
	Vector4 B(pointB);
	Vector4 C(pointC);

	Vector4 AB = B - A;
	Vector4 AC = C - A;

	Vector4 Normal = AC.Cross(AB);
	Normal.NormaliseSelf();
	Vector3 output(Normal);

	return output;
}

Vector3 Heightmap::GetNormal(int i, int j) {
	return normalmap[i][j];
}

void Heightmap::SquareStep(int xLow, int xHigh, int yLow, int yHigh, int iteration)
{
	int xMid = (xHigh-xLow)/2 + xLow;
	int yMid = (yHigh-yLow)/2 + yLow;
	float midHeight = heightmap[xMid][yMid];

	midHeight = (heightmap[xLow][yLow] + heightmap[xLow][yHigh] + heightmap[xHigh][yLow] + heightmap[xHigh][yHigh])/4;

	midHeight += GetRandom(roughness*(xHigh-xLow));

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


	topMidHeight += GetRandom(roughness*(xHigh-xLow));
	bottomMidHeight += GetRandom(roughness*(xHigh-xLow));
	leftMidHeight += GetRandom(roughness*(xHigh-xLow));
	rightMidHeight += GetRandom(roughness*(xHigh-xLow));

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