#include "staticobject.h"


StaticObject::StaticObject(void)
{
}


StaticObject::~StaticObject(void)
{
}


void StaticObject::CreateAndAttachMesh(const char* objectFile, const char* textureFile)
{
	Mesh temp(objectFile, textureFile, this);
//	temp.Initialize();
	meshes.push_back(temp);
}