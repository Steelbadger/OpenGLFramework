#pragma once
#include "glew.h"
#include "shader.h"
#include "uniformlocations.h"

#include <vector>
#include <map>

class Shader;
class Texture;

class Material
{
public:
	Material(void);
	Material(std::string name);
	~Material(void);

	void AddTexture(Texture t);
	void AddShader(std::string shader);
	
	std::vector<Texture> GetTextures();
	std::vector<Shader> GetShaders();
	GLuint GetShaderProgram(){return shaderProgramRef;}
	UniformLocations GetUniforms();


	void Apply();
	void Compile();

	void AddToMaterialLibrary(std::string name);
	static Material FetchMaterial(std::string name);

private:

	std::vector<Texture> textures;
	std::vector<GLuint> texRefs;

	UniformLocations uniforms;

	std::vector<Shader> shaders;
	ShaderProgram shaderProgram;
	GLuint shaderProgramRef;
	std::string name;

	static std::map<std::string, Material> MaterialLibrary;
};

