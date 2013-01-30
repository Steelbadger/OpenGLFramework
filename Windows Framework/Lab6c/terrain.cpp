#include "Terrain.h"
#include "heightmap.h"
#include "myvector4.h"
#include "my4x4matrix.h"
#include <iostream>
#include "noisegenerator.h"


Terrain::Terrain(float s, NoiseObject n, float r):
		squareSize(s),
			resolution(r)
{
	Create(n);
}


Terrain::~Terrain(void)
{
}

void Terrain::Create(NoiseObject n)
{

	float height = 0;

	NoiseGenerator noise;
	int size = squareSize/resolution;
	float step = squareSize/(size-1);
	Matrix4x4 scaleMatrix(Matrix4x4::IDENTITY);
	scaleMatrix.Scale(1/step, 1.0f, 1/step);
	int octaves = n.octaves;
	float zoom = n.zoom;
	float persistance = n.persistance;
	float amp = n.amplitude;

	for (float i = 0; i < size-1; i++) {
		for (float j = 0; j < size-1; j++) {
			verts.push_back(Vector3(i*step, noise.Perlin2D(i*step, j*step, octaves, zoom, persistance, amp), j*step));
			verts.push_back(Vector3(i*step, noise.Perlin2D(i*step, step+j*step, octaves, zoom, persistance, amp), step + j*step));
			verts.push_back(Vector3(step + i*step, noise.Perlin2D(step+i*step, step+j*step, octaves, zoom, persistance, amp), step + j*step));
			verts.push_back(Vector3(step + i*step, noise.Perlin2D(step+i*step, step+j*step, octaves, zoom, persistance, amp), step + j*step));
			verts.push_back(Vector3(step + i*step, noise.Perlin2D(step+i*step, j*step, octaves, zoom, persistance, amp), j*step));
			verts.push_back(Vector3(i*step, noise.Perlin2D(i*step, j*step, octaves, zoom, persistance, amp),j*step));

			Vector4 normalA, normalB, normalC, normalD;
			normalA = noise.NormalToPerlin2D(i*step, j*step, octaves, zoom, persistance, amp);
			normalB = noise.NormalToPerlin2D(step+i*step, step+j*step, octaves, zoom, persistance, amp);
			normalC = noise.NormalToPerlin2D(i*step, step+j*step, octaves, zoom, persistance, amp);
			normalD = noise.NormalToPerlin2D(step+i*step, j*step, octaves, zoom, persistance, amp);

			normals.push_back(normalA);
			normals.push_back(normalC);
			normals.push_back(normalB);
			normals.push_back(normalB);
			normals.push_back(normalD);
			normals.push_back(normalA);

			texCoords.push_back(Vector2(i*step, j*step));
			texCoords.push_back(Vector2(i*step, step + j*step));
			texCoords.push_back(Vector2(step + i*step, step + j*step));
			texCoords.push_back(Vector2(step + i*step, step + j*step));
			texCoords.push_back(Vector2(step + i*step, j*step));
			texCoords.push_back(Vector2(i*step, j*step));
		}
	}

	//for (int i = 0; i < size*size; i++) {
	//	texCoords.push_back(Vector2(0.0f, 1.0f));
	//	texCoords.push_back(Vector2(1.0f, 0.0f));
	//	texCoords.push_back(Vector2(0.0f, 0.0f));
	//	texCoords.push_back(Vector2(1.0f, 0.0f));
	//	texCoords.push_back(Vector2(0.0f, 1.0f));
	//	texCoords.push_back(Vector2(1.0f, 1.0f));
	//}

	textureFile = "grass.tga";


	std::cout << "Number of Triangles in Terrain Mesh: " << verts.size()/3 << std::endl;
}

float Terrain::GetHeight(float x, float z)
{
//	int size = heightmap.GetSize();
//	float step = squareSize/(size-1);

//	return (heightmap.GetFloatHeight(x/step,z/step));
	return 0.0f;
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