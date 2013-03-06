#pragma once
#include "glew.h"
#include "defines.h"


struct UniformLocations
{
	GLuint ProjectionMatrix;
	GLuint ViewMatrix;
	GLuint ModelMatrix;
	GLuint ModelViewMatrix;
	GLuint NormalMatrix;
	GLuint LightPositions[MAXLIGHTS];
	GLuint LightColours[MAXLIGHTS];
	GLuint NumLights;
	GLuint LightTypes[MAXLIGHTS];
	GLuint Textures[MAX_TEXTURES];
	GLuint MapWidth;
	GLuint Magnitude;
	GLuint Time;
};
