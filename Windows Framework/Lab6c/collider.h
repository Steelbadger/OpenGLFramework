#pragma once
#include "myvector4.h"
#include <vector>
#include "Terrain.h"

class AABBCollider;
class OABBCollider;
class SphereCollider;

struct Collision
{
	Vector4 PointOfCollision;
	Vector4 NormalToPlane;
};


class Collider
{
public:
	virtual ~Collider(void);

	virtual bool IsColliding(SphereCollider &other) = 0;
	virtual bool IsColliding(AABBCollider &other) = 0;
	virtual bool IsColliding(OABBCollider &other) = 0;
	virtual bool IsColliding(Terrain &other) = 0;
	Collision GetCollision() {return collision;}
	Vector4 *basePosition;
protected:
	Collider(Vector4 &BasePosition);
	Collision collision;

	static std::vector<Collider*> colliders;
	static void AddToColliders(Collider* collider);

};

class SphereCollider : public Collider
{
public:
	SphereCollider(float radius, Vector4 &BasePosition);
	virtual ~SphereCollider();

	virtual bool IsColliding(SphereCollider &other);
	virtual bool IsColliding(AABBCollider &other);
	virtual bool IsColliding(OABBCollider &other);
	virtual bool IsColliding(Terrain &other);

	float radius;
};

class AABBCollider : public Collider
{
public:
	AABBCollider(float width, float height, float depth, Vector4 &BasePosition);
	virtual ~AABBCollider();

	virtual bool IsColliding(SphereCollider &other);
	virtual bool IsColliding(AABBCollider &other);
	virtual bool IsColliding(OABBCollider &other);
	virtual bool IsColliding(Terrain &other);

	float width, height, depth;
};

class OABBCollider : public Collider
{
public:
	OABBCollider(Vector4 &BasePosition);
	~OABBCollider();

	virtual bool IsColliding(SphereCollider &other);
	virtual bool IsColliding(AABBCollider &other);
	virtual bool IsColliding(OABBCollider &other);
	virtual bool IsColliding(Terrain &other);
};


