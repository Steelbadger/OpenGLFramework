#include "quadtree.h"

QuadTree::QuadTree(NoiseObject n):
	noiseSeed(n)
{
}

QuadTree::QuadTree(int depthOfParent, NoiseObject n, float xMin, float yMin):
	noiseSeed(n)
{
	depth = depthOfParent+1;
	if(depth%3 != 0) {
		noiseSeed.octaves++;
	}
}


QuadTree::~QuadTree(void)
{
	if (northEast != NULL)
		delete northEast;
	if (northWest != NULL)
		delete northWest;
	if (southEast != NULL)
		delete southEast;
	if (southWest != NULL)
		delete southWest;
}


void QuadTree::Update(float x, float y)
{

}

bool QuadTree::BoxCheck(float xb, float yb, float rb, float xp, float yp)
{
	if (xp < xb + rb && xp > xb - rb && yp < yb + rb && yp > yb - rb) {
		return true;
	}
}
