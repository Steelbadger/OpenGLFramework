#include "Terrain.h"
#include "myvector4.h"
#include "my4x4matrix.h"
#include <iostream>
#include "noisegenerator.h"
#include <time.h>
#include <string>


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
	noise.Seed(n.seed);
	int size = squareSize/resolution;
	float step = squareSize/(size-1);

	double myTime = clock();

	for (float i = 0; i < size; i++) {
		for (float j = 0; j < size; j++) {
			verts.push_back(Vector3(i*step, noise.FractalSimplex(i*step, j*step, n)+15.0, j*step));
			Vector3 normalA = noise.FractalSimplexNormal(i*step, j*step, n, step);

			normals.push_back(normalA);

			texCoords.push_back(Vector2(i*step/4, j*step/4));
		}
	}

	for (int i = 0; i < size-1; i++) {
		for (int j = 0; j < size-1; j++) {
			index.push_back(i+j*size);
			index.push_back(1+i+(1+j)*size);
			index.push_back(i+(1+j)*size);
			index.push_back(1+i+(1+j)*size);
			index.push_back(i+j*size);
			index.push_back(1+i+j*size);
		}
	}

	myTime = clock()-myTime;
	textureFile = "grass.tga";
	rockFile = "rockTexture.tga";


	std::cout << "Number of Triangles in Terrain Mesh: " << index.size()/3 << std::endl;
	std::cout << "Generation Time: " << myTime/CLOCKS_PER_SEC << "s" << std::endl;
}

void Terrain::AttachShader(std::string shader)
{
	std::string type = shader.substr(shader.find_last_of(".") + 1);

	if (type == "vertexshader" || type == "fragmentshader" || type == "tesscontrol" || type == "tessevaluation") {
		shaders.push_back(shader);
	} else {
		//  If the file extension is not correct then return an error and stop
		std::cout << "Cannot Attach Shader, Unrecognised Shader File Extension: " << type << std::endl;
		return;
	}
}
