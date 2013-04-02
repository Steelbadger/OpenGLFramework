#include "heightmap.h"
#include "myvector3.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "tex.h"
#include <process.h>
#include <iostream>
#include "mythread.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/partitioner.h"
#include "mythread.h"



struct Generator
{
	Generator(float x, float y, NoiseObject n, float square, float st, unsigned short* m, int s):
		sq(square), no(n), xb(x), yb(y), step(st), map(m), size(s){};

	void operator()(const tbb::blocked_range<int> range) const {
		NoiseGenerator noise;
		for (int j = range.begin(); j != range.end(); j++) {
			int counter = j*size;
			int currentpixel = counter*4;
			for (float i = 0; i < size; i++) {
				float height = noise.FractalSimplex(i*step + xb, j*step + yb, no);
				Vector3 normal = noise.FractalSimplexNormal(i*step + xb, j*step + yb, no, step);

				//  Convert the numbers to short int
				map[currentpixel] = GLushort((normal.x+1)/2 * 65535);			//  R
				map[currentpixel + 1] = GLushort((normal.y+1)/2 * 65535);		//  G
				map[currentpixel + 2] = GLushort((normal.z+1)/2 * 65535);		//  B
				map[currentpixel + 3] = GLushort(((height+no.amplitude)/(2*no.amplitude)) * 65535);		// A

				counter++;
				currentpixel = counter*4;
			}	
		}
	}

	float sq;
	NoiseObject no;
	float xb;
	float yb;
	float step;
	unsigned short* map;
	int size;

};


Heightmap::Heightmap(void)
{
}


Heightmap::~Heightmap(void)
{
}

unsigned short* Heightmap::GenerateHeightField(float x, float y, NoiseObject n, float square)
{
	const int subdivs = 64;  //  256, 512, 1024
	const int threads = size/subdivs;


	GLushort* map = (GLushort*)malloc(size*size*4*sizeof(GLushort));

	NoiseGenerator noise;
	float max = n.amplitude;
	int counter = 0;
	int currentpixel = 0;
	float step = float(square/size);

	ThreadData dataArray[threads][threads];
	HANDLE threadHandles[threads*threads];
	
	for(int i = 0; i < threads; i++) {
		for(int j = 0; j < threads; j++) {
			dataArray[i][j] = ThreadData(map, i*size/threads, j*size/threads, size, i*step*subdivs, j*step*subdivs, square, subdivs, n);
			threadHandles[j+i*threads] = (HANDLE) _beginthreadex(NULL,0,&Heightmap::GenerateSection,(void*)&dataArray[i][j],0,NULL);
		}
	}

	for (int i = 0; i < threads*threads; i+=64) {
		if (threads*threads - i > 64) {
			WaitForMultipleObjects(64, &threadHandles[i], TRUE, INFINITE);
		} else {
			WaitForMultipleObjects(threads*threads-i, &threadHandles[i], TRUE, INFINITE);
		}
	}

//	write_tga("FractalSimplex.tga", size, map);


	return map;
}

unsigned short* Heightmap::TBBGenerateHeightField(float x, float y, NoiseObject n, float square)
{
	GLushort* map = (GLushort*)malloc(size*size*4*sizeof(GLushort));

	float step = float(square/size);

	tbb::parallel_for(tbb::blocked_range<int>(0, size, size/4),Generator(x, y, n, square, step, map, size), tbb::simple_partitioner());

//	write_tga("TBBSimplex.tga", size, map);


	return map;
}

unsigned int Heightmap::GenerateHeightmap(float x, float y, NoiseObject n, float square)
{

	GLushort* map = GenerateHeightField(x, y, n, square);

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
			height = noise.FractalSimplex(i*step + x, j *step + y, args.n);
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

void Heightmap::write_tga(const char *filename, int size, unsigned short* base)
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
		o.put(char(base[i*4]/256));
		o.put(char(base[i*4+1]/256));
		o.put(char(base[i*4+2]/256));
		o.put(char(base[i*4+3]/256));
	}   
	
	//close the file
	o.close();

}