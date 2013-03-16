#include "noisegenerator.h"
#include <math.h>
#include "myvector4.h"
#include "my4x4matrix.h"
#include <stdlib.h>
#include <algorithm>


unsigned char NoiseGenerator::permutation[SIZE];
unsigned char NoiseGenerator::perm[SIZE*2];
float NoiseGenerator::gradX[SIZE];
float NoiseGenerator::gradY[SIZE];

int NoiseGenerator::grads[12][3] = {{1,1,0},{-1,1,0},{1,-1,0},{-1,-1,0},
									{1,0,1},{-1,0,1},{1,0,-1},{-1,0,-1},
									{0,1,1},{0,-1,1},{0,1,-1},{0,-1,-1}};

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

	for(int i = 0; i < SIZE; i++) {
		gradX[i] = rand()/float(RAND_MAX/2) - 1.0f; 
		gradY[i] = rand()/float(RAND_MAX/2) - 1.0f;
	}

	for (int i = 0; i < SIZE*2; i++) {
		perm[i] = permutation[i&255];
	}

	pTableBuilt = true;
}


NoiseObject::NoiseObject(int o, float z, float p, float a, float s):
	octaves(o),
	zoom(z),
	persistance(p),
	amplitude(a),
	seed(s)
{}

NoiseGenerator::NoiseGenerator(void)
{
	if (!pTableBuilt) {
		seed = 0.0f;
		seedSSE = _mm_set1_ps(seed);
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

float NoiseGenerator::Perlin2DFourPass(float x, float y, NoiseObject n, int iteration)
{
//	float frequency = pow(2.0f,i);//This increases the frequency with every loop of the octave.
//	float amplitude = pow(persistance,i);//This decreases the amplitude with every loop of the octave.

	__declspec(align(16)) float frequency[4];
	__declspec(align(16)) float amplitude[4];
	__declspec(align(16)) float out[4];
	for (int i = 0; i < 4; i++) {
		frequency[i] = pow(2.0f, iteration + i);
		amplitude[i] = pow(n.persistance, iteration + i);
	}

	__m128 freq = _mm_load_ps(frequency);
	__m128 amp = _mm_load_ps(amplitude);

	__m128 coeff = _mm_div_ps(freq, _mm_set1_ps(n.zoom));

	__m128 xa = _mm_mul_ps(_mm_set1_ps(x), coeff);
	__m128 ya = _mm_mul_ps(_mm_set1_ps(y), coeff);

//	noise += Perlin2DSinglePass(x*frequency/zoom, y/zoom*frequency)*amplitude;

//	float floorX = float(int(x));

	__m128 floorX = _mm_cvtepi32_ps(_mm_cvtps_epi32(xa));

//	float floorY = float(int(y));

	__m128 floorY = _mm_cvtepi32_ps(_mm_cvtps_epi32(ya));

//	float s, t, u, v;							//Integer declaration
	__m128 s, t, u, v;

//	s = NonCoherentNoise2D(floorX,floorY); 
	s = SSENonCoherentNoise2D(floorX, floorY);
//	t = NonCoherentNoise2D(floorX+1,floorY);
	t = SSENonCoherentNoise2D(_mm_add_ps(floorX, _mm_set1_ps(1.0f)), floorY);
//	u = NonCoherentNoise2D(floorX,floorY+1);	//Get the surrounding pixels to calculate the transition.
	u = SSENonCoherentNoise2D(floorX, _mm_add_ps(floorY, _mm_set1_ps(1.0f)));
//	v = NonCoherentNoise2D(floorX+1,floorY+1);
	v = SSENonCoherentNoise2D(_mm_add_ps(floorX, _mm_set1_ps(1.0f)), _mm_add_ps(floorY, _mm_set1_ps(1.0f)));

//	float int1 = Interpolate(s,t,x-floorX);		//Interpolate between the values.
	__m128 int1 = Interpolate(s, t, _mm_sub_ps(xa, floorX));

//	float int2 = Interpolate(u,v,x-floorX);		//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
	__m128 int2 = Interpolate(u, v, _mm_sub_ps(xa, floorX));

//	return Interpolate(int1, int2, y-floorY);	//Here we use y-floory, to get the 2nd dimension.
	__m128 final = Interpolate(int1, int2, _mm_sub_ps(ya, floorY));

	final = _mm_mul_ps(final, amp);

	_mm_store_ps(out, final);

	return (out[0]+out[1]+out[2]+out[3]);
}

__m128 NoiseGenerator::Interpolate(__m128 a, __m128 b, __m128 x)
{
	static const float PI = 3.14159265359;

	__m128 ft = _mm_mul_ps(x, _mm_set1_ps(PI));
	__m128 f = _mm_mul_ps(_mm_sub_ps(_mm_set1_ps(1.0f), Cosine(ft)), _mm_set1_ps(0.5f));
	return _mm_add_ps(_mm_mul_ps(a, _mm_sub_ps(_mm_set1_ps(1.0f), f)), _mm_mul_ps(b, f));
}

__m128 NoiseGenerator::Cosine(__m128 a)
{

	static const float PI2 = 1.57079632679;
	static const float PI = 3.14159265359;
	static const float TWOPI = PI*2;
    static const float B = 4/PI;
    static const float C = -4/(PI*PI);


	//  Method courtesy of Nick (http://devmaster.net/forums/topic/4648-fast-and-accurate-sinecosine/)

	__m128 arg = _mm_add_ps(a, _mm_set1_ps(PI2));
	__m128 mask = _mm_cmpnlt_ps(_mm_set1_ps(PI), arg); 
	arg = _mm_sub_ps(arg, _mm_and_ps(_mm_set1_ps(TWOPI), mask));
	__m128 absx = _mm_mul_ps((_mm_and_ps(_mm_cmpgt_ps(arg, _mm_set1_ps(0.0f)), _mm_set1_ps(-1.0f))), arg);


 //   float y = B * x + C * x * abs(x);

	__m128 out = _mm_add_ps(_mm_mul_ps(_mm_set1_ps(B), arg), _mm_mul_ps(_mm_mul_ps(_mm_set1_ps(C), arg), absx));

	return out;
}

__m128 NoiseGenerator::SSENonCoherentNoise2D(__m128 x, __m128 y)
{
	x = _mm_mul_ps(x, seedSSE);
	y = _mm_mul_ps(y, seedSSE);
//	int n = int(x)+int(y*57);

	__m128i n = _mm_add_epi32(_mm_cvtps_epi32(x), _mm_cvtps_epi32(_mm_mul_ps(y, _mm_set1_ps(57.0f))));

	n = _mm_xor_si128(_mm_slli_epi32(n, 13), n);
//	n = (n<<13)^n;

	//  madness?  THIS.  IS.  SIMD!!!
	__m128i nn = _mm_and_si128(_mm_add_epi32(_mm_mul_epi32(n, _mm_mul_epi32(_mm_mul_epi32(_mm_mul_epi32(n,n), _mm_set1_epi32(60493)), _mm_set1_epi32(19990303))), _mm_set1_epi32(1376312589)), _mm_set1_epi32(0x7fffffff));
//	int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;

	return _mm_sub_ps(_mm_set1_ps(1.0), _mm_div_ps(_mm_cvtepi32_ps(nn), _mm_set1_ps(1073741824.0f)));
//	return 1.0-((float)nn/1073741824.0);
}

float NoiseGenerator::HaxFractalPerlin(float x, float y, NoiseObject n)
{
	float noise = 0;
	float maxamp = 0;
	for(int i = 0; i < n.octaves; i++) {
		float amplitude = pow(n.persistance,i);//This decreases the amplitude with every loop of the octave.
		maxamp += amplitude;
	}

	for(int i = 0; i < n.octaves; i+=4) {
		noise += FourOctaveSimplex(x, y, n, i);	
	}
	noise /= maxamp;

	return noise*n.amplitude;	
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

Vector3 NoiseGenerator::NormalToTurbulentPerlin2D(float x, float y, NoiseObject n, float step)
{
	//float offs = 0.01f;
	//Vector4 A(x, TurbulentPerlin2D(x,y,n), y-offs, 1.0f);
	//Vector4 B(x-offs, TurbulentPerlin2D(x-offs,y+offs,n), y+offs, 1.0f);
	//Vector4 C(x+offs, TurbulentPerlin2D(x+offs,y+offs,n), y+offs, 1.0f);

	//Vector4 AB = B - A;
	//Vector4 AC = C - A;

	//Vector4 Normal = AB.Cross(AC);
	//Normal.NormaliseSelf();
	//Vector3 output(Normal);
	//return output;

	float x1 = x-step;
	float x2 = x;
	float x3 = x+step;
	float y1 = y-step;
	float y2 = y;
	float y3 = y+step;

	Vector4 A(x1, TurbulentPerlin2D(x1,y1,n), y1, 1.0f);
	Vector4 B(x2, TurbulentPerlin2D(x2,y1,n), y1, 1.0f);

	Vector4 C(x1, TurbulentPerlin2D(x1,y2,n), y2, 1.0f);
	Vector4 D(x2, TurbulentPerlin2D(x2,y2,n), y2, 1.0f);
	Vector4 E(x3, TurbulentPerlin2D(x3,y2,n), y2, 1.0f);

	Vector4 F(x2, TurbulentPerlin2D(x2,y3,n), y3, 1.0f);
	Vector4 G(x3, TurbulentPerlin2D(x2,y3,n), y3, 1.0f);

	Vector4 DC = C - D;
	Vector4 DA = A - D;
	Vector4 DB = A - D;
	Vector4 DE = E - D;
	Vector4 DG = G - D;
	Vector4 DF = F - D;

	Vector4 Normal1 = DA.Cross(DC);
	Vector4 Normal2 = DB.Cross(DA);
	Vector4 Normal3 = DA.Cross(DB);
	Vector4 Normal4 = DG.Cross(DE);
	Vector4 Normal5 = DF.Cross(DG);
	Vector4 Normal6 = DC.Cross(DF);


	Vector4 Normal = Normal1 + Normal2 + Normal3 + Normal4 + Normal5 + Normal6;

	Normal.NormaliseSelf();

	return Normal;
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

Vector3 NoiseGenerator::NormalToPerlin2D(float x, float y, int octaves, float zoom, float persistance, float amp, float step)
{
	//float offs = 0.01f;

	float x1 = x-step;
	float x2 = x;
	float x3 = x+step;
	float y1 = y-step;
	float y2 = y;
	float y3 = y+step;

	//Vector4 A(x, Perlin2D(x,y,octaves,zoom,persistance,amp), y-offs, 1.0f);
	//Vector4 B(x-offs, Perlin2D(x-offs,y+offs,octaves,zoom,persistance,amp), y+offs, 1.0f);
	//Vector4 C(x+offs, Perlin2D(x+offs,y+offs,octaves,zoom,persistance,amp), y+offs, 1.0f);

	Vector4 A(x1, Perlin2D(x1,y1,octaves,zoom,persistance,amp), y1, 1.0f);
	Vector4 B(x2, Perlin2D(x2,y1,octaves,zoom,persistance,amp), y1, 1.0f);

	Vector4 C(x1, Perlin2D(x1,y2,octaves,zoom,persistance,amp), y2, 1.0f);
	Vector4 D(x2, Perlin2D(x2,y2,octaves,zoom,persistance,amp), y2, 1.0f);
	Vector4 E(x3, Perlin2D(x3,y2,octaves,zoom,persistance,amp), y2, 1.0f);

	Vector4 F(x2, Perlin2D(x2,y3,octaves,zoom,persistance,amp), y3, 1.0f);
	Vector4 G(x3, Perlin2D(x2,y3,octaves,zoom,persistance,amp), y3, 1.0f);

	Vector4 DC = C - D;
	Vector4 DA = A - D;
	Vector4 DB = A - D;
	Vector4 DE = E - D;
	Vector4 DG = G - D;
	Vector4 DF = F - D;

	Vector4 Normal1 = DC.Cross(DA);
	Vector4 Normal2 = DA.Cross(DB);
	Vector4 Normal3 = DB.Cross(DE);
	Vector4 Normal4 = DE.Cross(DG);
	Vector4 Normal5 = DG.Cross(DF);
	Vector4 Normal6 = DF.Cross(DC);

	Vector4 Normal = Normal1 + Normal2 + Normal3 + Normal4 + Normal5 + Normal6;

	//Vector4 AB = B - A;
	//Vector4 AC = C - A;
	
	//Vector4 Normal = AB.Cross(AC);

	Normal.NormaliseSelf();

	return Normal;
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

Vector3 NoiseGenerator::NormalToPerlin2D(float x, float y, NoiseObject n, float step)
{
	return NormalToPerlin2D(x, y, n.octaves, n.zoom, n.persistance, n.amplitude, step);
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
	x *= seed;
	y *= seed;
	int n = int(x)+int(y*57);
	n = (n<<13)^n;
	int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
	return 1.0-((float)nn/1073741824.0);
}
float NoiseGenerator::Simplex(float x, float y)
{
	float root3 = 1.73205080757;
	float n1, n2, n3;		// Noise contributions from the three corners

	// Skew the input space to determine which simplex cell we're in
	float skewFactor = 0.5*(root3-1.0);
	float s = (x+y)*skewFactor;			// Hairy factor for 2D

	int i = int(x+s);
	int j = int(y+s);

	float unskewFactor = (3.0-root3)/6.0;
	float t = (i+j)*unskewFactor;

	float X0 = i-t;		// Unskew the cell origin back to (x,y) space
	float Y0 = j-t;
	float dx = x-X0;		// The x,y distances from the cell origin
	float dy = y-Y0;

	// For the 2D case, the simplex shape is an equilateral triangle.
	// Determine which simplex we are in.

	int i1, j1;		// Offsets for second (middle) corner of simplex in (i,j) coords

	int check = dy-dx;

	if(dx>dy) {		// lower triangle, XY order: (0,0)->(1,0)->(1,1)
		i1=1;
		j1=0;
	}
	else {			// upper triangle, YX order: (0,0)->(0,1)->(1,1)
		i1=0;
		j1=1;
	}				


	float x2 = dx - i1 + unskewFactor;				// Offsets for middle corner in (x,y) unskewed coords
	float y2 = dy - j1 + unskewFactor;
	float x3 = dx - 1.0 + 2.0 * unskewFactor;		// Offsets for last corner in (x,y) unskewed coords
	float y3 = dy - 1.0 + 2.0 * unskewFactor;

	// Work out the hashed gradient indices of the three simplex corners

	int ii = i & 255;
	int jj = j & 255;

	int grad1 = perm[ii+perm[jj]] % 12;
	int grad2 = perm[ii+i1+perm[jj+j1]] % 12;
	int grad3 = perm[ii+1+perm[jj+1]] % 12;

	double t1 = 0.5 - dx*dx-dy*dy;
	if (t1<0) {
		n1 = 0.0;
	} else {
		t1 *= t1;
		n1 = t1 * t1 * (grads[grad1][0] * dx + grads[grad1][1] * dy);		// (x,y) of grad3 used for 2D gradient
	}


	double t2 = 0.5 - x2*x2-y2*y2;
	if (t2<0) {
		n2 = 0.0;
	} else {
		t2 *= t2;
		n2 = t2 * t2 *(grads[grad2][0] * x2 + grads[grad2][1] * y2);
	}


	double t3 = 0.5 - x3*x3-y3*y3;
	if (t3<0) {
		n3 = 0.0;
	} else {
		t3 *= t3;
		n3 = t3 * t3 * (grads[grad3][0]* x3 + grads[grad3][1] * y3);
	}


	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1] (ish).
	return 70.0 * (n1 + n2 + n3);

}


float NoiseGenerator::FractalSimplex(float x, float y, NoiseObject n)
{
	float noise = 0;
	float maxamp = 0;
	for(int i = 0; i < n.octaves; i++) {
		float frequency = pow(2.0f,i);//This increases the frequency with every loop of the octave.
		float amplitude = pow(n.persistance,i);//This decreases the amplitude with every loop of the octave.
		maxamp += amplitude;
		noise += Simplex(x*frequency/n.zoom, y/n.zoom*frequency)*amplitude;
	}
	noise /= maxamp;

	return noise*n.amplitude;	
}

Vector3 NoiseGenerator::FractalSimplexNormal(float x, float y, NoiseObject n, float step)
{
	float offs = step;
	float xtrioffs = offs * 0.86602540378;
	float ytrioffs = offs * 0.5;
	Vector4 A(x, FractalSimplex(x,y+xtrioffs,n), y+xtrioffs, 1.0f);
	Vector4 B(x-xtrioffs, FractalSimplex(x-xtrioffs,y-ytrioffs,n), y-ytrioffs, 1.0f);
	Vector4 C(x+xtrioffs, FractalSimplex(x+xtrioffs,y-ytrioffs,n), y-ytrioffs, 1.0f);

	Vector4 AB = B - A;
	Vector4 AC = C - A;

	Vector4 Normal = AC.Cross(AB);
	Normal.NormaliseSelf();
	Vector3 output(Normal);
	return output;
}

float NoiseGenerator::FourOctaveSimplex(float x, float y, NoiseObject n, int iteration)
{
	float root3 = 1.73205080757;

	__declspec(align(16)) float frequency[4];
	for (int i = 0; i < 4; i++) {
		frequency[i] = pow(2.0f, iteration + i);
	}

	//  Put our zoom level and frequency into 4-float-wide sse variables
	__m128 zoom = _mm_set1_ps(n.zoom);
	__m128 freq = _mm_load_ps(frequency);


	//  Put the x position of interest into a 4-float-wide sse variable
	__m128 xb = _mm_set1_ps(x);
	__m128 yb = _mm_set1_ps(y);

	  
	__m128 scaleFactor = _mm_div_ps(freq, zoom);

	__m128 xa = _mm_mul_ps(xb, scaleFactor);
	__m128 ya = _mm_mul_ps(yb, scaleFactor);



	float skewFact = 0.5*(root3-1.0);
	float unskewFact = (3.0-root3)/6.0;

	__m128 skewFactor = _mm_set1_ps(skewFact);
	__m128 unskewFactor = _mm_set1_ps(unskewFact);

	//float s = (x+y)*skewFactor;			// Hairy factor for 2D	
	__m128 hairy = _mm_mul_ps(_mm_add_ps(xa, ya), skewFactor);


	//int i = int(x+s)
	//int j = int(y+s)
	__m128 fi = _mm_cvtepi32_ps(_mm_cvtps_epi32(_mm_add_ps(xa, hairy)));
	__m128 fj = _mm_cvtepi32_ps(_mm_cvtps_epi32(_mm_add_ps(ya, hairy)));

	//float t = (i+j)*unskewFactor;
	__m128 t = _mm_mul_ps(_mm_add_ps(fi, fj), skewFactor);

	//float X0 = i-t;		// Unskew the cell origin back to (x,y) space
	//float Y0 = j-t;

	__m128 X0 = _mm_sub_ps(fi, t);
	__m128 Y0 = _mm_sub_ps(fj, t);

	//float dx = x-X0;		// The x,y distances from the cell origin
	//float dy = y-Y0;

	__m128 dx = _mm_sub_ps(xa, X0);
	__m128 dy = _mm_sub_ps(ya, Y0);


	//  Make some 4 repeat versions of numbers we'll use
	__m128 Zero =  _mm_set1_ps(0.0f);
	__m128 One =  _mm_set1_ps(1.0f);
	__m128 Two =  _mm_set1_ps(2.0f);


	//int check = dy-dx;

	//if(dx>dy) {		// lower triangle, XY order: (0,0)->(1,0)->(1,1)
	//	i1=1;
	//	j1=0;
	//}
	//else {			// upper triangle, YX order: (0,0)->(0,1)->(1,1)
	//	i1=0;
	//	j1=1;
	//}			

	//  check = 0xffffffff if dx > dy else 0x0
	__m128 check = _mm_cmpgt_ps(dx, dy);

	//if check is 0xffffffff then i1 is 1.0, else it's zero
	__m128 i1 = _mm_and_ps(check, One);
	//  j1 has 1 is i1 is 0 and 0 if i1 is 1;
	__m128 j1 = _mm_sub_ps(One, i1);

	//float x2 = dx - i1 + unskewFactor;				// Offsets for middle corner in (x,y) unskewed coords
	//float y2 = dy - j1 + unskewFactor;

	__m128 x2 = _mm_add_ps(_mm_sub_ps(dx, i1), unskewFactor);
	__m128 y2 = _mm_add_ps(_mm_sub_ps(dy, j1), unskewFactor);

	//float x3 = dx - 1.0 + 2.0 * unskewFactor;		// Offsets for last corner in (x,y) unskewed coords
	//float y3 = dy - 1.0 + 2.0 * unskewFactor;

	//  Calculation reordered to dx - (1-2*unskew) = dx - 1 + 2*unskew
	__m128 fac3 = _mm_sub_ps(One, _mm_mul_ps(unskewFactor, Two));

	__m128 x3 = _mm_sub_ps(dx, fac3);
	__m128 y3 = _mm_sub_ps(dy, fac3);

	
	//int ii = i & 255;
	//int jj = j & 255;

	//int grad1 = perm[ii+perm[jj]] % 12;
	//int grad2 = perm[ii+i1+perm[jj+j1]] % 12;
	//int grad3 = perm[ii+1+perm[jj+1]] % 12;

	__m128i MASK =  _mm_set1_epi32(255);

	//  ii = i & 255
	__m128i ii = _mm_and_si128(_mm_cvtps_epi32(fi), MASK);
	__m128i jj = _mm_and_si128(_mm_cvtps_epi32(fj), MASK);


	//  Get ii and jj out and cast to int arrays;
	__m128i iiout, jjout;

	int* iitest = reinterpret_cast<int*>(&ii);

	_mm_store_si128(&iiout, ii);
	_mm_store_si128(&jjout, jj);
	int* iip = (int*)&iiout;
	int* jjp = (int*)&jjout;

	__declspec(align(16)) float i1f[4];
	__declspec(align(16)) float j1f[4];

	_mm_store_ps(i1f,i1);
	_mm_store_ps(j1f,j1);

	int i1_index[4] = {i1f[0], i1f[1], i1f[2], i1f[3]};
	int j1_index[4] = {j1f[0], j1f[1], j1f[2], j1f[3]};



	// Work out the hashed gradient indices of the three simplex corners
	__declspec(align(16)) int grad1[4];
	__declspec(align(16)) int grad2[4];
	__declspec(align(16)) int grad3[4];

	for (int c = 0; c < 4; c++) {
		grad1[c] = perm[iip[c] + perm[jjp[c]]]%12;
		grad2[c] = perm[iip[c] + i1_index[c] + perm[jjp[c] + j1_index[c]]]%12;
		grad3[c] = perm[iip[c] + 1 + perm[jjp[c] + 1]]%12;
	}

	// Calculate the contribution from the three corners
	//  This method is slower than the second method, but has no branching.

	__m128 PointFive = _mm_set_ps1(0.5f);

	__declspec(align(16)) float xGrads1[4] = {grads[grad1[0]][0], grads[grad1[1]][0], grads[grad1[2]][0], grads[grad1[2]][0]};
	__declspec(align(16)) float yGrads1[4] = {grads[grad1[0]][1], grads[grad1[1]][1], grads[grad1[2]][1], grads[grad1[2]][1]};

	__declspec(align(16)) float xGrads2[4] = {grads[grad2[0]][0], grads[grad2[1]][0], grads[grad2[2]][0], grads[grad2[2]][0]};
	__declspec(align(16)) float yGrads2[4] = {grads[grad2[0]][1], grads[grad2[1]][1], grads[grad2[2]][1], grads[grad2[2]][1]};

	__declspec(align(16)) float xGrads3[4] = {grads[grad3[0]][0], grads[grad3[1]][0], grads[grad3[2]][0], grads[grad3[2]][0]};
	__declspec(align(16)) float yGrads3[4] = {grads[grad3[0]][1], grads[grad3[1]][1], grads[grad3[2]][1], grads[grad3[2]][1]};

	__m128 gradsx1 = _mm_load_ps(xGrads1);
	__m128 gradsy1 = _mm_load_ps(yGrads1);
	__m128 gradsx2 = _mm_load_ps(xGrads2);
	__m128 gradsy2 = _mm_load_ps(yGrads2);
	__m128 gradsx3 = _mm_load_ps(xGrads3);
	__m128 gradsy3 = _mm_load_ps(yGrads3);

	__m128 t1 = _mm_sub_ps(PointFive, _mm_add_ps(_mm_mul_ps(dx, dx), _mm_mul_ps(dy, dy)));

	t1 = _mm_max_ps(Zero, t1);
	t1 = _mm_mul_ps(t1, t1);
	t1 = _mm_mul_ps(t1, t1);
	__m128 n1 = _mm_mul_ps(t1, _mm_add_ps(_mm_mul_ps(gradsx1, dx), _mm_mul_ps(gradsy1, dy)));


	__m128 t2 = _mm_sub_ps(PointFive, _mm_add_ps(_mm_mul_ps(x2, x2), _mm_mul_ps(y2, y2)));

	t2 = _mm_max_ps(Zero, t2);
	t2 = _mm_mul_ps(t2, t2);
	t2 = _mm_mul_ps(t2, t2);
	__m128 n2 = _mm_mul_ps(t2, _mm_add_ps(_mm_mul_ps(gradsx2, x2), _mm_mul_ps(gradsy2, y2)));

	__m128 t3 = _mm_sub_ps(PointFive, _mm_add_ps(_mm_mul_ps(x3, x3), _mm_mul_ps(y3, y3)));

	t3 = _mm_max_ps(Zero, t3);
	t3 = _mm_mul_ps(t3, t3);
	t3 = _mm_mul_ps(t3, t3);
	__m128 n3 = _mm_mul_ps(t3, _mm_add_ps(_mm_mul_ps(gradsx3, x3), _mm_mul_ps(gradsy3, y3)));
	

	__declspec(align(16)) float n1Out[4];
	__declspec(align(16)) float n2Out[4];
	__declspec(align(16)) float n3Out[4];

	_mm_store_ps(n1Out, n1);
	_mm_store_ps(n2Out, n2);
	_mm_store_ps(n3Out, n3);

	float sum = 0;
	float amplitude = 0;

	for (int i = 0; i < 4; i++) {
		amplitude = pow(n.persistance, i+iteration);
		sum += (n1Out[i] + n2Out[i] + n3Out[i])*70.0f*amplitude;
	}

	// Add contributions from each corner to get the final noise value.
	return sum;
}

float NoiseGenerator::HaxFractalSimplex(float x, float y, NoiseObject n)
{
	float noise = 0;
	float maxamp = 0;
	for(int i = 0; i < n.octaves; i++) {
		float frequency = pow(2.0f,i);//This increases the frequency with every loop of the octave.
		float amplitude = pow(n.persistance,i);//This decreases the amplitude with every loop of the octave.
		maxamp += amplitude;
//		Simplex(x*frequency/n.zoom, y/n.zoom*frequency)*amplitude;
	}

	for(int i = 0; i < n.octaves; i+=4) {
		noise += FourOctaveSimplex(x, y, n, i);	
	}
	noise /= maxamp;

	return noise*n.amplitude;	
}

Vector3 NoiseGenerator::HaxFractalSimplexNormal(float x, float y, NoiseObject n, float step)
{
	float offs = step;
	float xtrioffs = offs * 0.86602540378;
	float ytrioffs = offs * 0.5;
	Vector4 A(x, HaxFractalSimplex(x,y+xtrioffs,n), y+xtrioffs, 1.0f);
	Vector4 B(x-xtrioffs, HaxFractalSimplex(x-xtrioffs,y-ytrioffs,n), y-ytrioffs, 1.0f);
	Vector4 C(x+xtrioffs, HaxFractalSimplex(x+xtrioffs,y-ytrioffs,n), y-ytrioffs, 1.0f);

	Vector4 AB = B - A;
	Vector4 AC = C - A;

	Vector4 Normal = AC.Cross(AB);
	Normal.NormaliseSelf();
	Vector3 output(Normal);
	return output;
}



