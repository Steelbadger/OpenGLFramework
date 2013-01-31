#include "noisegenerator.h"
#include <math.h>
#include "myvector4.h"
#include "my4x4matrix.h"
#include <stdlib.h>


unsigned char NoiseGenerator::permutation[SIZE];
float NoiseGenerator::gradX[SIZE];
float NoiseGenerator::gradY[SIZE];
bool NoiseGenerator::pTableBuilt = false;


void NoiseGenerator::GeneratePermutationTable()
{
	int j;
	int tempSwap;
	for(int i = 0; i < SIZE; i++) {
		permutation[i] = i;
	}
	for(int i = 0; i < SIZE; i++) {
		j = rand() % SIZE;
		tempSwap = permutation[i];
		permutation[i]  = permutation[j];
		permutation[j]  = tempSwap;
	}
	for(int i = 0; i < SIZE; i++)
	{
		gradX[i] = rand()/float(RAND_MAX/2) - 1.0f; 
		gradY[i] = rand()/float(RAND_MAX/2) - 1.0f;
	}



	pTableBuilt = true;
}


NoiseObject::NoiseObject(int o, float z, float p, float a):
	octaves(o),
	zoom(z),
	persistance(p),
	amplitude(a)
{}

NoiseGenerator::NoiseGenerator(void)
{
	if (!pTableBuilt) {
		GeneratePermutationTable();
	}
}


NoiseGenerator::~NoiseGenerator(void)
{
}

float NoiseGenerator::Perlin2DSinglePass(float x, float y)
{
	float floorX = float(int(x));
	float floorY = float(int(y));

	float s, t, u, v;							//Integer declaration

	s = NonCoherentNoise2D(floorX,floorY); 
	t = NonCoherentNoise2D(floorX+1,floorY);
	u = NonCoherentNoise2D(floorX,floorY+1);	//Get the surrounding pixels to calculate the transition.
	v = NonCoherentNoise2D(floorX+1,floorY+1);

	float int1 = Interpolate(s,t,x-floorX);		//Interpolate between the values.
	float int2 = Interpolate(u,v,x-floorX);		//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
	return Interpolate(int1, int2, y-floorY);	//Here we use y-floory, to get the 2nd dimension.
}

float NoiseGenerator::FastPerlin2DSinglePass(float x, float y)
{
	int floorX = x;
	int floorY = y;

	float s, t, u, v;							//Integer declaration

	int perm00 = permutation[(floorY + permutation[floorX % SIZE]) % SIZE];
	int perm01 = permutation[(floorY + permutation[floorX+1 % SIZE]) % SIZE];
	int perm10 = permutation[(floorY+1 + permutation[floorX % SIZE]) % SIZE];
	int perm11 = permutation[(floorY+1 + permutation[floorX+1 % SIZE]) % SIZE];

	// Computing vectors from the four points to the input point
	float tx0 = x - floorX;
	float tx1 = tx0 - 1;
	float ty0 = y - floorY;
	float ty1 = ty0 - 1;

	// Compute the dot-product between the vectors and the gradients
	s = gradX[perm00]*tx0 + gradY[perm00]*ty0;
	t = gradX[perm01]*tx1 + gradY[perm01]*ty0;
	u = gradX[perm10]*tx0 + gradY[perm10]*ty1;
	v = gradX[perm11]*tx1 + gradY[perm11]*ty1;

	float interpolation1 = lInterpolate(s,t,tx0);		//Interpolate between the values.
	float interpolation2 = lInterpolate(u,v,tx0);		//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
	return lInterpolate(interpolation1, interpolation2, ty0);	//Here we use y-floory, to get the 2nd dimension.
}

float NoiseGenerator::Perlin2D(float x, float y, int octaves, float zoom, float persistance, float amp)
{
	float noise = 0;
	for(int i = 0; i < octaves-1; i++) {
		float frequency = pow(2.0f,i);//This increases the frequency with every loop of the octave.
		float amplitude = pow(persistance,i);//This decreases the amplitude with every loop of the octave.

		noise += Perlin2DSinglePass(x*frequency/zoom, y/zoom*frequency)*amplitude;
	}

	return noise*amp;
}

float NoiseGenerator::TurbulentPerlin2D(float x, float y, NoiseObject n)
{
	return abs(Perlin2D(x, y, n.octaves, n.zoom, n.persistance, n.amplitude));
}

