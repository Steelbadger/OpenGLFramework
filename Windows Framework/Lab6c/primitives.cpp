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

Cube::Cube(float x, float y, float z, float w, float h, float d, const char *TextureFile)
{
	Initialise(x, y, z, w, h, d, TextureFile);
}

void Cube::Initialise(float x, float y, float z, float w, float h, float d, const char *TextureFile)
{
	CreateGLTexture(TextureFile, texture);
	textured = true;
	CreateCube(x, y, z, w, h, d);
}

void Cube::ApplyTexture(const char *TextureFile)
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

CubeArray::CubeArray()
{
	Initialise(1.0f, 1.0f, 1.0f);
}

CubeArray::CubeArray(float x, float y, float z)
{
	Initialise(x, y, z);
}

CubeArray::CubeArray(float x, float y, float z, const char *Tex)
{
	Initialise(x, y, z);
	ApplyTexture(Tex);
}

void CubeArray::ApplyTexture(const char *TextureFile)
{
	CreateGLTexture(TextureFile, texture);
}

void CubeArray::Initialise(float width, float height, float depth)
{
	GLfloat vertices[]={ width/2, height/2, depth/2,  -width/2, height/2, depth/2,  -width/2,-height/2, depth/2,   width/2,-height/2, depth/2,   // v0,v1,v2,v3 (front)
						 width/2, height/2, depth/2,   width/2,-height/2, depth/2,   width/2,-height/2,-depth/2,   width/2, height/2,-depth/2,   // v0,v3,v4,v5 (right)
						 width/2, height/2, depth/2,   width/2, height/2,-depth/2,  -width/2, height/2,-depth/2,  -width/2, height/2, depth/2,   // v0,v5,v6,v1 (top)
						-width/2, height/2, depth/2,  -width/2, height/2,-depth/2,  -width/2,-height/2,-depth/2,  -width/2,-height/2, depth/2,   // v1,v6,v7,v2 (left)
						-width/2,-height/2,-depth/2,   width/2,-height/2,-depth/2,   width/2,-height/2, depth/2,  -width/2,-height/2, depth/2,   // v7,v4,v3,v2 (bottom)
						 width/2,-height/2,-depth/2,  -width/2,-height/2,-depth/2,  -width/2, height/2,-depth/2,   width/2, height/2,-depth/2 }; // v4,v7,v6,v5 (back)

	GLfloat normalsTemp[]={ 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   // v0,v1,v2,v3 (front)
							1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   // v0,v3,v4,v5 (right)
							0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,   // v0,v5,v6,v1 (top)
						   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,   // v1,v6,v7,v2 (left)
							0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,   // v7,v4,v3,v2 (bottom)
							0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1 }; // v4,v7,v6,v5 (back)

	GLfloat texTemp[] =	  { 1, 1,  0, 1,  0, 0,  1, 0,   // v0,v1,v2,v3 (front)
							0, 1,  0, 0,  1, 0,  1, 1,   // v0,v3,v4,v5 (right)
							1, 0,  1, 1,  0, 1,  0, 0,   // v0,v5,v6,v1 (top)
						    1, 1,  0, 1,  0, 0,  1, 0,   // v1,v6,v7,v2 (left)
							0, 0,  1, 0,  1, 1,  0, 1,   // v7,v4,v3,v2 (bottom)
							0, 0,  1, 0,  1, 1,  0, 1 }; // v4,v7,v6,v5 (back)


	GLubyte indicesTemp[]  =  { 0, 1, 2,   2, 3, 0,      // front
								4, 5, 6,   6, 7, 4,      // right
								8, 9,10,  10,11, 8,      // top
								12,13,14,  14,15,12,      // left
								16,17,18,  18,19,16,      // bottom
								20,21,22,  22,23,20 };    // back

	for (int i = 0; i < 72; i++) {
		verts[i] = vertices[i];
		normals[i] = normalsTemp[i];
	}
	for (int i = 0; i < 36; i++) {
		indices[i] = indicesTemp[i];
	}
	for (int i = 0; i < 48; i++) {
		texCoords[i] = texTemp[i];
	}

}

void CubeArray::Draw()
{
	// activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindTexture(GL_TEXTURE_2D, texture);
	glVertexPointer(3, GL_FLOAT, 0, verts);
	glNormalPointer(GL_FLOAT, 0, normals);
	glTexCoordPointer(2,GL_FLOAT,0,texCoords);

	// draw a cube
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glBindTexture(GL_TEXTURE_2D, NULL);

}
