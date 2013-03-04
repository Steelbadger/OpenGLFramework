#include "material.h"

#include "shader.h"
#include "texture.h"



std::map<std::string, Material> Material::MaterialLibrary;

Material::Material(void)
{
	shaderProgramRef = shaderProgram.Reference();
}

Material::Material(std::string n):
	name(n)
{
	shaderProgramRef = shaderProgram.Reference();
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
	Shader newShader(shader);
	shaders.push_back(newShader);
	shaderProgram.AddShader(newShader);
}

std::vector<Texture> Material::GetTextures()
{
	return textures;
}

std::vector<Shader> Material::GetShaders()
{
	return shaders;
}

void Material::AddToMaterialLibrary(std::string n)
{
	name = n;
	if (!MaterialLibrary.count(name)) {
		MaterialLibrary[name] = *this;
	}
}
void Material::AddToMaterialLibrary()
{
	if (!MaterialLibrary.count(name)) {
		MaterialLibrary[name] = *this;
	}
}

Material Material::FetchMaterial(std::string name)
{
	if (!MaterialLibrary.count(name)){
		return Material(name);
	} else {
		return MaterialLibrary[name];
	}
}

void Material::Apply()
{
	glUseProgram(shaderProgramRef);
	int numTextures = texRefs.size();

	for (int i = 0; i < numTextures; i++) {
		textures[i].Apply(i, uniforms.Textures[i]);
	}

}

UniformLocations Material::GetUniforms()
{
	return uniforms;
}

void Material::Compile()
{
	shaderProgram.Compile();
	uniforms = shaderProgram.GetUniforms();
	shaderProgramRef = shaderProgram.Reference();
}