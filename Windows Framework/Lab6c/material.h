#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	The material class which contains all material properties	//
//	of the mesh with which it is associated; the textures		//
//	and shader programs.										//
//////////////////////////////////////////////////////////////////

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
	void ReplaceTexture(Texture oldTex, Texture newTex);
	float GetDisplacementMagnitude(){return displacementMapMagnitude;}
	void ReplaceTexture(Texture::Type t, Texture newTex);
	
	std::vector<Texture> GetTextures();
	std::vector<Shader> GetShaders();
	GLuint GetShaderProgram(){return shaderProgramRef;}
	UniformLocations GetUniforms();

	void Apply();
	void Compile();

	void AddToMaterialLibrary();
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

	float displacementMapMagnitude;
	bool compiled;

	static std::map<std::string, Material> MaterialLibrary;
};

