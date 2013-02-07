#pragma once
#include "gameobject.h"

struct Light
{
	float position[4];
	float colour[4];
};

class LightSource : public GameObject
{
public:
	enum Type{POINT, DIRECTIONAL, SPOT};

	LightSource(Type);
	~LightSource(void);
	Light GetLightAsStruct(Matrix4x4 view);
	void SetColour(float r, float g, float b);
	void SetAmbient(float brightness);
	void SetBeamAngle(float angle);
private:
	Vector3 colour;
	float ambient;
	Type type;
	float angle;

	void CalculateDirectional(Matrix4x4 view, Light &out);
};
