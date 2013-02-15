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
	const int size = 1024;
	GLubyte* map = (GLubyte *)malloc(size*size*4);	
	NoiseGenerator noise;
	float max = n.amplitude+3;
	n.octaves = 18;
	GLuint currentByte = 0;
	float step = 1500/((1500/0.75)-1);
	float height;
	Vector3 normal;
	for (float i = 0; i < size; i++) {
		for (float j = 0; j < size; j++) {
			height = noise.FractalSimplex(i*step, j*step, n);
			normal = noise.FractalSimplexNormal(i*step, j*step, n);
			map[currentByte] = unsigned char(normal.x * 255);
			map[currentByte + 1] = unsigned char(normal.y * 255);
			map[currentByte + 2] = unsigned char(normal.z * 255);
			map[currentByte + 3] = unsigned char(((height+max)/(2*max)) * 255);

			currentByte += 4;
		}
	}

	GLuint TexID;

	glGenTextures(1, &TexID);				// Create The Texture ( CHANGE )
	glBindTexture(GL_TEXTURE_2D, TexID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, map);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	write_tga("output.tga", size, map);

	if (map)						// If Texture Image Exists ( CHANGE )
	{
		free(map);					// Free The Texture Image Memory ( CHANGE )
	}


	return TexID;
}

void Heightmap::write_tga(const char *filename, int size, unsigned char* base)
{
	FILE *f = fopen(filename, "wb");
	if (f == NULL) {
		fprintf(stderr, "Cannot open %s\n", filename);
		exit(1);
	}

	unsigned char header[18] = {
		0, // no image ID
		0, // no colour map
		2, // uncompressed 24-bit image
		0, 0, 0, 0, 0, // empty colour map specification
		0, 0, // X origin
		0, 0, // Y origin
		size & 0xFF, (size >> 8) & 0xFF, // width
		size & 0xFF, (size >> 8) & 0xFF, // height
		24, // bits per pixel
		0, // image descriptor
	};
	fwrite(header, 1, 18, f);

	unsigned int currentByte = 0;

	for (int y = 0; y < size; ++y)
	{
		for (int x = 0; x < size; ++x)
		{
			unsigned char pixel[3] = {base[currentByte+3], base[currentByte+3], base[currentByte+3]};
//			unsigned char pixel[4] = {base[currentByte], base[currentByte+1], base[currentByte+2], base[currentByte+3]};
			currentByte +=4;
			fwrite(pixel, 1, 3, f);
		}
	}

	fclose(f);
}