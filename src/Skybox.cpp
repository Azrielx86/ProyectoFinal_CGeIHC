#include "Skybox.h"

Skybox::Skybox()
{
}

Skybox::Skybox(std::vector<std::string> faceLocations)
{
	skyShader = new Shader();
	skyShader->loadShader("shaders/skybox.vert", "shaders/skybox.frag");
	uniformProjection = skyShader->getUniformProjection();
	uniformView = skyShader->getUniformView();

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	int width, height, bitDepth;
	for (size_t i = 0; i < 6; i++)
	{
		auto alpha = Utils::ImageUtils::GetPngChannel(faceLocations[i].c_str()) == Utils::ImageUtils::CHANNEL_TYPE::RGBA;     // true si es RGBA
		unsigned char *texData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, alpha ? STBI_rgb_alpha : 0); // el tipo unsigned char es para un array de bytes de la imagen, obtener datos de la imagen
		if (!texData)
		{
			std::cerr << "File not found: " << faceLocations[i] << "\n";
			return;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, texData); // SIN CANAL ALPHA A ENOS QUE QUERAMOS AGREGAR EFECTO DE PARALLAX
		stbi_image_free(texData);                                                                                                          // para liberar la informaci�n de la imagen
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// clang-format off
	// Creando el Mesh del skybox
	unsigned int skyboxIndices[] = {
	    // front
	    0, 1, 2,
	    2, 1, 3,
	    // right
	    2, 3, 5,
	    5, 3, 7,
	    // back
	    5, 7, 4,
	    4, 7, 6,
	    // left
	    4, 6, 0,
	    0, 6, 1,
	    // top
	    4, 0, 5,
	    5, 0, 2,
	    // bottom
	    1, 6, 3,
	    3, 6, 7};

	float skyboxVertices[] = {
	    -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // v0
	    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // V1
	    1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // V2
	    1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // V3
	    -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // V4
	    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // V5
	    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // V6
	    1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f    // V7
	};
	// clang-format on
	skyMesh = new Model::Mesh();
	skyMesh->createMesh(skyboxVertices, skyboxIndices, 64, 36);
}

void Skybox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));

	glDepthMask(false);
	skyShader->useProgram();
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	// skyShader->Validate();
	skyMesh->render();
	glDepthMask(true);
}

Skybox::~Skybox()
{
}
