#pragma once

#include "simd.h"
#include "TBB/mutex.h"

class Vector3;

class NoiseObject
{
public:
	NoiseObject(int octaves, float zoom, float persistance, float amplitude, float seed);
	int octaves;
	float zoom;
	float persistance;
	float amplitude;
	float seed;
};

class NoiseGenerator
{
public:
	NoiseGenerator(void);
	~NoiseGenerator(void);
	float Perlin2DSinglePass(float x, float y);
	float Perlin2DFourPass(float x, float y, NoiseObject n, int iteration);
	float Perlin2D(float x, float y, int octaves, float zoom, float persistance, float amplitude);
	Vector3 NormalToPerlin2D(float x, float y, int octaves, float zoom, float persistance, float amplitude, float step);
	float Perlin2D(float x, float y, NoiseObject n);
	float Simplex(float x, float y);
	float FractalSimplex(float x, float y, NoiseObject n);
	Vector3 FractalSimplexNormal(float x, float y, NoiseObject n, float step);
	Vector3 NormalToPerlin2D(float x, float y, NoiseObject n, float step);
	static void GeneratePermutationTable();
	void Seed(float s){seed = s;}
	float NonCoherentNoise2D(float x, float y);
	float MaxAmplitude(NoiseObject n);

	SIMD::Floats NonCoherentNoise2D(SIMD::Floats& x, SIMD::Floats& y);
	float Perlin2DFourPass(float x, float y, float zoom, float persistance, int base);
	float SIMDPerlin2D(float x, float y, NoiseObject n);
	Vector3 SIMDPerlinNormal(float x, float y, NoiseObject n, float step);

private:
	float NonCoherentNoise1D(float x);
	float Interpolate(float a, float b, float x);
	SIMD::Floats Interpolate (SIMD::Floats& a, SIMD::Floats& b, SIMD::Floats& x);
	float seed;

	static tbb::mutex setupMutex;
	static const int SIZE = 256;
	static unsigned char permutation[SIZE];
	static unsigned char perm[SIZE*2];
	static float gradX[SIZE];
	static float gradY[SIZE];
	static bool pTableBuilt;

	static int grads[12][3];
};

