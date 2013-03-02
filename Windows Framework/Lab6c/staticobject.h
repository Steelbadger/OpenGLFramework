#pragma once
#include "gameobject.h"
#include "mesh.h"

class StaticObject :
	public GameObject
{
public:
	StaticObject(void);
	~StaticObject(void);

	void AttachMesh(Mesh m) {m.SetParent(this); meshes.push_back(m);}
	std::vector<Mesh> GetMeshes(){return meshes;}
	virtual Mesh* GetMesh(){return &meshes[0];}

private:
	std::vector<Mesh> meshes;
};

