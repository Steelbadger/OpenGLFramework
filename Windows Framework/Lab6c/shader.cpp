#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::map<std::string, GLuint> Shader::ShaderLibrary;

Shader::Shader(void)
{
}

Shader::Shader(std::string s)
{
	Create(s);
}

Shader::~Shader(void)
{
}

void Shader::Create(std::string fileName)
{
	std::string fn = fileName;
	if (fn.empty()) {
		return;
	}

	//  Check we haven't already done this file
	if (!ShaderLibrary.count(fn)) {
		GLuint shader;

		//  Get the file extension so that we know what kind of shader it is
		std::string type = fn.substr(fn.find_last_of(".") + 1);

		//  depending on extension, create a shader ref of the corresponding type
		if (type == "vertexshader") {
			shader = glCreateShader(GL_VERTEX_SHADER);
		} else if (type == "fragmentshader") {
			shader = glCreateShader(GL_FRAGMENT_SHADER);
		} else if (type == "tesscontrol") {
			shader = glCreateShader(GL_TESS_CONTROL_SHADER);
		} else if (type == "tessevaluation") {
			shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
		} else {
			//  If the file extension is not correct then return an error and stop
			std::cout << "Unrecognised Shader File Extension: " << type << std::endl;
			return;
		}

		std::string code;
		std::ifstream fileStream(fn, std::ios::in);
		if (!fileStream.is_open()) {
			std::cout << "Could not open shader file : " << fileName << std::endl;
			return;
		}
		//  Load in the code to a stream
		std::string line = "";
		while(getline(fileStream, line)) {
			code += "\n" + line;
		}
		fileStream.close();

		//  compile that code
		std::cout << "Compiling " << fn << std::endl;
		char const * codePointer = code.c_str();
		glShaderSource(shader, 1, &codePointer, NULL);
		glCompileShader(shader);


		//  Check GLSL compiler output for errors
		GLint result = GL_FALSE;
		int logLength;

		// Check Vertex Shader
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> ShaderErrorMessage(logLength);
		glGetShaderInfoLog(shader, logLength, NULL, &ShaderErrorMessage[0]);
		std::string output(ShaderErrorMessage.begin(), ShaderErrorMessage.end());
		std::cout << output << std::endl;


		//  If we failed to compile the shader quit without adding the the shadermap
		if (result == GL_FALSE) {
			return;
		}

		ShaderLibrary[fn] = shader;
	}
}



ShaderProgram::ShaderProgram()
{
	programRef = glCreateProgram();
}


ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::AddShader(Shader s)
{
	shaders.push_back(s);
	glAttachShader(programRef, s.Reference());
}

void ShaderProgram::Compile()
{
	glLinkProgram(programRef);

	//  Check the infolog for errors
	GLint result = GL_FALSE;
	int logLength;

	glGetProgramiv(programRef, GL_LINK_STATUS, &result);
	glGetProgramiv(programRef, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> ProgramErrorMessage(logLength);
	glGetProgramInfoLog(programRef, logLength, NULL, &ProgramErrorMessage[0]);
	std::string output(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
	std::cout << output << std::endl;

	FindUniformLocations();
}

void ShaderProgram::FindUniformLocations()
{
	UniformLocations newLocations;

	newLocations.ProjectionMatrix = glGetUniformLocation(programRef, "projectionMatrix");
	newLocations.ViewMatrix = glGetUniformLocation(programRef, "viewMatrix");
	newLocations.ModelMatrix = glGetUniformLocation(programRef, "modelMatrix");
	newLocations.ModelViewMatrix = glGetUniformLocation(programRef, "modelViewMatrix");
	newLocations.NormalMatrix = glGetUniformLocation(programRef, "normalMatrix");

	newLocations.NumLights = glGetUniformLocation(programRef, "numLights");
	newLocations.MapWidth = glGetUniformLocation(programRef, "mapWidth");
	newLocations.Magnitude = glGetUniformLocation(programRef, "magnitude");

	std::string nameString;
	std::stringstream stream;

	for (int i = 0; i < LightSource::MAXLIGHTS; i++) {
		//  Create the string to search for; eg. lights[0].colour by combining the counter and string concatenation
		stream << "lights[" << i << "].colour";

		//  Get the created string out of the stringstream
		nameString = stream.str();

		//  Clear the stringstream for next usage
		stream.str(std::string());

		newLocations.LightColours[i] = glGetUniformLocation(programRef, nameString.c_str());

		stream << "lights[" << i << "].position";
		nameString = stream.str();
		stream.str(std::string());

		newLocations.LightPositions[i] = glGetUniformLocation(programRef, nameString.c_str());

		stream << "lights[" << i << "].type";
		nameString = stream.str();
		stream.str(std::string());

		newLocations.LightTypes[i] = glGetUniformLocation(programRef, nameString.c_str());
	}

	newLocations.Texture1 = glGetUniformLocation(programRef, "texture1");
	newLocations.Texture2 = glGetUniformLocation(programRef, "texture2");
	newLocations.Texture3 = glGetUniformLocation(programRef, "texture3");
	newLocations.Texture4 = glGetUniformLocation(programRef, "texture4");
		
	uniforms = newLocations;
}