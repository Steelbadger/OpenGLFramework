#include "material.h"

#include "shader.h"
#include "texture.h"



std::map<std::string, Material> Material::MaterialLibrary;

Material::Material(void)
{
	shaderProgramRef = shaderProgram.Reference();
	compiled = false;
}

Material::Material(std::string n):
	name(n)
{
	shaderProgramRef = shaderProgram.Reference();
	compiled = false;
}


Material::~Material(void)
{
}


void Material::AddTexture(Texture t)
{
	textures.push_back(t);
	texRefs.push_back(t.Reference());
}

void Material::ReplaceTexture(Texture oldTex, Texture newTex)
{
	Texture defTex;
	if (oldTex.Reference() != defTex.Reference()) {
		std::vector<Texture>::iterator it;
		int i = 0;
		for (it = textures.begin(); it != textures.end(); it++) {
			if ((*it).Reference() == oldTex.Reference()) {
				*it = newTex;
				texRefs[i] = newTex.Reference();
				break;
			}
			i++;
		}
	}

	textures.push_back(newTex);
	texRefs.push_back(newTex.Reference());
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
	if(compiled == false) {
		shaderProgram.Compile();
		uniforms = shaderProgram.GetUniforms();
		shaderProgramRef = shaderProgram.Reference();
		compiled = true;
	}
}