#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "../../LoadingTexture.h"
#include "../../Shader.h"
#include "../../Camera.h"
#include "../../Model.h"
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
bool firstMouse = true;
float lastX = 0.0f, lastY = 0.0f;

float ratio = (float)4 / 3;
Camera cam(glm::vec3(0.0f, 1.0f, 5.0f));


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
	glfwSetCursorPosCallback(window, mouse_callback);




	float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f,  1.0f,
						  
		0.0f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  0.0f ,0.0f, 1.0f,  1.0f,  0.0f
	};
	unsigned int VAO[2], VBO[2];
	glGenBuffers(2, VBO);
	glGenVertexArrays(2, VAO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), &transparentVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

	glBindVertexArray(0);

	Model nanosuit("Model/Nanosuit/nanosuit.obj");

	Shader shade("GLSL/Model/Vertex.vs","GLSL/Model/NormalMap.fs");
	Shader light("GLSL/LightShade/Vertex.vs", "GLSL/LightShade/Fragment.fs");

	unsigned int diffuse=loadTexture("Texture/brickwall.jpg");
	unsigned int normal = loadTexture("Texture/brickwall_normal.jpg");


	
	glm::vec3 lightPos(0.5f,0.2f,0.1f);
	shade.use();
	shade.setVec3("light.pos", lightPos);
	shade.setVec3("light.ambient", glm::vec3(0.4f));
	shade.setVec3("light.diffuse", glm::vec3(1.0f));
	shade.setVec3("light.specular", glm::vec3(1.0f));


	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		processInput(window);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection= glm::perspective(glm::radians(35.0f), ratio, 0.1f, 1000.0f);

		shade.use();
		glm::mat4 model(1.0f);
		shade.setMat4fv("view", 1,GL_FALSE,cam.getView());
		shade.setMat4fv("projection",1,GL_FALSE,projection);
		shade.setMat4fv("model",1, GL_FALSE, model);
		shade.setVec3("viewPos", cam.getPos());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse);
		shade.setInt("diffuse1", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal);
		shade.setInt("normalMap1", 1);

		glBindVertexArray(VAO[0]);
		//glDrawArrays(GL_TRIANGLES, 0,6);




		model = glm::mat4(1.0f);
		model = glm::translate(model,lightPos);
		model = glm::scale(model, glm::vec3(0.05f));
		light.use();
		light.setMat4fv("view", 1, GL_FALSE, cam.getView());
		light.setMat4fv("projection", 1, GL_FALSE, projection);
		light.setMat4fv("model", 1, GL_FALSE, model);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::translate(model, glm::vec3(2.0f, -5.0f, 0.0f));
		shade.use();
		shade.setMat4fv("model", 1, GL_FALSE, model);
		nanosuit.Draw(shade);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	ratio = (float)width / height;
	glViewport(0, 0, width, height);

}

void processInput(GLFWwindow *window) {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.ProcessKeyboard(RIGHT, deltaTime);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	cam.ProcessMouseMovement(xoffset, yoffset, true, 0.1);
}
