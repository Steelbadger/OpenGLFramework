#include "noisegenerator.h"
#include <math.h>
#include "myvector4.h"
#include "my4x4matrix.h"
#include <stdlib.h>

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
	double xin = x;
	double yin = y;
	float root3 = 1.73205080757;
    double n0, n1, n2; // Noise contributions from the three corners
    // Skew the input space to determine which simplex cell we're in
    double F2 = 0.5*(root3-1.0);
    double s = (xin+yin)*F2; // Hairy factor for 2D
    int i = int(xin+s);
    int j = int(yin+s);
    double G2 = (3.0-root3)/6.0;
    double t = (i+j)*G2;
    double X0 = i-t; // Unskew the cell origin back to (x,y) space
    double Y0 = j-t;
    double x0 = xin-X0; // The x,y distances from the cell origin
    double y0 = yin-Y0;
    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
    if(x0>y0) {i1=1; j1=0;} // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    else {i1=0; j1=1;}      // upper triangle, YX order: (0,0)->(0,1)->(1,1)
    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6
    double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    double y1 = y0 - j1 + G2;
    double x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
    double y2 = y0 - 1.0 + 2.0 * G2;
    // Work out the hashed gradient indices of the three simplex corners
    int ii = i & 255;
    int jj = j & 255;
    int gi0 = perm[ii+perm[jj]] % 12;
    int gi1 = perm[ii+i1+perm[jj+j1]] % 12;
    int gi2 = perm[ii+1+perm[jj+1]] % 12;
    // Calculate the contribution from the three corners
    double t0 = 0.5 - x0*x0-y0*y0;
    if(t0<0) n0 = 0.0;
    else {
      t0 *= t0;
      n0 = t0 * t0 * (grads[gi0][0] * x0 + grads[gi0][1] * y0);  // (x,y) of grad3 used for 2D gradient
    }
    double t1 = 0.5 - x1*x1-y1*y1;
    if(t1<0) n1 = 0.0;
    else {
      t1 *= t1;
      n1 = t1 * t1 *(grads[gi1][0] * x1 + grads[gi1][1] * y1);
    }
	double t2 = 0.5 - x2*x2-y2*y2;
    if(t2<0) n2 = 0.0;
    else {
      t2 *= t2;
      n2 = t2 * t2 * (grads[gi2][0]* x2 + grads[gi2][1] * y2);
    }
    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [-1,1].
    return 70.0 * (n0 + n1 + n2);


	////  Skew input to find coordinates in simplex-space
	//float skewFactor = 0.5*(root3-1);
	//float s = (x + y) * skewFactor;
	//int i = int(x+s);
	//int j = int(y+s);

	////  Skew back to input space and find first simplex corner
	//float unskewFactor = (3.0 - root3)/6;
	//float t = (i + j) * unskewFactor;
	//float x0 = i - t;
	//float y0 = j - t;

	//float x1 = x - x0;
	//float y1 = x - y0;

	////  2D case, simplex is triangle, determine which triangle we are in
	////  We're working in Simplex space again here, so i and j come back.
	//int iOffset, jOffset;

	//if (x1 > y1) {
	//	iOffset = 1;
	//	jOffset = 0;
	//} else {
	//	iOffset = 0;
	//	jOffset = 1;
	//}

	////  Find other two simplex corners
	//float x2 = x1 - iOffset + unskewFactor;
	//float y2 = y1 - jOffset + unskewFactor;

	//float x3 = x1 - 1.0 + 2*unskewFactor;
	//float y3 = y1 - 1.0 + 2*unskewFactor;

	////  Find the hashed gradient indices for the three corners
	//int ihash = i & 255;
	//int jhash = j & 255;

	//int grad1 = perm[ihash + perm[jhash]]%12;
	//int grad2 = perm[ihash + iOffset + perm[jhash + jOffset]]%12;
	//int grad3 = perm[ihash + 1 + perm[jhash + 1]]%12;

	////  Calculate the noise contributions

	//float n1, n2, n3;

	//float smooth1 = 0.5 - x1 * x1 - y1 * y1;
	//if (smooth1 < 0) {
	//	n1 = 0.0;
	//} else {
	//	smooth1 *= smooth1;
	//	n1 = smooth1 * smooth1 * (grads[grad1][0] * x1 + grads[grad1][1] * y1);
	//}

	//float smooth2 = 0.5 - x2 * x2 - y2 - y2;
	//if (smooth2 < 0) {
	//	n2 = 0;
	//} else {
	//	smooth2 *= smooth2;
	//	n2 = smooth2 * smooth2 * (grads[grad2][0] * x2 + grads[grad2][1] * y2);
	//}

	//float smooth3 = 0.5 - x3 * x3 - y3 * y3;
	//if (smooth3 < 0) {
	//	n3 = 0;
	//} else {
	//	smooth3 *= smooth3;
	//	n3 = smooth3 * smooth3 * (grads[grad3][0] * x3 + grads[grad3][1] * y3);
	//}
	//
	//return (70 * (n1 + n2 + n3));
}


float NoiseGenerator::FractalSimplex(float x, float y, NoiseObject n)
{
	float noise = 0;
	for(int i = 0; i < n.octaves; i++) {
		float frequency = pow(2.0f,i);//This increases the frequency with every loop of the octave.
		float amplitude = pow(n.persistance,i);//This decreases the amplitude with every loop of the octave.

		//noise += Perlin2DSinglePass(x*frequency/n.zoom, y/n.zoom*frequency)*amplitude;
		//noise += Simplex(x, y)*amplitude;
		noise += Simplex(x*frequency/n.zoom, y/n.zoom*frequency)*amplitude;
	}

	return noise*n.amplitude;	
}

Vector3 NoiseGenerator::FractalSimplexNormal(float x, float y, NoiseObject n)
{
	float offs = 0.1f;
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
