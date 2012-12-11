#pragma once
#include "myvector4.h"

class AABBCollider;
class OABBCollider;
class SphereCollider;

class Collider
{
public:
	Collider(Vector4 &BasePosition);

	virtual bool IsColliding(SphereCollider &other) = 0;
	virtual bool IsColliding(AABBCollider &other) = 0;
	virtual bool IsColliding(OABBCollider &other) = 0;

	Vector4 *basePosition;
protected:
	~Collider(void);

};

class SphereCollider : public Collider
{
public:
	SphereCollider(float radius, Vector4 &BasePosition);
	~SphereCollider();

	virtual bool IsColliding(SphereCollider &other);
	virtual bool IsColliding(AABBCollider &other);
	virtual bool IsColliding(OABBCollider &other);

	float radius;
};

class AABBCollider : public Collider
{
public:
	AABBCollider(float width, float height, float depth, Vector4 &BasePosition);
	~AABBCollider();

	virtual bool IsColliding(SphereCollider &other);
	virtual bool IsColliding(AABBCollider &other);
	virtual bool IsColliding(OABBCollider &other);

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
};

