#pragma once

class Vector4;

class Vector3
{
public:
	Vector3(void);
	Vector3(const float x, const float y, const float z);
	Vector3(const Vector3 & rhs);
	Vector3(const Vector4 & rhs);
	~Vector3(void);


	bool operator==(const Vector3 & rhs) const;
	Vector3 & operator+=(const Vector3 & rhs);

	float x, y, z;
};

