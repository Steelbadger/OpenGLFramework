#pragma once
#include "texture.h"
#include <vector>
#include <map>

class Material
{
public:
	Material(void);
	~Material(void);

	void AddTexture(Texture t);
	void AddShader(std::string shader);
	
	std::vector<Texture> GetTextures();
	std::vector<std::string> GetShaders();

	static void AddToMaterialLibrary(Material m);
	static Material FetchMaterial(std::string name);
private:
	std::vector<Texture> textures;
	std::vector<std::string> shaders;
	std::string name;

	static std::map<std::string, Material> MaterialLibrary;
};

