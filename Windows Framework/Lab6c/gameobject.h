#pragma once
class GameObject
{
public:
	GameObject(void);
	~GameObject(void);
	void SetLocation(float x, float y, float z);
	void SetRotate(float xRot, float yRot, float zRot);

	void MoveDeltaX(float dx);
	void MoveDeltaY(float dy);
	void MoveDeltaZ(float dz);

	void RotateDeltaX(float dx);
	void RotateDeltaY(float dy);
	void RotateDeltaZ(float dz);

	float GetX();
	float GetY();
	float GetZ();

	float GetRotationX();
	float GetRotationY();
	float GetRotationZ();

private:
	float x, y, z;
	float xRot, yRot, zRot;
};

