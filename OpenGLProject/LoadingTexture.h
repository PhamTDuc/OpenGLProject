#ifndef LOADING_TEXTURE_H
#define LOADING_TEXTURE_H
#include "stb_image.h"
#include <iostream>
#include "string"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
unsigned int loadTexture(std::string path);
#endif