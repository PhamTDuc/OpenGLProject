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


int main(){
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


	float points[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*5,(void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float)*2));
	glBindVertexArray(0);


	Shader 	shader("GLSL/GeometryShader/Vertex.vs", "GLSL/GeometryShader/Fragment.fs", "GLSL/GeometryShader/Geometry.gs");
	Shader nanosuitShader("GLSL/Model/Vertex.vs", "GLSL/Model/ReflectiveFragment.fs");
	Shader  normalShader("GLSL/GeometryShader/VisualizeVector/Vertex.vs", "GLSL/GeometryShader/VisualizeVector/Fragment.fs", "GLSL/GeometryShader/VisualizeVector/Geometry.gs");
	Shader explode("GLSL/Model/Vertex.vs", "GLSL/Model/Fragment.fs", "GLSL/GeometryShader/Explode/Geometry.gs");
	

	//Load shader
	Model nanosuit("Model/Nanosuit/nanosuit.obj");

	//Load texture
	std::vector<std::string> faces
	{
			"Texture/skybox/right.jpg",
			"Texture/skybox/left.jpg",
			"Texture/skybox/top.jpg",
			"Texture/skybox/bottom.jpg",
			"Texture/skybox/front.jpg",
			"Texture/skybox/back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		processInput(window);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 4);

		//Draw Nanosuit
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(35.0f), ratio, 0.1f, 200.0f);
		glm::mat4 nanosuitMat4(1.0f);
		nanosuitMat4 = glm::scale(nanosuitMat4, glm::vec3(0.5f));

		/*nanosuitShader.use();
		nanosuitShader.setMat4fv("model", 1, GL_FALSE, nanosuitMat4);
		nanosuitShader.setMat4fv("view", 1, GL_FALSE, cam.getView());
		nanosuitShader.setMat4fv("projection", 1, GL_FALSE, projection);
		nanosuitShader.setVec3("camPos", cam.getPos());
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		nanosuitShader.setInt("skybox", 3);
		nanosuit.Draw(nanosuitShader);*/

		//Draw Nanosuit Normal
		explode.use();
		explode.setMat4fv("model", 1, GL_FALSE, nanosuitMat4);
		explode.setMat4fv("view", 1, GL_FALSE, cam.getView());
		explode.setMat4fv("projection", 1, GL_FALSE, projection);
		explode.setVec3("camPos", cam.getPos());
		explode.setFloat("time", (float)glfwGetTime());
		nanosuit.Draw(explode);



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
