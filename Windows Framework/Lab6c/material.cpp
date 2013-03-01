#include "material.h"


std::map<std::string, Material> Material::MaterialLibrary;

Material::Material(void)
{
}


Material::~Material(void)
{
}


void Material::AddTexture(Texture t)
{
	textures.push_back(t);
	texRefs.push_back(t.Reference());
}

void Material::AddShader(std::string shader)
{
	shaders.push_back(shader);
}

std::vector<Texture> Material::GetTextures()
{
	return textures;
}

std::vector<std::string> Material::GetShaders()
{
	return shaders;
}

void Material::AddToMaterialLibrary(Material m)
{
	if (!MaterialLibrary.count(m.name)) {
		MaterialLibrary[m.name] = m;
	}
}

Material Material::FetchMaterial(std::string name)
{
	if (!MaterialLibrary.count(name)){
		return Material();
	} else {
		return MaterialLibrary[name];
	}
}