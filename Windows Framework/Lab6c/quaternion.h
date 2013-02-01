#pragma once
class Vector4;
class Vector3;


class Quaternion
{
public:
	Quaternion(void);
	Quaternion(float s, float x, float y, float z);
	Quaternion(const Vector4 & rhs);
	Quaternion(const Vector3 & rhs);
	Quaternion(float s, Vector3 rhs);
	Quaternion(float s, Vector4 rhs);
	~Quaternion(void);

	void Rotation(float angle, float xComp, float yComp, float zComp);
	void Rotation(float angle, Vector3 axis);
	void Align(Vector3 v1, Vector3 v2);


	float Modulus();
	float ModulusSqr();

	Quaternion Conjugate();
	Quaternion Inverse();
	Quaternion Normalize();
	void NormalizeSelf();
	void NormalizeVector();
	float GetAngle();
	Vector3 GetAxis();

	float s, x, y, z;
};

inline Quaternion operator + (const Quaternion &q1,
						   const Quaternion &q2)
{
	return Quaternion(q1.s + q2.s, q1.x + q2.x, q1.y + q2.y, q1.z + q2.z);
}

inline Quaternion operator - (const Quaternion &q1,
						   const Quaternion &q2)
{
	return Quaternion(q1.s - q2.s, q1.x - q2.x, q1.y - q2.y, q1.z - q2.z);
}

inline Quaternion operator * (const Quaternion &q1,
						   const Quaternion &q2)
{
	return Quaternion	( q1.s*q2.s+q1.x*-q2.x+q1.y*-q2.y+q1.z*-q2.z
						, q1.s*q2.x+q1.x*q2.s+q1.y*q2.z+q1.z*-q2.y
						, q1.s*q2.y+q1.x*-q2.z+q1.y*q2.s+q1.z*q2.x
						, q1.s*q2.z+q1.x*q2.y+q1.y*-q2.x+q1.z*q2.s);
}
inline Quaternion operator * (const Quaternion &q,
						   const float &c)
{
	return Quaternion(q.s*c, q.x*c, q.y*c, q.z*c);
}

inline Quaternion operator * (const float &c,
						   const Quaternion &q)
{
	return Quaternion(q.s*c, q.x*c, q.y*c, q.z*c);
}

inline Quaternion operator / (const Quaternion &q,
						   const float & c)
{
	return Quaternion(q.s/c, q.x/c, q.y/c, q.z/c);
}