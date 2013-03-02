#pragma once
#include "glew.h"
#include <string>
#include <map>
#include <vector>
#include "lights.h"
#include "texture.h"

struct UniformLocations;

class Shader
{
public:
	Shader(void);
	Shader(std::string);
	void Create(std::string);
	GLuint Reference(){return shaderRef;}
	~Shader(void);
private:
	GLuint shaderRef;
	static std::map<std::string, GLuint> ShaderLibrary;
};

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	void AddShader(Shader s);
	void Compile();
	GLuint Reference(){return programRef;}
	UniformLocations GetUniforms(){return uniforms;}	
private:
	GLuint programRef;
	std::vector<Shader> shaders;
	UniformLocations uniforms;

	void FindUniformLocations();
};

struct UniformLocations
{
	GLuint ProjectionMatrix;
	GLuint ViewMatrix;
	GLuint ModelMatrix;
	GLuint ModelViewMatrix;
	GLuint NormalMatrix;
	GLuint LightPositions[LightSource::MAXLIGHTS];
	GLuint LightColours[LightSource::MAXLIGHTS];
	GLuint NumLights;
	GLuint LightTypes[LightSource::MAXLIGHTS];
	GLuint Texture1;
	GLuint Texture2;
	GLuint Texture3;
	GLuint Texture4;
	GLuint Textures[Texture::MAX_TEXTURES];
	GLuint MapWidth;
	GLuint Magnitude;
};

