#include "myvector3.h"
#include "myvector4.h"


Vector3::Vector3(void):
	x(0),y(0),z(0)
{
}
Vector3::Vector3(const float _x, const float _y, const float _z)
	:x(_x),y(_y),z(_z)
{
}
Vector3::Vector3(const Vector3 &rhs):
	x(rhs.x), y(rhs.y), z(rhs.z)
{

}
Vector3::Vector3(const Vector4 &rhs):
	x(rhs.x/rhs.w), y(rhs.y/rhs.w), z(rhs.z/rhs.w)
{

}


Vector3::~Vector3(void)
{
}