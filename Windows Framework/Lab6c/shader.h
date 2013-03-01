#pragma once
#include "glew.h"
#include <string>
#include <map>
#include <vector>

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
	
private:
	GLuint programRef;
	std::vector<Shader> shaders;

	static std::map<std::string, GLuint> ProgramLibrary;
};

