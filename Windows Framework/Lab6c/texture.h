#pragma once
#include "glew.h"
#include <string>
#include <map>
#include "tex.h"


class Texture
{
public:
	enum Type {DIFFUSE, BUMP, NORMAL, DISPLACEMENT};

	Texture(void);
	Texture(Type t, std::string path);
	Texture(Type t, unsigned short* base, unsigned int length);
	~Texture(void);
	void SetTexture(Type t, std::string path);
	void SetTexture(Type t, unsigned short* base, unsigned int length);
	GLuint Reference(){return texRef;}
private:
	Type type;
	std::string path;
	GLuint texRef;


	static std::map<std::string, GLuint> TextureLibrary;
};

