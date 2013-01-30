#pragma once

class Vector3;

class NoiseObject
{
public:
	NoiseObject(int octaves, float zoom, float persistance, float amplitude);
	int octaves;
	float zoom;
	float persistance;
	float amplitude;
};

class NoiseGenerator
{
public:
	NoiseGenerator(void);
	~NoiseGenerator(void);
	float Perlin2DSinglePass(float x, float y);
	float Perlin2D(float x, float y, int octaves, float zoom, float persistance, float amplitude);
	Vector3 NormalToPerlin2D(float x, float y, int octaves, float zoom, float persistance, float amplitude);

	float Perlin2D(float x, float y, NoiseObject n);
	Vector3 NormalToPerlin2D(float x, float y, NoiseObject n);

private:
	float NonCoherentNoise1D(float x);
	float NonCoherentNoise2D(float x, float y);
	float Interpolate(float a, float b, float x);
};

