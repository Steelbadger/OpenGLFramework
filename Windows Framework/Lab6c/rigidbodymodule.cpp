#include "rigidbodymodule.h"
#include "gameobject.h"
#include "noisegenerator.h"


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

bool Rigidbody::CheckGroundCollision(NoiseObject n)
{
	NoiseGenerator ground;
	ground.Seed(n.seed);
	n.octaves = 9;
	if(active) {
		if (parent->GetPosition().y < 2.0f+ground.TurbulentPerlin2D(parent->GetPosition().x, parent->GetPosition().z, n)) {
			parent->SetLocation(parent->GetPosition().x, 2.0f+ground.TurbulentPerlin2D(parent->GetPosition().x, parent->GetPosition().z, n), parent->GetPosition().z);
			velocity = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
			return true;
		}

		if (parent->GetPosition().y > 2.0f+ground.TurbulentPerlin2D(parent->GetPosition().x, parent->GetPosition().z, n)) {
			velocity -= GameObject::GLOBALY*GRAVITY*input.GetTimeForLastFrame();
			return false;
		}
	}
	velocity = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	return false;
}

void Rigidbody::Update()
{
	parent->MoveDeltaY(velocity.y*input.GetTimeForLastFrame());
}
