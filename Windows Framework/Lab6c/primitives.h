#pragma once
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include "glew.h"
#include <gl/gl.h>
#include "tex.h"

class Cube
{
public:
	Cube(void);

	Cube(float OriginX, float OriginY, float OriginZ);
	Cube(float OriginX, float OriginY, float OriginZ, float Width, float Height, float Depth);
	Cube(float OriginX, float OriginY, float OriginZ, float Width, float Height, float Depth, const char *TextureFile);
	void Initialise(float OriginX, float OriginY, float OriginZ, float Width, float Height, float Depth, const char *TextureFile);
	void ApplyTexture(const char *TextureFile);
	~Cube(void);
	void Draw();
	GLuint texture;

private:
	void CreateCube(float OriginX, float OriginY, float OriginZ, float Width, float Height, float Depth);
	void BindTexture();
	float OriginX, OriginY, OriginZ, Width, Height, Depth;
	bool textured;
};

class CubeArray
{
public:
	CubeArray();
	CubeArray(float width, float height, float depth);
	CubeArray(float width, float height, float depth, const char *Texture);

	void Initialise(float width, float height, float depth);
	void ApplyTexture(const char *Texture);
	void GenerateList();
	void Draw();
	GLuint texture;

private:
	GLfloat verts[72];
	GLfloat normals[72];
	GLfloat texCoords[72];
	GLubyte indices[36];
	GLuint displayList;
};