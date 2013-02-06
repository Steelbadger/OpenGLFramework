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
};

struct DirectionalLight
{
	DirectionalLight(float dX, float dY, float dZ, float r, float g, float b);
	Vector3 direction;
	float r;
	float g;
	float b;
};


class LightSource
{
public:
	LightSource(GameObject* parent);
	~LightSource(void);
	DirectionalLight GetAsDLight();
private:
	GameObject* parent;
	Vector3 colour;
	float brightness;
};

