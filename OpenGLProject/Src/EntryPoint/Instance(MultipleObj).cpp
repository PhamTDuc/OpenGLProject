#include <iostream>
#include <memory>
#include <array>
#include <random>
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
//THIS IS NOT AN EXTRACT WAY TO INSTANCE MULTIPLE OBJECTS
//IT USES LOOP TO DRAW OBJECTS
//THIS IS NOT AN EXTRACT WAY TO INSTANCE MULTIPLE OBJECTS
//IT USES LOOP TO DRAW OBJECTS
//THIS IS NOT AN EXTRACT WAY TO INSTANCE MULTIPLE OBJECTS
//IT USES LOOP TO DRAW OBJECTS

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
	glfwWindowHint(GLFW_SAMPLES, 16);

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



	//Instance Matrix
	constexpr unsigned int amount = 4000;
	glm::mat4 *modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed	
	float radius = 20.0;
	float offset = 5.5f;
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
		float scale = (rand() % 10) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}

	std::unique_ptr<Model> group = std::make_unique<Model>("Model/Asteriod/Group.obj");
	std::unique_ptr<Model> planet = std::make_unique<Model>("Model/Asteriod/Planet.obj");

	//Load Shader
	Shader shade("GLSL/Model/Vertex.vs","GLSL/Model/Fragment.fs");
	Shader instanceShade("GLSL/Instancing/VertexInstance.vs", "GLSL/Model/Fragment.fs");
	Shader skyboxShader("GLSL/Skybox/Vertex.vs", "GLSL/Skybox/Fragment.fs");




	//Initialize random number between 0 and 2
	std::default_random_engine generator(0);
	std::uniform_int_distribution<int> distribution(0, 2);
	std::array<unsigned int, amount> IndexObj;
	for (auto &index : IndexObj) {
		index = distribution(generator);
	}


	//SkyBox Texture
	std::vector<std::string> faces
	{
			"Texture/ame_nebula/purplenebula_rt.tga",
			"Texture/ame_nebula/purplenebula_lf.tga",
			"Texture/ame_nebula/purplenebula_up.tga",
			"Texture/ame_nebula/purplenebula_dn.tga",
			"Texture/ame_nebula/purplenebula_ft.tga",
			"Texture/ame_nebula/purplenebula_bk.tga",

		
	};

	unsigned int cubemapTexture = loadCubemap(faces);

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		processInput(window);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(35.0f), ratio, 0.1f, 1000.0f);
		shade.use();
		shade.setMat4fv("view",1,GL_FALSE,cam.getView());
		shade.setMat4fv("projection", 1, GL_FALSE, projection);
		shade.setMat4fv("model", 1, GL_FALSE, glm::mat4(1.0f));
		planet->Draw(shade);

		for (unsigned int i = 0; i < amount; i++) {
			shade.setMat4fv("model", 1, GL_FALSE, modelMatrices[i]);
			group->meshes[IndexObj[i]].Draw(shade);
		}
		
		////Draw skybox
		glDepthFunc(GL_LEQUAL);
		glm::mat4 skybox_view = glm::mat4(glm::mat3(cam.getView()));
		glDepthMask(GL_FALSE);
		skyboxShader.use();
		skyboxShader.setMat4fv("view", 1, GL_FALSE, skybox_view);
		skyboxShader.setMat4fv("projection", 1, GL_FALSE, projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		skyboxShader.setInt("skybox", 0);
		glBindVertexArray(skyboxVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	delete modelMatrices;

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
