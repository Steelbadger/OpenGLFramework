#include "primitives.h"

#include <math.h>

PrimitiveFactory::PrimitiveFactory(void)
{
}


PrimitiveFactory::~PrimitiveFactory(void)
{
}

Mesh PrimitiveFactory::Plane(float width, float height, int widthSubDivs, int heightSubDivs)
{
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;
	std::vector<unsigned int> index;

	float widthStep = width/(widthSubDivs-1);
	float heightStep = height/(heightSubDivs-1);

	for (int i = 0; i < heightSubDivs; i++) {
		for (int j = 0; j < widthSubDivs; j++) {
			verts.push_back(Vector3(i*widthStep-width/2, 0, j*heightStep-height/2));
			Vector3 normalA = Vector3(0,1,0);
			normals.push_back(normalA);
			texCoords.push_back(Vector2(i*widthStep/width, j*heightStep/height));
		}
	}

	for (int i = 0; i < heightSubDivs-1; i++) {
		for (int j = 0; j < widthSubDivs-1; j++) {
			index.push_back(i+j*widthSubDivs);
			index.push_back(1+i+(1+j)*widthSubDivs);
			index.push_back(i+(1+j)*widthSubDivs);
			index.push_back(1+i+(1+j)*widthSubDivs);
			index.push_back(i+j*widthSubDivs);
			index.push_back(1+i+j*widthSubDivs);
		}
	}

	return Mesh(verts, normals, texCoords, index);
}

Mesh PrimitiveFactory::SimpleInnerBox()
{
	Mesh output = UnitCube();
	output.ReverseNormals();
	return output;
}

Mesh PrimitiveFactory::UnitCube()
{
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;

	std::vector<Vector3> points;
	std::vector<Vector3> faceNormals;
	std::vector<Vector2> uvPoints;

	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				points.push_back(Vector3(-0.5+i, -0.5+j, -0.5+k));
			}
		}
	}

	faceNormals.push_back(Vector3(-1.0, 0, 0));
	faceNormals.push_back(Vector3(1.0, 0, 0));
	faceNormals.push_back(Vector3(0, -1.0, 0));
	faceNormals.push_back(Vector3(0, 1.0, 0));
	faceNormals.push_back(Vector3(0, 0, -1.0));
	faceNormals.push_back(Vector3(0, 0, 1.0));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			uvPoints.push_back(Vector2(0.25*j, 0.25*i));
		}
	}


	//  Face 1 - Triangle 1
	verts.push_back(points[0]);
	verts.push_back(points[2]);
	verts.push_back(points[1]);
	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[0]);
	}
	texCoords.push_back(uvPoints[5]);
	texCoords.push_back(uvPoints[6]);
	texCoords.push_back(uvPoints[10]);


	//  Face 1 - Triangle 2
	verts.push_back(points[1]);
	verts.push_back(points[2]);
	verts.push_back(points[3]);
	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[0]);
	}
	texCoords.push_back(uvPoints[10]);
	texCoords.push_back(uvPoints[6]);
	texCoords.push_back(uvPoints[11]);

	//  Face 2 - Triangle 1
	verts.push_back(points[7]);
	verts.push_back(points[3]);
	verts.push_back(points[2]);
	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[3]);
	}
	texCoords.push_back(uvPoints[12]);
	texCoords.push_back(uvPoints[11]);
	texCoords.push_back(uvPoints[6]);


	//  Face 2 - Triangle 2
	verts.push_back(points[7]);
	verts.push_back(points[2]);
	verts.push_back(points[6]);
	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[3]);
	}
	texCoords.push_back(uvPoints[12]);
	texCoords.push_back(uvPoints[6]);
	texCoords.push_back(uvPoints[7]);

	//  Face 3 - Triangle 1
	verts.push_back(points[5]);
	verts.push_back(points[7]);
	verts.push_back(points[6]);
	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[1]);
	}
	texCoords.push_back(uvPoints[13]);
	texCoords.push_back(uvPoints[12]);
	texCoords.push_back(uvPoints[7]);

	//  Face 3 - Triangle 2
	verts.push_back(points[4]);
	verts.push_back(points[5]);
	verts.push_back(points[6]);
	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[1]);
	}
	texCoords.push_back(uvPoints[8]);
	texCoords.push_back(uvPoints[13]);
	texCoords.push_back(uvPoints[7]);

	//  Face 4 - Triangle 1
	verts.push_back(points[5]);
	verts.push_back(points[1]);
	verts.push_back(points[3]);
	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[5]);
	}
	texCoords.push_back(uvPoints[17]);
	texCoords.push_back(uvPoints[16]);
	texCoords.push_back(uvPoints[11]);

	//  Face 4 - Triangle 2
	verts.push_back(points[3]);
	verts.push_back(points[7]);
	verts.push_back(points[5]);
	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[5]);
	}
	texCoords.push_back(uvPoints[11]);
	texCoords.push_back(uvPoints[12]);
	texCoords.push_back(uvPoints[17]);

	//  Face 5 - Triangle 1
	verts.push_back(points[0]);
	verts.push_back(points[1]);
	verts.push_back(points[5]);
	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[2]);
	}
	texCoords.push_back(uvPoints[9]);
	texCoords.push_back(uvPoints[14]);
	texCoords.push_back(uvPoints[13]);

	//  Face 5 - Triangle 2
	verts.push_back(points[0]);
	verts.push_back(points[5]);
	verts.push_back(points[4]);
	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[2]);
	}
	texCoords.push_back(uvPoints[9]);
	texCoords.push_back(uvPoints[13]);
	texCoords.push_back(uvPoints[8]);

	//  Face 6 - Triangle 1
	verts.push_back(points[0]);
	verts.push_back(points[4]);
	verts.push_back(points[2]);
	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[4]);
	}
	texCoords.push_back(uvPoints[1]);
	texCoords.push_back(uvPoints[2]);
	texCoords.push_back(uvPoints[6]);

	//  Face 6 - Triangle 2
	verts.push_back(points[4]);
	verts.push_back(points[6]);
	verts.push_back(points[2]);
	for (int i = 0; i < 3; i++) {
		normals.push_back(faceNormals[4]);
	}
	texCoords.push_back(uvPoints[2]);
	texCoords.push_back(uvPoints[7]);
	texCoords.push_back(uvPoints[6]);

	return Mesh(verts, normals, texCoords);
}

Mesh PrimitiveFactory::LSphere(int lats, int longs)
{
	const float pi = 3.14159265359;

	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;
	std::vector<unsigned int> index;

	float deltaTheta = 2*pi/(lats+1);
	float deltaGamma = 2*pi/(longs-1);

	for(float Theta = 0; Theta < 2*pi; Theta+=deltaTheta) {
		for(float Gamma = 0; Gamma < pi; Gamma+=deltaGamma) {
			float sinTheta = sin(Theta);
			float cosTheta = cos(Theta);
			float sinGamma = sin(Gamma);
			float cosGamma = cos(Gamma);
			Vector3 point(cosTheta*sinGamma, sinTheta*sinGamma, cosGamma);
			verts.push_back(point);
			normals.push_back(point);
			texCoords.push_back(Vector2(Theta/(2*pi), Gamma/pi));
		}
	}

	for (int i = 0; i < longs; i++) {
		index.push_back(0);
		index.push_back(1+i*(lats+2));
		index.push_back(0);

		index.push_back(lats+i*(lats+2));
		index.push_back(lats+i*(lats+2)+1);
		index.push_back(lats+(i+1)*(lats+2));
	}


	return Mesh();
}
