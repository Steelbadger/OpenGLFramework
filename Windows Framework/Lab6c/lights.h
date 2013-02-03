#pragma once
#include "gameobject.h"

struct PointLight
{
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
	float brightness;
};

struct DirectionalLight
{
	float directionVectorX;
	float directionVectorY;
	float directionVectorZ;
	float r;
	float g;
	float b;
	float brightness;
};





class LightSource : public GameObject
{
public:
	LightSource(void);
	~LightSource(void);
	Vector3 colour;
	float brightness;


private:
};

