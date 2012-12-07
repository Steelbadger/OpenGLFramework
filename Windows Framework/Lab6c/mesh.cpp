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
	for (float i = -50; i < 50; i++) {
		for (float j = -50; j < 50; j++) {
			verts.push_back(-1.0f+i*2.0f);
			verts.push_back(ybase);
			verts.push_back(-1.0f+j*2.0f);

			verts.push_back(1.0f+i*2.0f);
			verts.push_back(ybase);
			verts.push_back(1.0f+j*2.0f);

			verts.push_back(-1.0f+i*2.0f);
			verts.push_back(ybase);
			verts.push_back(1.0f+j*2.0f);

			verts.push_back(1.0f+i*2.0f);
			verts.push_back(ybase);
			verts.push_back(1.0f+j*2.0f);

			verts.push_back(-1.0f+i*2.0f);
			verts.push_back(ybase);
			verts.push_back(-1.0f+j*2.0f);

			verts.push_back(1.0f+i*2.0f);
			verts.push_back(ybase);
			verts.push_back(-1.0f+j*2.0f);
		}
	}

	for (int i = 0; i < 60000 ; i++) {
		normals.push_back(0.0f);
		normals.push_back(1.0f);
		normals.push_back(0.0f);
	}
}

void Mesh::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &verts[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glDrawArrays(GL_TRIANGLES, 0, 60000);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
