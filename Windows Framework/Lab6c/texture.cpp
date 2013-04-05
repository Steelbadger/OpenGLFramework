#include "texture.h"


int Texture::UNIQUE_ID_COUNTER = 0;
std::map<int, GLuint> Texture::TextureLibrary;

Texture::Texture(void) : texRef(0), created(false), uid(UNIQUE_ID_COUNTER++)
{
}

Texture::Texture(Type t, std::string p) : texRef(0), created(false), uid(UNIQUE_ID_COUNTER++)
{
	SetTexture(t, p);
}

Texture::Texture(Type t, unsigned short* base, unsigned int length) : texRef(0), created(false), uid(UNIQUE_ID_COUNTER++)
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
	wrapping = GL_REPEAT;
	magnitude = 0;
	created = true;
	TextureLibrary[uid] = texRef;
}

void Texture::SetTexture(Type t, unsigned short* base, unsigned int length)
{
	type = t;
	path = "POINTER";
	imgBase = base;
	imgSize = length;

	//GLuint TexID;

	//glGenTextures(1, &TexID);				// Create The Texture
	//glBindTexture(GL_TEXTURE_2D, TexID);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, length, length, 0, GL_RGBA, GL_UNSIGNED_SHORT, base);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//texRef = TexID;
	wrapping = GL_CLAMP;
	magnitude = 80;
}

void Texture::SetDisplacementMap(unsigned short* base, unsigned int length, float m)
{
	SetTexture(Type::DISPLACEMENT, base, length);
	magnitude = m;
}

void Texture::Apply(int targetUnit, GLuint targetLoc)
{
	if (texRef == 0) {
		if (TextureLibrary.count(uid) == 0) {
			GLuint TexID;

			glGenTextures(1, &TexID);				// Create The Texture
			glBindTexture(GL_TEXTURE_2D, TexID);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, imgSize, imgSize, 0, GL_RGBA, GL_UNSIGNED_SHORT, imgBase);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			delete[] imgBase;

			texRef = TexID;
		//	wrapping = GL_CLAMP;
		//	magnitude = 80;
			created = true;
			TextureLibrary[uid] = texRef;
		} else {
			texRef = TextureLibrary[uid];
		}
	}

	glActiveTexture(targetUnit+GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texRef);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
	glUniform1i(targetLoc, targetUnit);
}