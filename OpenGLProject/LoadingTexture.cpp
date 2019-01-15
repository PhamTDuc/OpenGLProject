#include "LoadingTexture.h"
unsigned int loadTexture(std::string path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, textureID);
	int width, height, nrChannels;

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	GLenum channels;
	if (data) {
		switch (nrChannels)
		{
		case 1:
			channels = GL_RED;
			break;
		case 3:
			channels = GL_RGB;
			break;
		case 4:
			channels = GL_RGBA;
		default:
			break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, channels, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else
	{
		std::cout << "Failed to load texture from:" << path << std::endl;
	}
	stbi_image_free(data);
	return textureID;
}