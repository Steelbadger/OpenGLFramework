#include "lights.h"
#include "myvector4.h"

LightSource::LightSource(Type t):
	type(t),
	ambient(0.0f),
	colour(0.0f, 0.0f, 0.0f),
	angle(0.0f)
{
}

LightSource::~LightSource(void)
{
}

Light LightSource::GetLightAsStruct(Matrix4x4 view)
{
	Light out;

	switch (type) {
		case POINT:
			CalculatePoint(view, out);
			break;
		case DIRECTIONAL:
			CalculateDirectional(view, out);
			break;
		case SPOT:
			out.position[0] = GetPosition().x;
			out.position[1] = GetPosition().y;
			out.position[2] = GetPosition().z;
			out.position[3] = 1.0f;
			out.colour[3] = angle;
			out.type = 2;
			break;
	}

	out.colour[0] = colour.x;
	out.colour[1] = colour.y;
	out.colour[2] = colour.z;

	return out;
}

void LightSource::SetAmbient(float b)
{
	ambient = b;
}

void LightSource::SetColour(float r, float g, float b)
{
	colour = Vector3(r, g, b);
}

void LightSource::CalculateDirectional(Matrix4x4 view, Light &out)
{
	Matrix4x4 transform(Matrix4x4::IDENTITY);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			transform.elem[i][j] = view.elem[i][j];
		}
	}
	Vector4 pos = GetLocalZ();
	pos = transform * pos;

	out.position[0] = pos.x;
	out.position[1] = pos.y;
	out.position[2] = pos.z;
	out.position[3] = 0.0f;
	out.colour[3] = ambient;
	out.type = 1;
}

void LightSource::CalculatePoint(Matrix4x4 view, Light&out)
{
	Vector4 pos = GetPosition();

	pos = view * pos;
	out.position[0] = pos.x;
	out.position[1] = pos.y;
	out.position[2] = pos.z;
	out.position[3] = 1.0f;
	out.colour[3] = ambient;
	out.type = 0;
}