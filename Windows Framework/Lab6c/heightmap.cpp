#include "heightmap.h"
#include "myvector3.h"
#include "glew.h"
#include <gl\GL.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "tex.h"
#include <process.h>
#include <iostream>


struct ThreadData
{
	ThreadData():n(12, 500.0f, 0.41f, 70.0f, -1563.155f){}
	ThreadData(GLushort* p, int baseX, int baseY, int size, float mbaseX, float mbaseY, float msize, int ssize, NoiseObject noise) :
		start(p), imageBaseX(baseX), imageBaseY(baseY), imageSize(size), mapBaseX(mbaseX), mapBaseY(mbaseY),	mapSize(msize),
			sectionSize(ssize),	n(noise) {}
	GLushort* start;
	int imageBaseX;
	int imageBaseY;
	int imageSize;
	float mapBaseX;
	float mapBaseY;
	float mapSize;
	int sectionSize;
	NoiseObject n;
};


Heightmap::Heightmap(void)
{
}


Heightmap::~Heightmap(void)
{
}


unsigned int Heightmap::GenerateHeightmap(float x, float y, NoiseObject n, float square)
{

	const int size = 2048;
	const int subdivs = 512;
	const int threads = size/subdivs;


	GLushort* map = (GLushort*)malloc(size*size*4*sizeof(GLushort));

	NoiseGenerator noise;
	float max = n.amplitude;
	int counter = 0;
	int currentpixel = 0;
	float step = 1000/((1000/0.75)-1);
	step = float(square/size);
	float height;
	Vector3 normal;

	ThreadData dataArray[threads][threads];
	HANDLE threadHandles[threads*threads];
	
	
	for(int i = 0; i < threads; i++) {
		for(int j = 0; j < threads; j++) {
			dataArray[i][j] = ThreadData(map, i*size/threads, j*size/threads, size, i*step*subdivs, j*step*subdivs, square, subdivs, n);
			threadHandles[j+i*threads] = (HANDLE) _beginthreadex(NULL,0,&Heightmap::GenerateSection,(void*)&dataArray[i][j],0,NULL);
		}
	}

	WaitForMultipleObjects(threads*threads,threadHandles,TRUE, INFINITE);

	GLuint TexID;

	glGenTextures(1, &TexID);				// Create The Texture
	glBindTexture(GL_TEXTURE_2D, TexID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, size, size, 0, GL_RGBA, GL_UNSIGNED_SHORT, map);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	if (map)						// If Texture Image Exists ( CHANGE )
	{
		free(map);					// Free The Texture Image Memory ( CHANGE )
	}

	return TexID;
}

unsigned __stdcall Heightmap::GenerateSection(void *data)
{
	ThreadData args = *(ThreadData*)data;

	float height = 0;
	Vector3 normal(0,0,0);

	NoiseGenerator noise;
	float step = args.mapSize/args.imageSize;
	float x = args.mapBaseX;
	float y = args.mapBaseY;
	int counter = 0;
	int currentpixel = 0;
	float max = args.n.amplitude;

	for (float j = 0; j < args.sectionSize; j++) {
		counter = (j+args.imageBaseY) * args.imageSize + args.imageBaseX;
		currentpixel = counter*4;
		for (float i = 0; i < args.sectionSize; i++) {
			height = noise.FractalSimplex(i*step + x, j*step + y, args.n);
			normal = noise.FractalSimplexNormal(i*step + x, j*step + y, args.n, step);

			//  Convert the numbers to short int
			args.start[currentpixel] = GLushort((normal.x+1)/2 * 65535);			//  R
			args.start[currentpixel + 1] = GLushort((normal.y+1)/2 * 65535);		//  G
			args.start[currentpixel + 2] = GLushort((normal.z+1)/2 * 65535);		//  B
			args.start[currentpixel + 3] = GLushort(((height+max)/(2*max)) * 65535);		// A

			counter++;
			currentpixel = counter*4;
		}
	}


	return 0;
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