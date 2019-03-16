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
float xDir = 0.0f;

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

	glViewport(0, 0, 1920, 1080);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);


	float postQuad[]{
		//Left Down Triangle.
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,

	//Up Right Triangle.
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 1.0f
	};


	unsigned int quadVAO, quadVBO;

	//Bind Quad Vertices Data
	glGenBuffers(1, &quadVBO);
	glGenVertexArrays(1, &quadVAO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(postQuad), &postQuad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
	glBindVertexArray(0);

	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	//generate texture
	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//attach to current bound framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//using RenderBufferObject to make sure Depth testing happen
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Create Framebuffer for Shadow Mapping
	//Create Framebuffer for Shadow Mapping
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	const unsigned int SHADOW_WIDTH = 1024*2, SHADOW_HEIGHT = 1024*2;
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);


	//Loading models
	Model ground("Model/Shadow/ground.obj");
	Model cube("Model/Shadow/Cube.obj");
	Model monkey("Model/Shadow/Monkey.obj");

	//Loading shader
	Shader groundShader("GLSL/Model/Vertex.vs","GLSL/Model/Fragment.fs");
	Shader depthShader("GLSL/ShadowMapping/Vertex.vs", "GLSL/ShadowMapping/Fragment.fs");
	Shader postShader("GLSL/PostProcessing/Vertex.vs", "GLSL/PostProcessing/Fragment.fs");
	Shader postShaderDepthMap("GLSL/PostProcessing/Vertex.vs", "GLSL/PostProcessing/FragmentDepthMap.fs");


	//Setting shader
	groundShader.use();
	groundShader.setVec3("light.ambient", glm::vec3(0.05f));
	groundShader.setVec3("light.diffuse", glm::vec3(0.5f));
	groundShader.setVec3("light.specular", glm::vec3(2.0f));



	//Initial Setting
	//Initial Setting
	glEnable(GL_DEPTH_TEST);
	glm::vec3 lightPos(1.0f, 6.0f, 2.0f);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		processInput(window);


		//Render Shadow first (From light Perspective)
		//Render Shadow first (From light Perspective)

		glCullFace(GL_FRONT);//Prevent Peter Panning
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float near_plane = 0.1f, far_plane = 10.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4  lightView = glm::lookAt(lightPos, glm::vec3(xDir, -1.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		depthShader.use();
		depthShader.setMat4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glm::mat4 model(1.0f);
		depthShader.setMat4fv("model", 1, GL_FALSE, model);
		ground.Draw(depthShader);
		glm::mat4 cubemodel;
		cubemodel = glm::translate(model, glm::vec3(0.0f, 2.0f, -1.0f));
		depthShader.setMat4fv("model", 1, GL_FALSE, cubemodel);
		cube.Draw(depthShader);
		glm::mat4 monkeymodel;
		monkeymodel = glm::translate(model, glm::vec3(2.0f, 1.0f, 0.0f));
		depthShader.setMat4fv("model", 1, GL_FALSE, monkeymodel);
		monkey.Draw(depthShader);
		
		//Draw objects to frame buffer(from camera view)
		//Draw objects to frame buffer(from camera view)
		glViewport(0, 0, 1920, 1080);
		glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(35.0f), ratio, 0.1f, 1000.0f);
		groundShader.use();
		groundShader.setVec3("light.direction", glm::vec3(xDir, -1.0f, 0.0f));
		groundShader.setMat4fv("view", 1, GL_FALSE, cam.getView());
		groundShader.setMat4fv("projection", 1, GL_FALSE, projection);
		groundShader.setMat4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		groundShader.setInt("shadowMap", 3);

		groundShader.setMat4fv("model",1,GL_FALSE, model);
		ground.Draw(groundShader);
		cubemodel = glm::translate(model,glm::vec3(0.0f, 2.0f, -1.0f));
		groundShader.setMat4fv("model", 1, GL_FALSE, cubemodel);
		cube.Draw(groundShader);
		monkeymodel = glm::translate(model, glm::vec3(2.0f, 1.0f, 0.0f));
		groundShader.setMat4fv("model", 1, GL_FALSE, monkeymodel);
		monkey.Draw(groundShader);
		glCullFace(GL_BACK); //Prevent Peter Panning



		//Render to quad
		//Render to quad
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.5f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glDisable(GL_DEPTH_TEST); //CAUTION: Will not work for depth mapping
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		postShader.use();
		postShader.setFloat("near_plane", near_plane);
		postShader.setFloat("far_plane", far_plane);
		postShader.setInt("DepthMap", 0);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);    
		glBindVertexArray(0);

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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		xDir += 10*deltaTime;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		xDir -= 10*deltaTime;
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
