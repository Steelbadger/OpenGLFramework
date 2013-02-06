#include "lights.h"
#include "myvector4.h"


DirectionalLight::DirectionalLight(float dX, float dY, float dZ, float red, float green, float blue)
{
	Vector4 dir(dX, dY, dZ, 1.0);
	dir.NormaliseSelf();
	direction = dir;
	r = red;
	g = green;
	b = blue;
}


LightSource::LightSource(GameObject* p):
	parent(p)
{
}


LightSource::~LightSource(void)
{
}
