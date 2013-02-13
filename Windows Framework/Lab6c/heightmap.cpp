#include "heightmap.h"
#include "myvector3.h"


struct pixel
{
	char A;
	char R;
	char G;
	char B;
};


Heightmap::Heightmap(void)
{
}


Heightmap::~Heightmap(void)
{
}


unsigned int Heightmap::GenerateHeightmap(float x, float y, NoiseObject n)
{
	pixel map[1024][1024];
	NoiseGenerator noise;
	float max = n.amplitude;

	for (int i = 0; i < 1024; i++) {
		for (int j = 0; j < 1024; j++) {
			float height = noise.TurbulentPerlin2D(x+i, y+j, n);
			Vector3 normal = noise.NormalToTurbulentPerlin2D(x+i, y+j, n, 1.0);

			map[i][j].A = char((height/max) * 255);
	//		map[i][j].R = char((
		}
	}

	return 1;
}
