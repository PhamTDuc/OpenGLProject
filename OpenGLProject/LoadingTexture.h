#ifndef LOADING_TEXTURE_H
#define LOADING_TEXTURE_H
#include "stb_image.h"
#include <iostream>
#include "string"
#include "vector"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
unsigned int loadTexture(const std::string &path, bool colorData=true);
unsigned int loadCubemap(const std::vector<std::string> &faces);
unsigned int loadHDR(const std::string &path);
#endif