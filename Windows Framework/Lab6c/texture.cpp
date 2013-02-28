#include "texture.h"
#include "tex.h"


std::map<std::string, GLuint> Texture::TextureLibrary;

Texture::Texture(void)
{
	texType = NONE;
	wrapping = GL_REPEAT;
	magnitude = 0;
}


Texture::~Texture(void)
{
}

void Texture::Create(Type t, std::string filename)
{
	texType = t;
	path = filename;

	if (!TextureLibrary.count(filename)) {
		CreateGLTexture(filename.c_str(), tex);
		TextureLibrary[filename] = tex;
	} else {
		tex = TextureLibrary[filename];
	}
}


void Texture::Create(Type t, unsigned short* basePtr, unsigned int pixelSize)
{
	if (basePtr != NULL) {
		texType = t;
		path = "POINTER";

		GLuint TexID;

		glGenTextures(1, &TexID);				// Create The Texture
		glBindTexture(GL_TEXTURE_2D, TexID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, pixelSize, pixelSize, 0, GL_RGBA, GL_UNSIGNED_SHORT, basePtr);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		tex = TexID;
		free(basePtr);
		basePtr = NULL;
	}
}

void Texture::Create(Type t, GLuint texH)
{
	texType = t;
	path = "POINTER";
	tex = texH;
}

TexStruct Texture::GetTexture()
{
	TexStruct out;
	out.tex = tex;
	out.type = texType;
	out.wrapping = wrapping;
	out.magnitude = magnitude;
	return out;
}

