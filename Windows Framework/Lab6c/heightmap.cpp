#include "heightmap.h"
#include "myvector3.h"
#include "glew.h"
#include <gl\GL.h>
#include <stdio.h>
#include <stdlib.h>


struct pixel
{
	unsigned char channels[4];
};


Heightmap::Heightmap(void)
{
}


Heightmap::~Heightmap(void)
{
}


unsigned int Heightmap::GenerateHeightmap(float x, float y, NoiseObject n)
{
	const int size = 2048;
//	unsigned char map[size][size][4];
	unsigned char* map = (GLubyte *)malloc(size*size*4);	
	NoiseGenerator noise;
	float max = n.amplitude;
	GLuint currentByte = 0;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			float height = noise.TurbulentPerlin2D(x+i, y+j, n);
			Vector3 normal = noise.NormalToTurbulentPerlin2D(x+i, y+j, n, 0.25);

			map[currentByte] = unsigned char(normal.x * 255);
			map[currentByte + 1] = unsigned char(normal.y * 255);
			map[currentByte + 2] = unsigned char(normal.z * 255);
			map[currentByte + 3] = unsigned char((height/max) * 255);

			currentByte += 4;
		}
	}

	GLuint TexID;

	glGenTextures(1, &TexID);				// Create The Texture ( CHANGE )
	glBindTexture(GL_TEXTURE_2D, TexID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, map);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	if (map)						// If Texture Image Exists ( CHANGE )
	{
		free(map);					// Free The Texture Image Memory ( CHANGE )
	}


	return TexID;
}
