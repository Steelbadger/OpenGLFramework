#include "Primitives.h"
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "tex.h"

Cube::Cube(void)
{
	textured = false;
	CreateCube(0,0,0,1,1,1);
}

Cube::Cube(float x, float y, float z)
{
	textured = false;
	CreateCube(x, y, z, 1,1,1);
}

Cube::Cube(float x, float y, float z, float w, float h, float d)
{
	textured = false;
	CreateCube(x, y, z, w, h, d);
	
}

Cube::Cube(float x, float y, float z, float w, float h, float d, char *TextureFile)
{
	Initialise(x, y, z, w, h, d, TextureFile);
}

void Cube::Initialise(float x, float y, float z, float w, float h, float d, char *TextureFile)
{
	CreateGLTexture(TextureFile, texture);
	textured = true;
	CreateCube(x, y, z, w, h, d);
}

void Cube::ApplyTexture(char *TextureFile)
{
	CreateGLTexture(TextureFile, texture);
	textured = true;
}

Cube::~Cube(void)
{
}

void Cube::CreateCube(float x, float y, float z, float w, float h, float d)
{
	OriginX = x;
	OriginY = y;
	OriginZ = z;
	Width = w;
	Height = h;
	Depth = d;
}

void Cube::BindTexture()
{
	if (textured == true) {
		glBindTexture(GL_TEXTURE_2D, texture);
	} else {
		glBindTexture(GL_TEXTURE_2D, NULL);
	}
}

void Cube::Draw()
{
	glEnable(GL_TEXTURE_2D);
	BindTexture();
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
	glTranslatef(OriginX, OriginY, OriginZ);

	//  Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(	Width, Height, Depth);
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, Height, Depth);
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0,0, Depth);
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f( Width,0, Depth);
	//  Right Face
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f( Width, Height,0);
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f( Width, Height, Depth);
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f( Width,0, Depth);
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f( Width,0,0);
	//  Back Face
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, Height,0);
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f( Width, Height,0);
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f( Width,0,0);
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0,0,0);
	//  Left Face
		glNormal3f( -1.0f, 0.0f, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0, Height, Depth);
		glNormal3f( 0.0f, -1.0f, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, Height,0);
		glNormal3f( 0.0f, -1.0f, 0.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0,0,0);
		glNormal3f( 0.0f, -1.0f, 0.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0,0, Depth);
	//  Top Face
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f( Width, Height,0);
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, Height,0);
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, Height, Depth);
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f( Width, Height, Depth);
	//  Bottom Face
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0,0, Depth);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0,0,0);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f( Width,0,0);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f( Width,0, Depth);
	glPopMatrix();	
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);
}