#include "mesh.h"
#include "heightmap.h"
#include "myvector4.h"


Mesh::Mesh(void)
{
}


Mesh::~Mesh(void)
{
}

void Mesh::Create()
{
	Heightmap heightmap = Heightmap(8);

	float ybase = -0.5f;
	float xbase = -0.5f;
	//for (float i = -25; i < 25; i++) {
	//	for (float j = -25; j < 25; j++) {
	//		verts.push_back(Vector3(-0.5f + i*1.0f, ybase, -0.5f + j*1.0f));
	//		verts.push_back(Vector3(0.5f + i*1.0f, ybase, 0.5f + j*1.0f));
	//		verts.push_back(Vector3(-0.5f + i*1.0f, ybase, 0.5f + j*1.0f));
	//		verts.push_back(Vector3(0.5f + i*1.0f, ybase, 0.5f + j*1.0f));
	//		verts.push_back(Vector3(-0.5f + i*1.0f, ybase,-0.5f + j*1.0f));
	//		verts.push_back(Vector3(0.5f + i*1.0f, ybase, -0.5f + j*1.0f));
	//	}
	//}

	for (float i = -25; i < 25; i++) {
		for (float j = -25; j < 25; j++) {
			verts.push_back(Vector3(-0.5f + i*1.0f, heightmap.GetHeight(i+25, j+25), -0.5f + j*1.0f));
			verts.push_back(Vector3(0.5f + i*1.0f, heightmap.GetHeight(1+i+25, 1+j+25), 0.5f + j*1.0f));
			verts.push_back(Vector3(-0.5f + i*1.0f, heightmap.GetHeight(i+25, 1+j+25), 0.5f + j*1.0f));
			verts.push_back(Vector3(0.5f + i*1.0f, heightmap.GetHeight(1+i+25, 1+j+25), 0.5f + j*1.0f));
			verts.push_back(Vector3(-0.5f + i*1.0f, heightmap.GetHeight(i+25, j+25),-0.5f + j*1.0f));
			verts.push_back(Vector3(0.5f + i*1.0f, heightmap.GetHeight(1+i+25, j+25), -0.5f + j*1.0f));
		}
	}


	//for (int i = 0; i < 15000 ; i++) {
	//	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
	//}
	for (float i = -25; i < 25; i++) {
		for (float j = -25; j < 25; j++) {
			for (int i = 0; i < 3 ; i++) {
				normals.push_back(CalcNormal(Vector3(-0.5f + i*1.0f, heightmap.GetHeight(i+25, j+25), -0.5f + j*1.0f)
											,Vector3(0.5f + i*1.0f, heightmap.GetHeight(1+i+25, 1+j+25), 0.5f + j*1.0f)
											,Vector3(-0.5f + i*1.0f, heightmap.GetHeight(i+25, 1+j+25), 0.5f + j*1.0f)));
				normals.push_back(CalcNormal(Vector3(-0.5f + i*1.0f, heightmap.GetHeight(i+25, j+25),-0.5f + j*1.0f)		
											,Vector3(0.5f + i*1.0f, heightmap.GetHeight(1+i+25, j+25), -0.5f + j*1.0f)
											,Vector3(0.5f + i*1.0f, heightmap.GetHeight(1+i+25, 1+j+25), 0.5f + j*1.0f)));

			}
		}
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

Vector3 Mesh::CalcNormal(Vector3 pointA, Vector3 pointB, Vector3 pointC)
{
	Vector4 A(pointA);
	Vector4 B(pointB);
	Vector4 C(pointC);

	Vector4 AB = B - A;
	Vector4 AC = C - A;

	Vector4 Normal = AC.Cross(AB);
	Normal.NormaliseSelf();
	Vector3 output(Normal);

	return output;

}
