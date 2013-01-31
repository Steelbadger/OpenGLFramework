#pragma once
#include <vector>
#include <list>
#include "myvector3.h"
#include "noisegenerator.h"

class QuadTree
{
public:
	enum quadrant{NE, NW, SE, SW};

	QuadTree();
	~QuadTree();

	float SampleValue(float x, float y);
	std::vector<Vector3> GetNodes();

private:

	Vector3 bounds[4];
	
	bool BoxCheck(float boxX, float boxY, float boxR, float pointX, float pointY);

	QuadTree* northEast;
	QuadTree* northWest;
	QuadTree* southEast;
	QuadTree* southWest;

	int depth;
	bool leaf;

	static const int MAXDEPTH = 12;
	static const int HIGHDETAILRADIUS = 10;
	static const int MEDIUMDETAILRADIUS = 200;
	static const int LOWDETAILRADIUS = 2000;
};
//
//class QuadTreeTerrain
//{
//public:
//	QuadTreeTerrain(float size);
//	~QuadTreeTerrain();
//
//private:
//	std::list<QuadTree*> map;
//}

