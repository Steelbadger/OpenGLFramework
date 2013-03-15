#pragma once

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
	float Perlin2D(float x, float y, int octaves, float zoom, float persistance, float amplitude);
	float TurbulentPerlin2D(float x, float y, NoiseObject n);
	Vector3 NormalToTurbulentPerlin2D(float x, float y, NoiseObject n, float step);
	Vector3 NormalToPerlin2D(float x, float y, int octaves, float zoom, float persistance, float amplitude, float step);
	Vector3 FastNormalToPerlin2D(float x, float y, int octaves, float zoom, float persistance, float amplitude);
	float Perlin2D(float x, float y, NoiseObject n);
	float FastPerlin2D(float x, float y, int octaves, float zoom, float persistance, float amplitude);
	float FastPerlin2D(float x, float y, NoiseObject n);
	float FastPerlin2DSinglePass(float x, float y);
	float Simplex(float x, float y);
	float FourOctaveSimplex(float x, float y, NoiseObject n, int iteration);
	float FractalSimplex(float x, float y, NoiseObject n);
	float HaxFractalSimplex(float x, float y, NoiseObject n);
	Vector3 FractalSimplexNormal(float x, float y, NoiseObject n, float step);
	Vector3 HaxFractalSimplexNormal(float x, float y, NoiseObject n, float step);
	Vector3 NormalToPerlin2D(float x, float y, NoiseObject n, float step);
	Vector3 FastNormalToPerlin2D(float x, float y, NoiseObject n);
	static void GeneratePermutationTable();
	void Seed(float s){seed = s;}

private:
	float NonCoherentNoise1D(float x);
	float NonCoherentNoise2D(float x, float y);
	float Interpolate(float a, float b, float x);
	float lInterpolate(float a, float b, float x);
	float seed;

	static const int SIZE = 256;
	static unsigned char permutation[SIZE];
	static unsigned char perm[SIZE*2];
	static float gradX[SIZE];
	static float gradY[SIZE];
	static bool pTableBuilt;
	static int grads[12][3];
};

