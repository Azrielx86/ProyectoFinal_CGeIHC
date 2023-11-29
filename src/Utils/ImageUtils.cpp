//
// Created by edgar on 11/5/2023.
//

#include "ImageUtils.h"

namespace Utils
{
ImageUtils::CHANNEL_TYPE ImageUtils::GetPngChannel(const std::string &path)
{
	std::ifstream file(path, std::ios::binary);
	if (!file)
	{
		file.close();
		return ReportError("Error reading file: ", path);
	}

	char header[8];
	file.read(header, 8);
	if (file.gcount() != 8)
	{
		file.close();
		return ReportError("Error getting the header: ", path);
	}

	/**
	 * Color Channel is in the IHDR Chunk.
	 */
	if (header[1] == 'P' || header[2] == 'N' || header[3] == 'G')
	{
		char ihdr[13];
		bool ihdrFound = false;
		while (!ihdrFound)
		{
			char chunk_size[4];
			file.read(chunk_size, 4);
			if (file.gcount() != 4)
			{
				file.close();
				return ReportError("Error reading chunk length of file.", path);
			}

			char chunk_type[5];
			file.read(chunk_type, 4);
			if (file.gcount() != 4)
			{
				file.close();
				return ReportError("Error reading chunk type of file.", path);
			}

			chunk_type[4] = '\0';

			if (std::string(chunk_type) == "IHDR")
			{
				file.read(ihdr, 13);
				if (file.gcount() != 13)
				{
					file.close();
					return ReportError("Error reading IHDR chunk of file.", path);
				}
				ihdrFound = true;
			}
			else
			{
				char chunk_data[4];
				file.read(chunk_data, 4);
				if (file.gcount() != 4)
				{
					file.close();
					return ReportError("Error reading chunk data of file.", path);
				}
				file.seekg((int) file.tellg() + std::stoi(std::string(chunk_data)) + 4);
			}
		}

		/**
		 * 0 - Grayscale
		 * 2 - RGB Truecolor
		 * 3 - Indexed
		 * 4 - Grayscale and alpha
		 * 6- RGBA
		 */
		if (ihdr[9] == 2)
		{
#ifdef PRINT_IMAGE_RESULT
			std::cout << "[ " << typeid(ImageUtils).name() << " ] PNG image have RGB channel...\n";
#endif
			file.close();
			return CHANNEL_TYPE::RGB;
		}
		else if (ihdr[9] == 6)
		{
#ifdef PRINT_IMAGE_RESULT
			std::cout << "[ " << typeid(ImageUtils).name() << " ] PNG image have RGBA channel.\n";
#endif
			file.close();
			return CHANNEL_TYPE::RGBA;
		}
	}
	else
	{
		std::cerr << typeid(ImageUtils).name() << " Something went wrong... \n";
	}

	file.close();
	return ImageUtils::ERROR;
}

ImageUtils::CHANNEL_TYPE ImageUtils::ReportError(const std::string &error, const std::string &path)
{
	std::cerr << "[ " << typeid(ImageUtils).name() << " ] " << error << path << '\n';
	return ImageUtils::ERROR;
}

} // namespace Utils