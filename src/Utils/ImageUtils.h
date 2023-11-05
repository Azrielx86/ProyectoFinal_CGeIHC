//
// Created by edgar on 11/5/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_IMAGEUTILS_H
#define PROYECTOFINAL_CGEIHC_IMAGEUTILS_H

#include <fstream>
#include <iostream>
#include <string>

namespace Utils
{
/**
 * Image Utils to determine image color channel type (only for PNG, maybe TGA if it´s necessary...)
 * TGA have the channel info in the byte 17.
 * https://en.wikipedia.org/wiki/Truevision_TGA
 */
class ImageUtils
{
  public:
	enum CHANNEL_TYPE
	{
		RGB,
		RGBA,
		ERROR
	};

	ImageUtils() = delete;
	void operator=(ImageUtils &) = delete;
	~ImageUtils() = delete;

	/**
	 * Determines the color channel of a image.
	 * For more information: https://en.wikipedia.org/wiki/PNG
	 * @param path Image path
	 * @return Enum value (See CHANNEL_TYPE) with color channel type.
	 */
	static CHANNEL_TYPE GetPngChannel(const std::string &path);

  private:
	/**
	 * Reports an error.
	 * @param error Error type.
	 * @param path Image path.
	 * @return CHANNEL_TYPE::ERROR.
	 */
	static CHANNEL_TYPE ReportError(const std::string &error, const std::string &path);
};

} // namespace Utils

#endif // PROYECTOFINAL_CGEIHC_IMAGEUTILS_H
