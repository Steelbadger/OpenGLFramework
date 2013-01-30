#include "noisegenerator.h"
#include <math.h>
#include "myvector4.h"
#include "my4x4matrix.h"


NoiseObject::NoiseObject(int o, float z, float p, float a):
	octaves(o),
	zoom(z),
	persistance(p),
	amplitude(a)
{}

NoiseGenerator::NoiseGenerator(void)
{
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

float NoiseGenerator::Perlin2D(float x, float y, NoiseObject n)
{
	return Perlin2D(x, y, n.octaves, n.zoom, n.persistance, n.amplitude);
}

Vector3 NoiseGenerator::NormalToPerlin2D(float x, float y, NoiseObject n)
{
	return NormalToPerlin2D(x, y, n.octaves, n.zoom, n.persistance, n.amplitude);
}


float NoiseGenerator::Interpolate(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1.0 - cos(ft)) * 0.5;
	return (a * (1.0 - f) + b * f);
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