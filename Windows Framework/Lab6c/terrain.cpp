#include "Terrain.h"
#include "heightmap.h"
#include "myvector4.h"
#include "my4x4matrix.h"
#include <iostream>


Terrain::Terrain(double s):
	heightmap(9),
		squareSize(s)
{
}


Terrain::~Terrain(void)
{
}

void Terrain::Create()
{
	heightmap.Initialise();

	float height = 0;

	int size = heightmap.GetSize();

	float step = squareSize/(size-1);

	Matrix4x4 scaleMatrix(Matrix4x4::IDENTITY);
	scaleMatrix.Scale(1/step, 1.0f, 1/step);

	for (float i = 0; i < size-1; i++) {
		for (float j = 0; j < size-1; j++) {
			verts.push_back(Vector3(i*step, heightmap.GetHeight(i, j), j*step));
			verts.push_back(Vector3(i*step, heightmap.GetHeight(i, 1+j), step + j*step));
			verts.push_back(Vector3(step + i*step, heightmap.GetHeight(1+i, 1+j), step + j*step));
			verts.push_back(Vector3(step + i*step, heightmap.GetHeight(1+i, 1+j), step + j*step));
			verts.push_back(Vector3(step + i*step, heightmap.GetHeight(1+i, j), j*step));
			verts.push_back(Vector3(i*step, heightmap.GetHeight(i, j),j*step));

			Vector4 normalA, normalB, normalC, normalD;

			normalA = heightmap.GetNormal(i,j);
			normalB = heightmap.GetNormal(1+i, 1+j);
			normalC = heightmap.GetNormal(i, 1+j);
			normalD = heightmap.GetNormal(1+i, j);

			normalA = scaleMatrix * normalA;
			normalB = scaleMatrix * normalB;
			normalC = scaleMatrix * normalC;
			normalD = scaleMatrix * normalD;

			normalA.NormaliseSelf();
			normalB.NormaliseSelf();
			normalC.NormaliseSelf();
			normalD.NormaliseSelf();

			normals.push_back(normalA);
			normals.push_back(normalC);
			normals.push_back(normalB);
			normals.push_back(normalB);
			normals.push_back(normalD);
			normals.push_back(normalA);
		}
	}

	for (int i = 0; i < size*size; i++) {
		texCoords.push_back(Vector2(0.0f, 1.0f));
		texCoords.push_back(Vector2(1.0f, 0.0f));
		texCoords.push_back(Vector2(0.0f, 0.0f));
		texCoords.push_back(Vector2(1.0f, 0.0f));
		texCoords.push_back(Vector2(0.0f, 1.0f));
		texCoords.push_back(Vector2(1.0f, 1.0f));
	}

	GLuint tex;
	CreateGLTexture("grass.tga", tex);

	texture = tex;

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

float Terrain::GetHeight(float x, float z)
{
	int size = heightmap.GetSize();
	float step = squareSize/(size-1);

	return (heightmap.GetFloatHeight(x/step,z/step));
}

void Terrain::CreateFromSource(std::vector<Vector3> &vertices)
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

void Terrain::Draw()
{
	glCallList(displayList);
}

void Terrain::SetTexture(GLuint t)
{
	texture = t;
}

Vector3 Terrain::CalcNormal(Vector3 pointA, Vector3 pointB, Vector3 pointC)
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

void Terrain::SmoothNormals()
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
