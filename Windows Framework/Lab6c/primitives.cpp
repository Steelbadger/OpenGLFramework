#include "primitives.h"


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
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;



	return Mesh();
}
