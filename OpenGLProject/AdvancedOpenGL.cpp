#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "LoadingTexture.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Camera cam(glm::vec3(0.0f, 0.0f, 20.0f));
float ratio = (float)4 / 3;
int main() {
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

	Shader shader("GLSL/Model/Vertex.vs", "GLSL/Model/Fragment.fs");
	Model castle("Model/castle/castle.obj");

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shader.use();
		glm::mat4 model=glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
		model = glm::scale(model,glm::vec3(0.12f));
		glm::mat4 projection;
		projection=glm::perspective(glm::radians(45.0f),ratio,0.1f,100.0f);
		shader.setMat4fv("view", 1, GL_FALSE, cam.getView());
		shader.setMat4fv("projection", 1, GL_FALSE, projection);
		shader.setMat4fv("model", 1, GL_FALSE, model);
		castle.Draw(shader);
		glfwSwapBuffers(window);
	}


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	ratio = (float)width / height;
	glViewport(0, 0, width, height);

}