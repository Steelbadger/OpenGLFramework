#include "mesh.h"
#include <string>
#include <iostream>
#include "tex.h"
#include "gameobject.h"

int Mesh::IDCOUNTER = 0;
std::map<int, Mesh*> Mesh::IdToMeshMap;

Mesh::Mesh(const char* mPath, const char* tPath, GameObject* p):
	uniqueID(IDCOUNTER++)
{
	meshPath = mPath;
	texturePath = tPath;
	parent = p;
	IdToMeshMap[uniqueID] = this;
	transparency = false;
}


Mesh::~Mesh(void)
{
}

void Mesh::Initialize()
{
	if (LoadMesh(meshPath.c_str()) && LoadTexture(texturePath.c_str())) {
		successfullBuild = true;
	} else {
		successfullBuild = false;
	}
	BuildDisplayList();
}

bool Mesh::LoadMesh(const char* path)
{
	std::string fn = path;
	if(fn.substr(fn.find_last_of(".") + 1) == "obj") {
		return LoadObj(path);
	} else {
		return false;
	}
}

void Mesh::Draw()
{
	glPushMatrix();
		glTranslatef(parent->GetPosition().x, parent->GetPosition().y, parent->GetPosition().z);
		glCallList(dList);
	glPopMatrix();

}

bool Mesh::LoadTexture(const char* path)
{
	GLuint tex;
	std::string fn = path;

	if(fn.substr(fn.find_last_of(".") + 1) == "tga") {
		CreateGLTexture(path, tex);
	} else {
		return false;
	}

	texture = tex;
}

void Mesh::BuildDisplayList()
{
	dList = glGenLists(1);

	glNewList(dList,GL_COMPILE);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, texture);
		glVertexPointer(3, GL_FLOAT, 0, &verts[0]);
		glNormalPointer(GL_FLOAT, 0, &normals[0]);
		glTexCoordPointer(2,GL_FLOAT,0,&uvs[0]);
		glDrawArrays(GL_TRIANGLES, 0, triangles);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEndList();
}

bool Mesh::LoadObj(const char* path)
{
	std::vector<unsigned int> vertIndices, uvIndices, normalIndices;
	std::vector<Vector3> tempVerts;
	std::vector<Vector3> tempNormals;
	std::vector<Vector2> tempUVs;

	FILE * file = fopen(path, "r");
	if( file == NULL ){
		std::cout << "Cannot Open File: " << path << std::endl;
		return false;
	}

	while(true){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			Vector3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			tempVerts.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			Vector2 uv;
			fscanf(file, "%f %f\n", &uv.u, &uv.v );
			tempUVs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			Vector3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			tempNormals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				std::cout << "Cannot Read File: " << path << std::endl;
				return false;
			}
			vertIndices.push_back(vertexIndex[0]);
			vertIndices.push_back(vertexIndex[1]);
			vertIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		Vector3 vertex = tempVerts[vertexIndex-1];
		Vector2 uv = tempUVs[uvIndex-1];
		Vector3 normal = tempNormals[normalIndex-1];
		
		// Put the attributes in buffers
		verts.push_back(vertex);
		uvs.push_back(uv);
		normals.push_back(normal);
	
	}

	triangles = vertIndices.size();

	return true;
}
