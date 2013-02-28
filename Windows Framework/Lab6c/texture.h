#pragma once
#include <string>
#include <map>
#include "glew.h"

struct TexStruct;

class Texture
{
public:
	enum Type {NONE, DIFFUSE, BUMP, NORMAL, DISPLACEMENT};
	Texture(void);
	void Create(Type t, std::string filename);
	void Create(Type t, unsigned short* basePtr, unsigned int pixelSize);
	void Create(Type t, GLuint texH);
	void SetMagnitude(float m){magnitude = m;}
	void SetWrapping(GLenum wrapType){wrapping = wrapType;}
	~Texture(void);

	TexStruct GetTexture();

private:
	Type texType;
	std::string path;
	GLuint tex;
	GLenum wrapping;
	float magnitude;

	static std::map<std::string, GLuint> TextureLibrary;
};

struct TexStruct
{
	GLuint tex;
	Texture::Type type;
	GLenum wrapping;
	float magnitude;
};
