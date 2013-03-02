#pragma once
#include "gameobject.h"

class Matrix4x4;
class Vector4;
class Vector3;

struct Light
{
	float position[4];
	float colour[4];
	int type;
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
	void CalculatePoint(Matrix4x4 view, Light &out);
};
