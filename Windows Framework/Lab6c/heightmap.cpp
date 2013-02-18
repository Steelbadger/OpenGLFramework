#include "heightmap.h"
#include "myvector3.h"
#include "glew.h"
#include <gl\GL.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "tex.h"


//struct pixel
//{
//	unsigned char channels[4];
//};


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
	float max = n.amplitude;
	n.octaves = 8;
	int counter = 0;
	int currentpixel = 0;
	float step = 1500/((1500/0.75)-1);
	step = float(1000.0/size);
	float height;
	Vector3 normal;
	for (float i = 0; i < size; i++) {
		for (float j = 0; j < size; j++) {
			height = noise.FractalSimplex(i*step, j*step, n);
			normal = noise.FractalSimplexNormal(i*step, j*step, n, step);
			map[currentpixel] = unsigned char((normal.x+1)/2 * 255);			//  R
			map[currentpixel + 1] = unsigned char((normal.y+1)/2 * 255);		//  G
			map[currentpixel + 2] = unsigned char((normal.z+1)/2 * 255);		//  B
			map[currentpixel + 3] = unsigned char(((height+max)/(2*max)) * 255);		// A

			counter++;
			currentpixel = counter*4;
		}
	}

	GLuint TexID;

	glGenTextures(1, &TexID);				// Create The Texture
	glBindTexture(GL_TEXTURE_2D, TexID);

//	glTexStorage2D(GL_TEXTURE_2D, 8, 4, GL_RGBA, size);
//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size, GL_RGBA, GL_UNSIGNED_BYTE, map);


	glTexImage2D(GL_TEXTURE_2D, 0, 4, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, map);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

//	write_tga("output.tga", size, map);

	if (map)						// If Texture Image Exists ( CHANGE )
	{
		free(map);					// Free The Texture Image Memory ( CHANGE )
	}

//	CreateGLTexture("output.tga", TexID);


	return TexID;
}

void Heightmap::write_tga(const char *filename, int size, unsigned char* base)
{
	std::ofstream o(filename, std::ios::out | std::ios::binary);

	//Write the header
	o.put(0);
   	o.put(0);
   	o.put(2);					/* uncompressed RGB */
   	o.put(0);	o.put(0);
   	o.put(0); 	o.put(0);
   	o.put(0);
   	o.put(0); 	o.put(0);		/* X origin */
   	o.put(0); 	o.put(0);		/* y origin */
   	o.put((size & 0x00FF));
   	o.put((size & 0xFF00) / 256);
   	o.put((size & 0x00FF));
   	o.put((size & 0xFF00) / 256);
   	o.put(32);					/* 32 bit bitmap */
   	o.put(0);
   	
	//Write the pixel data
	for (int i=0;i<size*size;i++) {
		o.put(base[i*4]);
		o.put(base[i*4+1]);
		o.put(base[i*4+2]);
		o.put(base[i*4+3]);
	}   
	
	//close the file
	o.close();

}