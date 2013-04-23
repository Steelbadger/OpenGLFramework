#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	The shader class represents a single shader source file		//
//	handles parsing and loading of the source file				//
//////////////////////////////////////////////////////////////////
//	The shaderprogram class represents a complete shader		//
//	program, this is the primary object used for manipulating	//
//	the shaders and material functionality of an object			//
//////////////////////////////////////////////////////////////////

#include "glew.h"
#include "texture.h"
#include "uniformlocations.h"

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
	UniformLocations GetUniforms(){return uniforms;}	
private:
	GLuint programRef;
	std::vector<Shader> shaders;
	UniformLocations uniforms;

	void FindUniformLocations();
};

