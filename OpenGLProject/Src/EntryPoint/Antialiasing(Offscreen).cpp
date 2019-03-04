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
Camera cam(glm::vec3(-2.0f, 1.0f, 10.0f));


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_SAMPLES, 16);

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
	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_DEPTH_TEST);


	//Screen Quad
	float quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};
	 // setup screen VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//Loading Model
	//Loading Model
	Model nanosuit("Model/Nanosuit/nanosuit.obj");

	//Create and Setup Shader
	//Create and Setup Shader
	Shader nanosuitShader("GLSL/Model/Vertex.vs", "GLSL/Model/Fragment.fs");
	glm::mat4 projection = glm::perspective(glm::radians(35.0f), ratio, 0.1f, 1000.0f);
	glm::mat4 model(1.0f);
	nanosuitShader.use();
	nanosuitShader.setMat4fv("projection", 1, GL_FALSE, projection);
	nanosuitShader.setMat4fv("model", 1, GL_FALSE, model);

	Shader screenShader("GLSL/PostProcessing/Vertex.vs", "GLSL/PostProcessing/Fragment.fs");
	screenShader.use();
	screenShader.setInt("ScreenTexture", 0);

	//Setting Framebuffer
	//Setting Framebuffer
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	/*unsigned int multisampleTex;
	glGenTextures(1, &multisampleTex);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisampleTex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, 800, 600, GL_TRUE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multisampleTex, 0);*/

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	unsigned int crbo;
	glGenRenderbuffers(1, &crbo);
	glBindRenderbuffer(GL_RENDERBUFFER, crbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, crbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! 1" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//Setting Screen Framebuffer
	unsigned int screenFBO;
	glGenFramebuffers(1, &screenFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);

	unsigned int screenTex;
	glGenTextures(1, &screenTex);
	glBindTexture(GL_TEXTURE_2D, screenTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTex, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! 2" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		processInput(window);

		//Drawing objects as normal
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		nanosuitShader.use();
		nanosuitShader.setMat4fv("view", 1, GL_FALSE, cam.getView());
		nanosuit.Draw(nanosuitShader);

		//Transfer framebuffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, screenFBO);
		glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		//Drawing using buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		screenShader.use();
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenTex); // use the now resolved color attachment as the quad's texture
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glfwSwapBuffers(window);
	}

	glDeleteBuffers(1, &quadVBO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteFramebuffers(1, &fbo);
	glDeleteFramebuffers(1, &screenFBO);
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
