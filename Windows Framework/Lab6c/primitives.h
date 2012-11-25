#pragma once
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <gl/gl.h>
#include "tex.h"

class Cube
{
public:
	Cube(void);

	Cube(float OriginX, float OriginY, float OriginZ);
	Cube(float OriginX, float OriginY, float OriginZ, float Width, float Height, float Depth);
	Cube(float OriginX, float OriginY, float OriginZ, float Width, float Height, float Depth, char *TextureFile);
	void Initialise(float OriginX, float OriginY, float OriginZ, float Width, float Height, float Depth, char *TextureFile);
	void ApplyTexture(char *TextureFile);
	~Cube(void);
	void Draw();
	GLuint texture;

private:
	void CreateCube(float OriginX, float OriginY, float OriginZ, float Width, float Height, float Depth);
	void BindTexture();
	float OriginX, OriginY, OriginZ, Width, Height, Depth;
	bool textured;
};
