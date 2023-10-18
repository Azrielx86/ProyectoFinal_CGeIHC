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

	glLinkProgram(shaderProgramId);

	GLint result;
	GLchar eLog[1024] = {0};
	glGetShaderiv(shaderProgramId, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shaderProgramId, sizeof(eLog), nullptr, eLog);
		std::cerr << "Error linking the shader: " << eLog << "\n";
		return;
	}

	uniformProjection = glGetUniformLocation(shaderProgramId, "projection");
	uniformModel = glGetUniformLocation(shaderProgramId, "model");
	uniformView = glGetUniformLocation(shaderProgramId, "view");
	uniformEyePosition = glGetUniformLocation(shaderProgramId, "eyePosition");
	uniformColor = glGetUniformLocation(shaderProgramId, "color");
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

GLuint Shader::getShaderProgramId() const { return shaderProgramId; }
GLuint Shader::getUniformProjection() const { return uniformProjection; }
GLuint Shader::getUniformModel() const { return uniformModel; }
GLuint Shader::getUniformColor() const { return uniformColor; }
GLuint Shader::getUniformView() const { return uniformView; }
GLuint Shader::getUniformEyePosition() const { return uniformEyePosition; }

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

#pragma clang diagnostic pop