#include "renderer.h"


Renderer::Renderer(void)
{
}


Renderer::~Renderer(void)
{
}

void Renderer::AddMeshToRenderer(Mesh m)
{
	renderList.push_back(m.GetDisplayList());
	renderListMap[m.GetDisplayList()]=renderList.size()-1;

	if (inactiveListMap.count(m.GetDisplayList())) {
		inactiveListMap.erase(m.GetDisplayList());
	}
}

void Renderer::RemoveMeshFromRenderer(Mesh m)
{
	int index = renderListMap[m.GetDisplayList()];
	std::vector<GLuint>::iterator it;
	it = renderList.begin();
	it += index;

	renderList.erase(it);

	inactiveList.push_back(m.GetDisplayList());
	inactiveListMap[m.GetDisplayList()]=renderList.size()-1;
}

void Renderer::RenderMesh(Mesh m)
{


}

void Renderer::RenderAll()
{

}