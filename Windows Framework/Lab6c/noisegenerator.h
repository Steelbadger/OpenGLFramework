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
	static float gradX[SIZE];
	static float gradY[SIZE];
	static bool pTableBuilt;
};

