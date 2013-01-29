#include "rigidbodymodule.h"
#include "gameobject.h"


float Rigidbody::GRAVITY = 9.81f;

Rigidbody::Rigidbody(GameObject* p):
	parent(p),
	active(false),
	velocity(0.0f, 0.0f, 0.0f, 1.0f)
{
}


Rigidbody::~Rigidbody(void)
{
}

void Rigidbody::SetGravity(float g)
{
	GRAVITY = g;
}

bool Rigidbody::CheckGroundCollision(Terrain &ground)
{
	if(active) {
		if (parent->GetPosition().y < 2.0f+ground.GetHeight(parent->GetPosition().x, parent->GetPosition().z)) {
			parent->SetLocation(parent->GetPosition().x, 2.0f+ground.GetHeight(parent->GetPosition().x, parent->GetPosition().z), parent->GetPosition().z);
			velocity = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
			return true;
		}

		if (parent->GetPosition().y > 2.0f+ground.GetHeight(parent->GetPosition().x, parent->GetPosition().z)) {
			velocity -= GameObject::GLOBALY*GRAVITY*input.GetTimeForLastFrame();
			return false;
		}
	}

	return false;
}

void Rigidbody::Update()
{
	parent->MoveDeltaY(velocity.y*input.GetTimeForLastFrame());
}
