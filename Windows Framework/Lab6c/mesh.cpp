#include "mesh.h"
#include "heightmap.h"
#include "myvector4.h"
#include <iostream>


Mesh::Mesh(void):
	heightmap(8)
{
}


Mesh::~Mesh(void)
{
}

void Mesh::Create()
{
	float ybase = -0.5f;
	float xbase = -0.5f;

	float height = 0;

	height = heightmap.GetFloatHeight(0.0f, 0.0f);
	height = heightmap.GetFloatHeight(1.0f, 1.0f);
	height = heightmap.GetFloatHeight(0.236f, 0.314f);
	height = heightmap.GetFloatHeight(0.5f, 0.5f);
	height = heightmap.GetFloatHeight(0.0052f, 0.0021f);

	int size = heightmap.GetSize();

	for (float i = 0; i < size-1; i++) {
		for (float j = 0; j < size-1; j++) {
			verts.push_back(Vector3(-0.5f + i*1.0f, heightmap.GetHeight(i, j), -0.5f + j*1.0f));
			verts.push_back(Vector3(0.5f + i*1.0f, heightmap.GetHeight(1+i, 1+j), 0.5f + j*1.0f));
			verts.push_back(Vector3(-0.5f + i*1.0f, heightmap.GetHeight(i, 1+j), 0.5f + j*1.0f));
			verts.push_back(Vector3(0.5f + i*1.0f, heightmap.GetHeight(1+i, 1+j), 0.5f + j*1.0f));
			verts.push_back(Vector3(-0.5f + i*1.0f, heightmap.GetHeight(i, j),-0.5f + j*1.0f));
			verts.push_back(Vector3(0.5f + i*1.0f, heightmap.GetHeight(1+i, j), -0.5f + j*1.0f));
		}
	}

	for (float i = 0; i < size-1; i++) {
		for (float j = 0; j < size-1; j++) {
			for (int k = 0; k < 3 ; k++) {
				normals.push_back(CalcNormal(Vector3(-0.5f + i*1.0f, heightmap.GetHeight(i, j), -0.5f + j*1.0f)
											,Vector3(0.5f + i*1.0f, heightmap.GetHeight(1+i, 1+j), 0.5f + j*1.0f)
											,Vector3(-0.5f + i*1.0f, heightmap.GetHeight(i, 1+j), 0.5f + j*1.0f)));
			}
			for (int k = 0; k < 3 ; k++) {
				normals.push_back(CalcNormal(Vector3(0.5f + i*1.0f, heightmap.GetHeight(1+i, 1+j), 0.5f + j*1.0f)
											,Vector3(-0.5f + i*1.0f, heightmap.GetHeight(i, j),-0.5f + j*1.0f)
											,Vector3(0.5f + i*1.0f, heightmap.GetHeight(1+i, j), -0.5f + j*1.0f)));
			}
		}
	}

	SmoothNormals();

	for (int i = 0; i < size*size; i++) {
		texCoords.push_back(Vector2(0.0f, 1.0f));
		texCoords.push_back(Vector2(1.0f, 0.0f));
		texCoords.push_back(Vector2(0.0f, 0.0f));
		texCoords.push_back(Vector2(1.0f, 0.0f));
		texCoords.push_back(Vector2(0.0f, 1.0f));
		texCoords.push_back(Vector2(1.0f, 1.0f));
	}
	CreateGLTexture("tiles.tga", texture);
	int numVerts = size*size*6;

	displayList = glGenLists(1);

	glNewList(displayList,GL_COMPILE);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, texture);
		glVertexPointer(3, GL_FLOAT, 0, &verts[0]);
		glNormalPointer(GL_FLOAT, 0, &normals[0]);
		glTexCoordPointer(2,GL_FLOAT,0,&texCoords[0]);
		glDrawArrays(GL_TRIANGLES, 0, numVerts);
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

void Mesh::SmoothNormals()
{
	std::vector<Vector3> smoothedNormals;
	std::vector<bool> checkedMap;
	std::vector<Vector3> knownNormals;

	checkedMap.resize(verts.size());
	knownNormals.resize(verts.size());
	smoothedNormals.resize(verts.size());

	int counter = 0;

	for (int i = 0; i < checkedMap.size(); i++) {
		checkedMap[i] = false;
	}

	for (int i = 0; i < verts.size(); i++) {
		if (checkedMap[i] == false) {
			Vector3 searchNode = verts[i];
			std::vector<int> found;
			Vector4 averagedNode = Vector3(0.0f, 0.0f, 0.0f);
			for (int j = 0; j < verts.size(); j++) {
				if (verts[j] == searchNode) {
					found.push_back(j);
					checkedMap[j] = true;
					averagedNode += Vector4(normals[j]);
				}
			}
			counter++;
			averagedNode /= found.size();
			for (int j = 0; j < found.size(); j++) {
				smoothedNormals[found[j]] = averagedNode;
			}
		}
	}

	normals = smoothedNormals;
	std::cout << "Unsmoothed " << verts.size() << " normals." << std::endl;
	std::cout << "Smoothed " << counter << " normals." << std::endl;

}
