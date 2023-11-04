//
// Created by edgar on 10/1/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_SHADER_H
#define PROYECTOFINAL_CGEIHC_SHADER_H

#include "GlobalConstants.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <gl/glew.h>
#include <iostream>
#include <sstream>
#include <string>

#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"

#ifdef __linux
#include <cstring>
#endif // __linux

class Shader
{
  public:
	enum ShaderTypes
	{
		SKYBOX_SHADER,
		LIGHT_SHADER,
		MODEL_TEX_SHADER,
		BASE_SHADER
	};

	Shader();
	virtual ~Shader();
	void loadShader(const char *vertex, const char *frag);

	GLuint getUniformPointLightCount() const;
	GLuint getUniformSpotLightCount() const;
	GLuint getShaderProgramId() const;
	GLuint getUniformProjection() const;
	GLuint getUniformModel() const;
	GLuint getUniformColor() const;
	GLuint getUniformView() const;
	GLuint getUniformEyePosition() const;
	GLuint getUniformSpecularIntensity() const;
	GLuint getUniformShininess() const;
	GLuint getUniformTextureOffset() const;

	void SetDirectionalLight(Lights::DirectionalLight *dLight);
	void SetPointLights(Lights::PointLight *pLight, unsigned int lightCount);
	void SetSpotLights(Lights::SpotLight *sLight, unsigned int lightCount);
	void useProgram() const;

  private:
	struct
	{
		GLuint uniformcolor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount = 0;

	struct
	{
		GLuint uniformcolor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount = 0;

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	int pointLightCount = 0;
	int spotLightCount = 0;
	GLuint shaderProgramId, uniformProjection, uniformModel, uniformColor, uniformView, uniformEyePosition,
	    uniformSpecularIntensity, uniformShininess, uniformTextureOffset;
	const char *vertexCode;
	const char *fragmentCode;
	void compileShader();
	static std::string readFile(const char *path);
	void addShader(GLuint program, const char *shaderCode, GLenum shaderType);
};

#endif // PROYECTOFINAL_CGEIHC_SHADER_H
