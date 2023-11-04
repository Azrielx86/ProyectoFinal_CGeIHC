//
// Created by edgar on 10/1/2023.
//

#include "Shader.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
Shader::Shader()
{
	shaderProgramId = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformColor = 0;
	uniformEyePosition = 0;
	uniformView = 0;
	vertexCode = nullptr;
	fragmentCode = nullptr;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "LocalValueEscapesScope"
void Shader::loadShader(const char *vertex, const char *frag)
{
	std::string vertexString = readFile(vertex);
	std::string fragString = readFile(frag);
	this->vertexCode = vertexString.c_str();
	this->fragmentCode = fragString.c_str();
	compileShader();
}
#pragma clang diagnostic pop

std::string Shader::readFile(const char *path)
{
	std::string stream;
	std::ifstream ifstream(path, std::ios::in);

	if (!ifstream.is_open())
	{
		std::string message = "File does not exists: ";
		message += std::string(path);
		std::cerr << message;
		return stream;
	}

	std::string line;
	while (!ifstream.eof())
	{
		std::getline(ifstream, line);
		stream.append(line + "\n");
	}

	ifstream.close();
	return stream;
}
void Shader::compileShader()
{
	this->shaderProgramId = glCreateProgram();
	if (!shaderProgramId)
	{
		std::cerr << "Canno't create GL Sahder Program.\n";
		return;
	}

	addShader(shaderProgramId, vertexCode, GL_VERTEX_SHADER);
	addShader(shaderProgramId, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result;
	GLchar eLog[1024] = {0};
	glLinkProgram(shaderProgramId);
	glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderProgramId, sizeof(eLog), nullptr, eLog);
		std::cerr << "Error linking the shader: " << eLog << "\n";
		return;
	}

	glValidateProgram(shaderProgramId);
	glGetProgramiv(shaderProgramId, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderProgramId, sizeof(eLog), nullptr, eLog);
		std::cerr << "Error validating the shader: " << eLog << "\n";
		return;
	}
	
	uniformProjection = glGetUniformLocation(shaderProgramId, "projection");
	uniformModel = glGetUniformLocation(shaderProgramId, "model");
	uniformView = glGetUniformLocation(shaderProgramId, "view");
	uniformDirectionalLight.uniformcolor = glGetUniformLocation(shaderProgramId, "directionalLight.base.color");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderProgramId, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderProgramId, "directionalLight.direction");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderProgramId, "directionalLight.base.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderProgramId, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderProgramId, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderProgramId, "eyePosition");
	uniformColor = glGetUniformLocation(shaderProgramId, "color");
	uniformTextureOffset = glGetUniformLocation(shaderProgramId, "toffset");

	uniformPointLightCount = glGetUniformLocation(shaderProgramId, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = {'\0'};

		snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.color", i);
		uniformPointLight[i].uniformcolor = glGetUniformLocation(shaderProgramId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderProgramId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderProgramId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderProgramId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderProgramId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderProgramId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderProgramId, locBuff);
	}

	uniformSpotLightCount = glGetUniformLocation(shaderProgramId, "spotLightCount");

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = {'\0'};

		snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].base.base.color", i);
		uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderProgramId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderProgramId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderProgramId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderProgramId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderProgramId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderProgramId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderProgramId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderProgramId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderProgramId, locBuff);
	}
}

void Shader::addShader(GLuint program, const char *shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	const GLchar *code[1];
	code[0] = shaderCode;

	GLint codeLenght[1];
	codeLenght[0] = (GLint) strlen(shaderCode);

	glShaderSource(shader, 1, code, codeLenght);
	glCompileShader(shader);

	GLint result;
	GLchar eLog[1024] = {0};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(shader, sizeof(eLog), nullptr, eLog);
		std::cerr << "Error compiling the shader: " << eLog << "\n";
		return;
	}

	glAttachShader(program, shader);
}

void Shader::useProgram() const { glUseProgram(shaderProgramId); }

Shader::~Shader()
{
	if (shaderProgramId != 0)
	{
		glDeleteProgram(shaderProgramId);
		shaderProgramId = 0;
	}
	uniformColor = uniformModel = uniformProjection = uniformView = 0;
}
void Shader::SetDirectionalLight(Lights::DirectionalLight *dLight)
{
	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformcolor,
	                 uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}
GLuint Shader::getUniformPointLightCount() const
{
	return uniformPointLightCount;
}
GLuint Shader::getUniformSpotLightCount() const
{
	return uniformSpotLightCount;
}

GLuint Shader::getShaderProgramId() const
{
	return shaderProgramId;
}
GLuint Shader::getUniformProjection() const
{
	return uniformProjection;
}
GLuint Shader::getUniformModel() const
{
	return uniformModel;
}
GLuint Shader::getUniformColor() const
{
	return uniformColor;
}
GLuint Shader::getUniformView() const
{
	return uniformView;
}
GLuint Shader::getUniformEyePosition() const
{
	return uniformEyePosition;
}
GLuint Shader::getUniformSpecularIntensity() const
{
	return uniformSpecularIntensity;
}
GLuint Shader::getUniformShininess() const
{
	return uniformShininess;
}
GLuint Shader::getUniformTextureOffset() const
{
	return uniformTextureOffset;
}
void Shader::SetPointLights(Lights::PointLight *pLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformcolor,
		                   uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
		                   uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
	}
}
void Shader::SetSpotLights(Lights::SpotLight *sLight, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColour,
		                   uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
		                   uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
		                   uniformSpotLight[i].uniformEdge);
	}
}

#pragma clang diagnostic pop