Vector3 NoiseGenerator::NormalToTurbulentPerlin2D(float x, float y, NoiseObject n)
{
	float offs = 0.01f;
	Vector4 A(x, TurbulentPerlin2D(x,y,n), y-offs, 1.0f);
	Vector4 B(x-offs, TurbulentPerlin2D(x-offs,y+offs,n), y+offs, 1.0f);
	Vector4 C(x+offs, TurbulentPerlin2D(x+offs,y+offs,n), y+offs, 1.0f);

	Vector4 AB = B - A;
	Vector4 AC = C - A;

	Vector4 Normal = AB.Cross(AC);
	Normal.NormaliseSelf();
	Vector3 output(Normal);
	return output;
}

float NoiseGenerator::FastPerlin2D(float x, float y, int octaves, float zoom, float persistance, float amp)
{
	float noise = 0;
	for(int i = 0; i < octaves-1; i++) {
		float frequency = pow(2.0f,i);//This increases the frequency with every loop of the octave.
		float amplitude = pow(persistance,i);//This decreases the amplitude with every loop of the octave.

		noise += FastPerlin2DSinglePass(x*frequency/zoom, y/zoom*frequency)*amplitude;
	}

	return noise*amp;
}

float NoiseGenerator::FastPerlin2D(float x, float y, NoiseObject n)
{
	return FastPerlin2D(x, y, n.octaves, n.zoom, n.persistance, n.amplitude);
}

Vector3 NoiseGenerator::NormalToPerlin2D(float x, float y, int octaves, float zoom, float persistance, float amp)
{
	float offs = 0.01f;
	Vector4 A(x, Perlin2D(x,y,octaves,zoom,persistance,amp), y-offs, 1.0f);
	Vector4 B(x-offs, Perlin2D(x-offs,y+offs,octaves,zoom,persistance,amp), y+offs, 1.0f);
	Vector4 C(x+offs, Perlin2D(x+offs,y+offs,octaves,zoom,persistance,amp), y+offs, 1.0f);

	Vector4 AB = B - A;
	Vector4 AC = C - A;

	Vector4 Normal = AB.Cross(AC);
	Normal.NormaliseSelf();
	Vector3 output(Normal);
	return output;
}

Vector3 NoiseGenerator::FastNormalToPerlin2D(float x, float y, int octaves, float zoom, float persistance, float amp)
{
	float offs = 0.01f;
	Vector4 A(x, FastPerlin2D(x,y,octaves,zoom,persistance,amp), y-offs, 1.0f);
	Vector4 B(x-offs, FastPerlin2D(x-offs,y+offs,octaves,zoom,persistance,amp), y+offs, 1.0f);
	Vector4 C(x+offs, FastPerlin2D(x+offs,y+offs,octaves,zoom,persistance,amp), y+offs, 1.0f);

	Vector4 AB = B - A;
	Vector4 AC = C - A;

	Vector4 Normal = AB.Cross(AC);
	Normal.NormaliseSelf();
	Vector3 output(Normal);
	return output;
}

float NoiseGenerator::Perlin2D(float x, float y, NoiseObject n)
{
	return Perlin2D(x, y, n.octaves, n.zoom, n.persistance, n.amplitude);
}

Vector3 NoiseGenerator::NormalToPerlin2D(float x, float y, NoiseObject n)
{
	return NormalToPerlin2D(x, y, n.octaves, n.zoom, n.persistance, n.amplitude);
}

Vector3 NoiseGenerator::FastNormalToPerlin2D(float x, float y, NoiseObject n)
{
	return FastNormalToPerlin2D(x, y, n.octaves, n.zoom, n.persistance, n.amplitude);
}

float NoiseGenerator::Interpolate(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1.0 - cos(ft)) * 0.5;
	return (a * (1.0 - f) + b * f);
}

float NoiseGenerator::lInterpolate(float a, float b, float x)
{
	return a*(1-x) + b*x;
}

float NoiseGenerator::NonCoherentNoise1D(float x)
{	 
	int n = (n<<13) ^ n;
	return float( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);	
}

float NoiseGenerator::NonCoherentNoise2D(float x, float y)
{
	int n = int(x)+int(y*57);
	n = (n<<13)^n;
	int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
	return 1.0-((float)nn/1073741824.0);
}