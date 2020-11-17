//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-17                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "shader.h"
#include <fstream>
#include "defines.h"
#include "helpers/log.h"

Shader::Shader()
{

}
Shader::~Shader()
{

}

void Shader::createFromFiles(std::string vertexFileName, std::string fragmentFileName)
{
	std::string vertexString = readFile(vertexFileName);
	std::string fragmentString = readFile(fragmentFileName);
	std::string vertexCode = vertexString.c_str();
	std::string fragmentCode = fragmentString.c_str();

	compileShader(vertexCode, fragmentCode);
}

std::string Shader::readFile(std::string fileName)
{
	std::string content;
	std::ifstream fileStream(("src/shaders/"+fileName).c_str(), std::ios::in);

	if(!fileStream.is_open())
	{
		Log::error("Shader", "Failed to read " + fileName + "! File doesn't exist.");
		return "";
	}

	std::string line = "";
	while(!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line+"\n");
	}

	fileStream.close();
	return content;
}

void Shader::compileShader(std::string vertexCode, std::string fragmentCode)
{
	_shaderId = glCreateProgram();

	if(!_shaderId)
	{
		Log::error("Shader", "Error creating shader program!");
		return;
	}

	addShader(_shaderId, vertexCode, GL_VERTEX_SHADER);
	addShader(_shaderId, fragmentCode, GL_FRAGMENT_SHADER); 

	GLint result = 0;
	GLchar eLog[1024] = {0};

	glLinkProgram(_shaderId);
	glGetProgramiv(_shaderId, GL_LINK_STATUS, &result);
	if(!result)
	{
		glGetProgramInfoLog(_shaderId, sizeof(eLog), NULL, eLog);
		Log::error("Shader", "Error linking program: '" + std::string(eLog)+ "'");
		return;
	}

	glValidateProgram(_shaderId);
	glGetProgramiv(_shaderId, GL_VALIDATE_STATUS, &result);
	if(!result)
	{
		glGetProgramInfoLog(_shaderId, sizeof(eLog), NULL, eLog);
		Log::error("Shader", "Error validating program: '" + std::string(eLog)+ "'");
		return;
	}

	//---------- Get uniforms ----------//
	_uniformModel = glGetUniformLocation(_shaderId, "model");
}

void Shader::useShader()
{
	glUseProgram(_shaderId);
}

void Shader::addShader(GLuint program, std::string shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode.c_str();

	GLint codeLength[1];
	codeLength[0] = shaderCode.size();

	glShaderSource(shader, 1, code, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = {0};

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(!result)
	{
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		Log::error("Shader", "Error compiling the " + std::to_string(shaderType)+ " shader: '" + std::string(eLog)+ "'");
		return;
	}

	glAttachShader(program, shader);
}
