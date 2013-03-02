#pragma once
#include "glew.h"
#include <string>
#include <map>
#include "tex.h"


class Texture
{
public:
	enum Type {DIFFUSE, BUMP, NORMAL, DISPLACEMENT};
	const static int MAX_TEXTURES = 16;

	Texture(void);
	Texture(Type t, std::string path);
	Texture(Type t, unsigned short* base, unsigned int length);
	~Texture(void);
	void SetTexture(Type t, std::string path);
	void SetTexture(Type t, unsigned short* base, unsigned int length);
	void Apply(int texUnit, GLuint target);
	GLuint Reference(){return texRef;}
private:
	Type type;
	std::string path;
	GLuint texRef;

	GLenum wrapping;

	static std::map<std::string, GLuint> TextureLibrary;
};

