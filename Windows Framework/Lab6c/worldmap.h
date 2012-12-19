#pragma once
#include <windows.h>
#include "myvector2.h"
class WorldMap
{
public:
	WorldMap(void);
	~WorldMap(void);

	void MoveAroundMap(float x, float z);

private:
	Vector2 currentGridLoc;
	static const int MAXSIZE = 1000;
	BYTE largeHeightmap[MAXSIZE][MAXSIZE];
	BYTE q1HeightModder[MAXSIZE][MAXSIZE];
	BYTE q2HeightModder[MAXSIZE][MAXSIZE];
	BYTE q3HeightModder[MAXSIZE][MAXSIZE];
	BYTE q4HeightModder[MAXSIZE][MAXSIZE];
};

