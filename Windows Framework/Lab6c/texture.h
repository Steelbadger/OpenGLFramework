#pragma once

#include "glew.h"
#include "tex.h"

#include <string>
#include <map>


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
	void SetDisplacementMap(unsigned short* base, unsigned int length, float magnitude);
	void Apply(int texUnit, GLuint target);
	Type GetType(){return type;}
	float GetDisplacementMagnitude(){return (type == DISPLACEMENT? magnitude : 0.0f);}
	GLuint Reference(){return texRef;}
private:
	Type type;
	std::string path;
	GLuint texRef;
	float magnitude;
	bool created;
	unsigned short* imgBase;
	int imgSize;
	int uid;

	GLenum wrapping;

	static std::map<int, GLuint> TextureLibrary;
	static int UNIQUE_ID_COUNTER;
};

