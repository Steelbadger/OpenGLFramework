#pragma once
#include "mesh.h"

struct Point {
	Vector3 p;
	Vector3 n;
	Vector2 u;
};


class PrimitiveFactory
{
public:
	PrimitiveFactory(void);
	~PrimitiveFactory(void);

	Mesh Plane(float width, float height, int widthSubDivs, int heightSubDivs);

	Mesh SimpleInnerBox();
	Mesh UnitCube();
	Mesh LSphere(int lats, int longs);
	Mesh GSphere(int resolution);
	Mesh SubDivide(Mesh m);

private:
	Point AveragePoints(Point p1, Point p2);

};

