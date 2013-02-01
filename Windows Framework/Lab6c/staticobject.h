#pragma once
#include "gameobject.h"
class StaticObject :
	public GameObject
{
public:
	StaticObject(void);
	~StaticObject(void);

	void CreateAndAttachMesh(const char* objectFile, const char* textureFile);
	std::vector<Mesh> GetMeshes(){return meshes;}
	virtual Mesh* GetMesh(){return &meshes[0];}

private:
	std::vector<Mesh> meshes;

};
