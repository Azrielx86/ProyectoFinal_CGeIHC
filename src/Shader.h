//
// Created by edgar on 10/1/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_SHADER_H
#define PROYECTOFINAL_CGEIHC_SHADER_H

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <gl/glew.h>
#include <iostream>
#include <sstream>
#include <string>

#ifdef __linux
#include <cstring>
#endif // __linux

class Shader
{
  public:
	Shader();
	virtual ~Shader();
	void loadShader(const char *vertex, const char *frag);
	GLuint getShaderProgramId() const;
	GLuint getUniformProjection() const;
	GLuint getUniformModel() const;
	GLuint getUniformColor() const;
	GLuint getUniformView() const;
	GLuint getUniformEyePosition() const;
	void useProgram() const;

  private:
	GLuint shaderProgramId, uniformProjection, uniformModel, uniformColor, uniformView, uniformEyePosition;
	const char *vertexCode;
	const char *fragmentCode;
	void compileShader();
	static std::string readFile(const char *path);
	void addShader(GLuint program, const char *shaderCode, GLenum shaderType);
};

#endif // PROYECTOFINAL_CGEIHC_SHADER_H
