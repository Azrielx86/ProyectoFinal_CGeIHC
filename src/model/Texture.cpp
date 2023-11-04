//
// Created by edgar on 11/3/2023.
//

#include "Texture.h"

namespace Model
{
Texture::Texture()
{
	textureId = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	path = "";
}

Texture::Texture(const char *path)
{
	textureId = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	this->path = path;
}

bool Texture::LoadTexture()
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char *texData = stbi_load(path.c_str(), &width, &height, &bitDepth, STBI_rgb_alpha);
	if (!texData)
	{
		std::cerr << "Texture not found: " << path << "\n";
		return false;
	}

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texData);

	return true;
}

void Texture::UseTexture() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureId);
	textureId = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	path = "";
}

Texture::~Texture()
{
	ClearTexture();
}

} // namespace Model