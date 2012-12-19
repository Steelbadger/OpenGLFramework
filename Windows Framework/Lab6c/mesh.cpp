#include "mesh.h"
#include "heightmap.h"


Mesh::Mesh(void)
{
}


Mesh::~Mesh(void)
{
}

void Mesh::Create()
{
	Heightmap myHeights;
	float ybase = -0.5f;
	float xbase = -0.5f;
	for (float i = -25; i < 25; i++) {
		for (float j = -25; j < 25; j++) {
			verts.push_back(Vector3(-0.5f + i*1.0f, ybase, -0.5f + j*1.0f));
			verts.push_back(Vector3(0.5f + i*1.0f, ybase, 0.5f + j*1.0f));
			verts.push_back(Vector3(-0.5f + i*1.0f, ybase, 0.5f + j*1.0f));
			verts.push_back(Vector3(0.5f + i*1.0f, ybase, 0.5f + j*1.0f));
			verts.push_back(Vector3(-0.5f + i*1.0f, ybase,-0.5f + j*1.0f));
			verts.push_back(Vector3(0.5f + i*1.0f, ybase, -0.5f + j*1.0f));
		}
	}

	for (int i = 0; i < 15000 ; i++) {
		normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
	}

	for (int i = 0; i < 2500; i++) {
		texCoords.push_back(Vector2(0.0f, 1.0f));
		texCoords.push_back(Vector2(1.0f, 0.0f));
		texCoords.push_back(Vector2(0.0f, 0.0f));
		texCoords.push_back(Vector2(1.0f, 0.0f));
		texCoords.push_back(Vector2(0.0f, 1.0f));
		texCoords.push_back(Vector2(1.0f, 1.0f));
	}
	CreateGLTexture("tiles.tga", texture);

	displayList = glGenLists(1);

	glNewList(displayList,GL_COMPILE);
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
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEndList();

}

void Mesh::CreateFromSource(std::vector<Vector3> &vertices)
{
	verts = vertices;
	displayList = glGenLists(1);

	glNewList(displayList,GL_COMPILE);
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindTexture(GL_TEXTURE_2D, texture);
		glVertexPointer(3, GL_FLOAT, 0, &verts[0]);
		glDrawArrays(GL_TRIANGLES, 0, 15000);
		glDisableClientState(GL_VERTEX_ARRAY);
	glEndList();
}

void Mesh::Draw()
{
	glCallList(displayList);
}
