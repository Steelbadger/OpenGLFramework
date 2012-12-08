#include "mesh.h"


Mesh::Mesh(void)
{
}


Mesh::~Mesh(void)
{
}

void Mesh::Create()
{
	float ybase = -0.5f;
	for (float i = -25; i < 25; i++) {
		for (float j = -25; j < 25; j++) {
			verts.push_back(-0.5f+i*1.0f);
			verts.push_back(ybase);
			verts.push_back(-0.5f+j*1.0f);

			verts.push_back(0.5f+i*1.0f);
			verts.push_back(ybase);
			verts.push_back(0.5f+j*1.0f);

			verts.push_back(-0.5f+i*1.0f);
			verts.push_back(ybase);
			verts.push_back(0.5f+j*1.0f);

			verts.push_back(0.5f+i*1.0f);
			verts.push_back(ybase);
			verts.push_back(0.5f+j*1.0f);

			verts.push_back(-0.5f+i*1.0f);
			verts.push_back(ybase);
			verts.push_back(-0.5f+j*1.0f);

			verts.push_back(0.5f+i*1.0f);
			verts.push_back(ybase);
			verts.push_back(-0.5f+j*1.0f);
		}
	}

	for (int i = 0; i < 15000 ; i++) {
		normals.push_back(0.0f);
		normals.push_back(1.0f);
		normals.push_back(0.0f);
	}

	for (int i = 0; i < 2500; i++) {
		texCoords.push_back(0.0f);
		texCoords.push_back(1.0f);

		texCoords.push_back(1.0f);
		texCoords.push_back(0.0f);

		texCoords.push_back(0.0f);
		texCoords.push_back(0.0f);

		texCoords.push_back(1.0f);
		texCoords.push_back(0.0f);

		texCoords.push_back(0.0f);
		texCoords.push_back(1.0f);

		texCoords.push_back(1.0f);
		texCoords.push_back(1.0f);
	}
	CreateGLTexture("tiles.tga", texture);
}

void Mesh::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, texture);
	glVertexPointer(3, GL_FLOAT, 0, &verts[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glTexCoordPointer(2,GL_FLOAT,0,&texCoords[0]);
	glDrawArrays(GL_TRIANGLES, 0, 15000);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
