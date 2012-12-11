#include "myvector2.h"


Vector2::Vector2(void)
{
}

Vector2::Vector2(float _u, float _v):
	u(_u), v(_v)
{
}

Vector2::Vector2(Vector2 &rhs):
	u(rhs.u),v(rhs.v)
{
}


Vector2::~Vector2(void)
{
}
