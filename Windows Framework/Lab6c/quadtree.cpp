#include "quadtree.h"

QuadTree::QuadTree()
{
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


bool QuadTree::BoxCheck(float xb, float yb, float rb, float xp, float yp)
{
	if (xp < xb + rb && xp > xb - rb && yp < yb + rb && yp > yb - rb) {
		return true;
	}
}
