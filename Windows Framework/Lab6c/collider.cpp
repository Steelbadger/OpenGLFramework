#include "collider.h"

std::vector<Collider*>Collider::colliders;

Collider::Collider(Vector4 &BasePosition):
	basePosition(&BasePosition)
{
	AddToColliders(this);
}


Collider::~Collider(void)
{
}

void Collider::AddToColliders(Collider* collider)
{
	colliders.push_back(collider);
}

SphereCollider::SphereCollider(float r, Vector4 &BasePosition) :
	Collider(BasePosition),
	radius(r)
{
}

SphereCollider::~SphereCollider()
{
}

bool SphereCollider::IsColliding(SphereCollider &other)
{
	Vector4 displacement = *other.basePosition - *basePosition;
	float distance = displacement.LengthSqr();
	if ((radius * radius + other.radius*other.radius) > distance) {
		return false;
	}
	collision.NormalToPlane = displacement.Normalise();
	collision.PointOfCollision = *basePosition + displacement; 
	return true;
}

bool SphereCollider::IsColliding(AABBCollider &other)
{
	Vector4 translatedOther = *other.basePosition - *basePosition;

	Vector4 pointArray[8] = {Vector4(translatedOther.x - other.width/2, translatedOther.y - other.height/2, translatedOther.z - other.depth/2, 1.0f),
							 Vector4(translatedOther.x + other.width/2, translatedOther.y - other.height/2, translatedOther.z - other.depth/2, 1.0f),
							 Vector4(translatedOther.x + other.width/2, translatedOther.y + other.height/2, translatedOther.z - other.depth/2, 1.0f),
							 Vector4(translatedOther.x + other.width/2, translatedOther.y + other.height/2, translatedOther.z + other.depth/2, 1.0f),
							 Vector4(translatedOther.x + other.width/2, translatedOther.y - other.height/2, translatedOther.z + other.depth/2, 1.0f),
							 Vector4(translatedOther.x - other.width/2, translatedOther.y - other.height/2, translatedOther.z + other.depth/2, 1.0f),
							 Vector4(translatedOther.x - other.width/2, translatedOther.y + other.height/2, translatedOther.z + other.depth/2, 1.0f),
							 Vector4(translatedOther.x - other.width/2, translatedOther.y + other.height/2, translatedOther.z - other.depth/2, 1.0f)};

	for(int i = 0; i < 8; i++) {
		if (pointArray[i].LengthSqr() < radius*radius) {
			return true;
		}
	}
	return false;
}

bool SphereCollider::IsColliding(OABBCollider &other)
{
	return false;
}

AABBCollider::AABBCollider(float w, float h, float d, Vector4 &BasePosition):
	Collider(BasePosition),
	width(w),
	height(h),
	depth(d)
{
}

AABBCollider::~AABBCollider()
{
}

bool AABBCollider::IsColliding(SphereCollider &other)
{
	Vector4 translated = *basePosition - *other.basePosition;
	Vector4 pointArray[8] = {Vector4(translated.x - width/2, translated.y - height/2, translated.z - depth/2, 1.0f),
							 Vector4(translated.x + width/2, translated.y - height/2, translated.z - depth/2, 1.0f),
							 Vector4(translated.x + width/2, translated.y + height/2, translated.z - depth/2, 1.0f),
							 Vector4(translated.x + width/2, translated.y + height/2, translated.z + depth/2, 1.0f),
							 Vector4(translated.x + width/2, translated.y - height/2, translated.z + depth/2, 1.0f),
							 Vector4(translated.x - width/2, translated.y - height/2, translated.z + depth/2, 1.0f),
							 Vector4(translated.x - width/2, translated.y + height/2, translated.z + depth/2, 1.0f),
							 Vector4(translated.x - width/2, translated.y + height/2, translated.z - depth/2, 1.0f)};

	for(int i = 0; i < 8; i++) {
		if (pointArray[i].LengthSqr() < other.radius*other.radius) {
			return true;
		}
	}
	return false;
}

bool AABBCollider::IsColliding(AABBCollider &other)
{
	Vector4 translated = *basePosition - *other.basePosition;
	Vector4 pointArray[8] = {Vector4(translated.x - width/2, translated.y - height/2, translated.z - depth/2, 1.0f),
							 Vector4(translated.x + width/2, translated.y - height/2, translated.z - depth/2, 1.0f),
							 Vector4(translated.x + width/2, translated.y + height/2, translated.z - depth/2, 1.0f),
							 Vector4(translated.x + width/2, translated.y + height/2, translated.z + depth/2, 1.0f),
							 Vector4(translated.x + width/2, translated.y - height/2, translated.z + depth/2, 1.0f),
							 Vector4(translated.x - width/2, translated.y - height/2, translated.z + depth/2, 1.0f),
							 Vector4(translated.x - width/2, translated.y + height/2, translated.z + depth/2, 1.0f),
							 Vector4(translated.x - width/2, translated.y + height/2, translated.z - depth/2, 1.0f)};

	for(int i = 0; i < 8; i++) {
		if (pointArray[i].x < other.width/2 && pointArray[i].x > -other.width/2) {
			if (pointArray[i].y < other.height/2 && pointArray[i].y > -other.height/2) {
				if (pointArray[i].z < other.depth/2 && pointArray[i].z > -other.depth/2) {
					return true;
				}
			}
		}
	}
	return false;
}

bool AABBCollider::IsColliding(OABBCollider &other)
{
	return false;
}


OABBCollider::OABBCollider(Vector4 &BasePosition):
	Collider(BasePosition)
{
}

OABBCollider::~OABBCollider()
{
}

bool OABBCollider::IsColliding(SphereCollider &other)
{
	return false;
}

bool OABBCollider::IsColliding(AABBCollider &other)
{
	return false;
}

bool OABBCollider::IsColliding(OABBCollider &other)
{
	return false;
}