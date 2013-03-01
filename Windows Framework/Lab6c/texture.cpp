#include "texture.h"


Texture::Texture(void)
{
}

Texture::Texture(Type t, std::string p)
{
	SetTexture(t, p);
}

Texture::Texture(Type t, unsigned short* base, unsigned int length)
{
	SetTexture(t, base, length);
}


Texture::~Texture(void)
{
}

void Texture::SetTexture(Type t, std::string p)
{
	type = t;
	path = p;
	GLuint tex;
	CreateGLTexture(p.c_str(), tex);
	texRef = tex;
}

void Texture::SetTexture(Type t, unsigned short* base, unsigned int length)
{
	type = t;
	path = "POINTER";

	GLuint TexID;

	glGenTextures(1, &TexID);				// Create The Texture
	glBindTexture(GL_TEXTURE_2D, TexID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, length, length, 0, GL_RGBA, GL_UNSIGNED_SHORT, base);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	if (base != NULL)
		free(base);

}