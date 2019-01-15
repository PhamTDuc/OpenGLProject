#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
float ratio = (float)4 / 3;
Camera cam(glm::vec3(1.0f, 1.0f, 11.0f));
int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMpAT, GL_TRUE);

#endif // __APPLE__
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	
	// Setting Shading
	// Setting Shading
	Shader lightShading("Debug/ShadingCode/LightingShading/Vert.txt", "Debug/ShadingCode/LightingShading/Frag.txt");
	Shader modelShading("Debug/ShadingCode/ModelShading/Vert.txt", "Debug/ShadingCode/ModelShading/Frag.txt");
	Shader loadShading("Debug/ShadingCode/loadModel/Vert.vs","Debug/ShadingCode/loadModel/Frag.fs");
	
	Model model("D:/Blender/Nanosuit/nanosuit.obj");
	while (!glfwWindowShouldClose(window))
	{
		//Render here
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		model.Draw(modelShading);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	ratio = (float)width / height;
	glViewport(0, 0, width, height);

}

