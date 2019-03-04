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



	float data[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};


	glm::vec3 cubePositions[] = {
	  glm::vec3(0.0f, 0.0f,  0.0f),
	  glm::vec3(2.0f, 0.0f, -1.0f),
	  glm::vec3(-1.5f, 0.0f, -2.5f),
	  glm::vec3(-3.8f, 0.0f, -1.3f),
	  glm::vec3(2.4f, 0.0f, -1.5f),
	  glm::vec3(-1.7f, 0.0f, -0.5f),
	  glm::vec3(1.3f, 0.0f, -0.5f),
	  glm::vec3(1.5f, 0.0f, -1.5f),
	  glm::vec3(1.5f, 0.0f, -1.5f),
	  glm::vec3(-1.3f, 0.0f, -1.5f)
	};
	//Instancing Array
	glm::vec2 translations[1000];
	int index = 0;
	for (int y = -10; y < 10; y += 2)
	{
		for (int x = -10; x < 10; x += 2)
		{
			glm::vec2 translation;
			translation.x = (float)x / 1.0f;
			translation.y = (float)y / 1.0f;
			translations[index++] = translation;
		}
	}


	
	unsigned int VAO[2], VBO[2];
	glGenBuffers(2, VBO);
	glGenVertexArrays(2, VAO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(translations), &translations, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	glVertexAttribDivisor(1, 1);
	
	

	//Instance Matrix
	constexpr unsigned int amount = 20;
	glm::mat4 *modelMatrices=new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed	
	float radius = 15.0;
	float offset = 2.5f;
	for (GLsizei i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = (rand() % 50) / 100.0f + 0.25;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}

	// vertex Buffer Object
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), modelMatrices, GL_STATIC_DRAW);

	//Loading Model
	Model steriod=Model("Model/Asteriod/Asteriod3.obj");
	Model earth = Model("Model/Asteriod/Planet.obj");
	//Loading Texture
	unsigned int earthTex = loadTexture("Model/Asteriod/Planet.jpg");
	unsigned int AsteriodTex = loadTexture("Model/Asteriod/Asteriod3.png");


	// vertex Attributes
	glBindVertexArray(steriod.meshes[0].VAO);
	
	//Important
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//Important

	GLsizei vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);




	Shader 	primitiveInstance("GLSL/Instancing/Vertex.vs", "GLSL/Instancing/Fragment.fs");
	Shader newInstance("GLSL/Instancing/VertexInstance.vs", "GLSL/Instancing/Fragment.fs");
	Shader steriodShader("GLSL/Instancing/SteriodInstance.vs", "GLSL/Model/Fragment.fs");
	Shader earthShader("GLSL/Model/Vertex.vs", "GLSL/Model/Fragment.fs");

	/*for (unsigned int i = 0; i < 100; i++)
	{
		std::stringstream ss;
		std::string index;
		ss << i;
		index = ss.str();
		primitiveInstance.setVec2(("offsets[" + index + "]").c_str(), translations[i]);
	}*/


	
	


	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		processInput(window);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(35.0f), ratio, 0.1f, 1000.0f);
		newInstance.use();
		newInstance.setMat4fv("projection", 1, GL_FALSE, projection);
		newInstance.setMat4fv("view", 1, GL_FALSE, cam.getView());
		
		earthShader.use();
		earthShader.setMat4fv("projection", 1, GL_FALSE, projection);
		earthShader.setMat4fv("view", 1, GL_FALSE, cam.getView());
		earthShader.setMat4fv("model", 1, GL_FALSE, glm::mat4(1.0f));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, earthTex);

		//glBindVertexArray(VAO[0]);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		earth.Draw(earthShader);

		

		steriodShader.use();
		steriodShader.setMat4fv("projection", 1, GL_FALSE, projection);
		steriodShader.setMat4fv("view", 1, GL_FALSE, cam.getView());

		steriodShader.setInt("diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, AsteriodTex);
	
		glBindVertexArray(steriod.meshes[0].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, steriod.meshes[0].indices.size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	delete modelMatrices;
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
