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
Camera cam(glm::vec3(0.0f, 0.0f, 10.0f));

<<<<<<< HEAD
//Light Position
static float lightPos_x = 0.0f;
static float lightPos_y = 0.0f;
=======
>>>>>>> 6b431755e6e41d299c10600d0e9a32744c5d911b

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

	Shader PBRShader("GLSL/Vertex.vs","GLSL/PBR/Fragment.fs");
	Shader light("GLSL/LightShade/Vertex.vs", "GLSL/LightShade/Fragment.fs");
	Model sphere("Model/Sphere/LightSphere.obj");

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		processInput(window);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		glm::mat4 projection;
		projection = glm::perspective(glm::radians(35.0f), ratio, 0.1f, 1000.0f);

		//Setting Lights 
		//Setting Lights 
		light.use();
		light.setMat4fv("view", 1, GL_FALSE, cam.getView());
		light.setMat4fv("projection", 1, GL_FALSE, projection);
		glm::mat4 light_model(1.0f);
		//T*R*S order
		//T*R*S order
<<<<<<< HEAD
		light_model = glm::translate(light_model, glm::vec3(lightPos_x,lightPos_y,4.0f));
=======
		light_model = glm::translate(light_model, glm::vec3(0.5f,0.0f,4.0f));
>>>>>>> 6b431755e6e41d299c10600d0e9a32744c5d911b
		light_model = glm::scale(light_model, glm::vec3(0.05f));
		light.setMat4fv("model", 1, GL_FALSE, light_model);
		light.setVec3("color", glm::vec3(1.0f));
		sphere.Draw(light);


		//Setting Models
		//Setting Models
		PBRShader.use();
		PBRShader.setMat4fv("view", 1, GL_FALSE, cam.getView());
		PBRShader.setMat4fv("projection", 1, GL_FALSE, projection);
		PBRShader.setMat4fv("model", 1, GL_FALSE, glm::mat4(1.0f));
		PBRShader.setVec3("viewPos", cam.getPos());
		PBRShader.setVec3("albedo", glm::vec3(1.0f,0.0f,0.0f));
<<<<<<< HEAD
		PBRShader.setFloat("roughness", 0.4f);
		PBRShader.setFloat("metalness", 0.0f);
		PBRShader.setVec3("lights[0].pos", glm::vec3(lightPos_x, lightPos_y, 2.0f));
=======
		PBRShader.setFloat("roughness", 0.0f);
		PBRShader.setFloat("metalness", 0.0f);
		PBRShader.setVec3("lights[0].pos", glm::vec3(0.5f, 0.0f, 4.0f));
>>>>>>> 6b431755e6e41d299c10600d0e9a32744c5d911b
		PBRShader.setVec3("lights[0].diffuse", glm::vec3(20.0f));
		sphere.Draw(PBRShader);


	


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
<<<<<<< HEAD
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		lightPos_x +=  0.4*deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		lightPos_x -= 0.4*deltaTime;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		lightPos_y += 0.4*deltaTime;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		lightPos_y -= 0.4*deltaTime;
=======

>>>>>>> 6b431755e6e41d299c10600d0e9a32744c5d911b
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
