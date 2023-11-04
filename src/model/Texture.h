//
// Created by edgar on 11/3/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_TEXTURE_H
#define PROYECTOFINAL_CGEIHC_TEXTURE_H
#include "../GlobalConstants.h"
#include <GL/glew.h>
#include <iostream>
#include <string>

namespace Model
{

class Texture
{
  public:
	Texture();
	explicit Texture(const char *path);
	~Texture();
	bool LoadTexture(bool alpha = false);
	void UseTexture() const;
	void ClearTexture();

  private:
	GLuint textureId;
	int width, height, bitDepth;
	std::string path;
};

} // namespace Model

#endif // PROYECTOFINAL_CGEIHC_TEXTURE_H
