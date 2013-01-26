#pragma once
#include "myvector4.h"

class GameObject;
class Mesh;

class Rigidbody
{
public:
	Rigidbody(GameObject* parent);
	~Rigidbody(void);

	static float GRAVITY;
	static void SetGravity(float g);

	void Update();

	bool CheckGroundCollision(Mesh &ground);
	
	Vector4 GetVelocity(){return velocity;}
	void SetVelocity(Vector4 v){velocity = v;}

	void SetInactive(){active = false;}
	void SetActive(){active = true;}
	
private:
	GameObject* parent;
	bool active;

	Vector4 velocity;
};

