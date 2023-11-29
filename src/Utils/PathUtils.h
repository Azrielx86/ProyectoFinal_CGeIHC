//
// Created by edgar on 11/3/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_PATHUTILS_H
#define PROYECTOFINAL_CGEIHC_PATHUTILS_H

#include <string>

#define MODELS_PATH "assets/Models"
#define TEXTURES_PATH "assets/Textures"
#define AUDIO_PATH "assets/Audios"

namespace Utils
{
class PathUtils
{
  public:
	PathUtils() = delete;
	static std::string getTexturesPath() { return TEXTURES_PATH; }
	static std::string getModelsPath() { return MODELS_PATH; }
	static std::string getAudioPath() { return AUDIO_PATH; }
};
} // namespace Utils

#endif // PROYECTOFINAL_CGEIHC_PATHUTILS_H
