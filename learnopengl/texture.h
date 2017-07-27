#pragma once

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <stb_image.h>

class Texture
{
public:
	Texture(const std::string texFilename, GLenum format)
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		unsigned char* image = stbi_load(texFilename.c_str(), &width, &height, &channels, 0);

		if (image)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load image" << std::endl;
		}
		stbi_image_free(image);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void bind(GLenum texture)
	{
		glActiveTexture(texture);
		glBindTexture(GL_TEXTURE_2D, ID);
	}

private:
	unsigned int ID;
};
