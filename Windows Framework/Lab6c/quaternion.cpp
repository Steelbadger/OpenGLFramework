#include "quaternion.h"
#include "myvector3.h";
#include "myvector4.h"
#include <math.h>


Quaternion::Quaternion(void):
s(1), x(0), y(0), z(0)
{
}


Quaternion::~Quaternion(void)
{
}

Quaternion::Quaternion(float s_, float x_, float y_, float z_):
s(s_), x(x_), y(y_), z(z_)
{
}

Quaternion::Quaternion(const Vector4 & rhs):
s(0), x(rhs.x), y(rhs.y), z(rhs.z)
{
}

Quaternion::Quaternion(const Vector3 & rhs):
s(0), x(rhs.x), y(rhs.y), z(rhs.z)
{
}

Quaternion::Quaternion(float s_, Vector3 rhs):
s(s_), x(rhs.x), y(rhs.y), z(rhs.z)
{
}

Quaternion::Quaternion(float s_, Vector4 rhs):
s(s_), x(rhs.x), y(rhs.y), z(rhs.z)
{
}

void Quaternion::Rotation(float angle, float xComp, float yComp, float zComp)
{
	Quaternion I(0.0f, xComp, yComp, zComp);
	I.NormalizeSelf();
	s = cos(angle/2);
	float vs = sin(angle/2);
	x = I.x*vs;
	y = I.y*vs;
	z = I.z*vs;
}

void Quaternion::Rotation(float angle, Vector3 axis)
{
	Rotation(angle, axis.x, axis.y, axis.z);
}

float Quaternion::Modulus()
{
	return sqrt(s*s+x*x+y*y+z*z);
}

float Quaternion::ModulusSqr()
{
	return (s*s+x*x+y*y+z*z);
}

Quaternion Quaternion::Conjugate()
{
	return Quaternion(s, -x, -y, -z);
}

Quaternion Quaternion::Inverse()
{
	if (ModulusSqr() == 1.0) {
		return Conjugate();
	} else {
		return Conjugate()/Modulus();
	}
}

void Quaternion::NormalizeSelf()
{
	if (ModulusSqr() != 1.0) {
		float l = Modulus();
		s /= l;
		x /= l;
		y /= l;
		z /= l;
	}
}

Quaternion Quaternion::Normalize()
{
	if (ModulusSqr() == 1.0) {
		return Quaternion(s, x, y, z);
	} else {
		float l = Modulus();
		return Quaternion(s/l, x/l, y/l, z/l);
	}
}

void Quaternion::NormalizeVector()
{
	Quaternion I(0.0f, x, y, z);
	I.NormalizeSelf();
	x = I.x;
	y = I.y;
	z = I.z;
}