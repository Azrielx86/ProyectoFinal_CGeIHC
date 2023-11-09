//
// Created by edgar on 10/1/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_SHADER_H
#define PROYECTOFINAL_CGEIHC_SHADER_H

#include "GlobalConstants.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <GL/glew.h>
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
	Shader();
	virtual ~Shader();
	void loadShader(const char *vertex, const char *frag);

	[[nodiscard]] GLuint getUniformPointLightCount() const;
	[[nodiscard]] GLuint getUniformSpotLightCount() const;
	[[nodiscard]] GLuint getShaderProgramId() const;
	[[nodiscard]] GLuint getUniformProjection() const;
	[[nodiscard]] GLuint getUniformModel() const;
	[[nodiscard]] GLuint getUniformColor() const;
	[[nodiscard]] GLuint getUniformView() const;
	[[nodiscard]] GLuint getUniformEyePosition() const;
	[[nodiscard]] GLuint getUniformSpecularIntensity() const;
	[[nodiscard]] GLuint getUniformShininess() const;
	[[nodiscard]] GLuint getUniformTextureOffset() const;

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

	[[maybe_unused]] int pointLightCount = 0;
	[[maybe_unused]] int spotLightCount = 0;
	GLuint shaderProgramId, uniformProjection, uniformModel, uniformColor, uniformView, uniformEyePosition,
	    uniformSpecularIntensity, uniformShininess, uniformTextureOffset;
	const char *vertexCode;
	const char *fragmentCode;
	void compileShader();
	static std::string readFile(const char *path);
	void addShader(GLuint program, const char *shaderCode, GLenum shaderType);
};

#endif // PROYECTOFINAL_CGEIHC_SHADER_H
