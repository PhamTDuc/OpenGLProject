#include <iostream>
#include <random>
#include <vector>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "../../LoadingTexture.h"
#include "../../Shader.h"
#include "../../Camera.h"
#include "../../Model.h"
float static deltaTime = 0.0f;	// Time between current frame and last frame
static float lastFrame = 0.0f; // Time of last frame
static float x_g = 0.0f;
static float y_g = 0.0f;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
bool firstMouse = true;
float lastX = 0.0f, lastY = 0.0f;

float ratio = (float)4 / 3;
Camera cam(glm::vec3(0.0f, 1.0f, 5.0f));


float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}


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
	//glfwSetCursorPosCallback(window, mouse_callback);



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



	//Create GBuffer object
	//Create GBuffer object
	unsigned int gBuffer;
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	unsigned int gPosition, gNormal, gColorSpec;

	// - position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// - normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// - color + specular color buffer
	glGenTextures(1, &gColorSpec);
	glBindTexture(GL_TEXTURE_2D, gColorSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);
	glBindTexture(GL_TEXTURE_2D, 0);


	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	//using RenderBufferObject to make sure Depth testing happen
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//Create Framebuffer to hold SSAO processing stage
	//Create Framebuffer to hold SSAO processing stage
	unsigned int ssaoFBO, ssaoBlurFBO;
	glGenFramebuffers(1, &ssaoFBO);  glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
	// SSAO color buffer
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 800, 600, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Framebuffer not complete!" << std::endl;
	// Blur Stage
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 800, 600, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Model scene("Model/Scene/scene.obj");
	Model lightSphere("Model/Sphere/LightSphere.obj");

	Shader ssaoGeometryShader("GLSL/Model/VertexSSAO.vs", "GLSL/Model/FragmentSSAO.fs");
	Shader ssaoChecker("GLSL/PostProcessing/Vertex.vs", "GLSL/PostProcessing/ssaoChecker.fs");
	Shader ssaoFinal("GLSL/PostProcessing/Vertex.vs", "GLSL/PostProcessing/FinalSSAO.fs");
	Shader light("GLSL/LightShade/Vertex.vs", "GLSL/LightShade/Fragment.fs");


	//Generate random number;
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> disx(-4.0f, 8.0f);
	std::uniform_real_distribution<float> disz(-5.0f, 5.0f);
	std::uniform_real_distribution<float> randomFloats(0.0, 1.0);

	float randomPos[4][2];
	float randomLightPos[4][2];
	for (int i = 0; i < 4; i++) {
		randomPos[i][0] = disx(gen);
		randomPos[i][1] = disz(gen);
		randomLightPos[i][0] = disz(gen);
		randomLightPos[i][1] = disx(gen);
	}


	//SSAO samples oriented Generator
	//SSAO samples oriented Generator
	glm::vec3 ssaoKernel[64];
	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(
			randomFloats(gen) * 2.0 - 1.0,
			randomFloats(gen) * 2.0 - 1.0,
			randomFloats(gen)
		);
		sample = glm::normalize(sample);
		sample *= randomFloats(gen);
		float scale = (float)i / 64.0f;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel[i] = sample;
	}

	glm::vec3 ssaoNoise[16];
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(
			randomFloats(gen) * 2.0 - 1.0,
			randomFloats(gen) * 2.0 - 1.0,
			0.0f);
		ssaoNoise[i] = noise;
	}

	unsigned int noiseTexture;
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	
	const char patterns[] = {
		 0, 32,  8, 40,  2, 34, 10, 42,
		48, 16, 56, 24, 50, 18, 58, 26,
		12, 44,  4, 36, 14, 46,  6, 38,
		60, 28, 52, 20, 62, 30, 54, 22,
		 3, 35, 11, 43,  1, 33,  9, 41,
		15, 47,  7, 39, 13, 45,  5, 37,
		63, 31, 55, 23, 61, 29, 53, 21,
	}; //Bayer matrix

	unsigned int ditherTexture;
	glGenTextures(1, &ditherTexture);
	glBindTexture(GL_TEXTURE_2D, ditherTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 8, 8, 0, GL_RED, GL_FLOAT, &patterns[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glm::mat4 projection = glm::perspective(glm::radians(35.0f), ratio, 0.1f, 1000.0f);

	//GUI parameters
	float radius=0.1f;
	float intensity = 0.01f;
	float val = 1.0f;
	glm::vec3 my_color;
	
	
	// GUI
	// GUI
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	//Load Fonts
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font1 = io.Fonts->AddFontDefault();
	ImFont* font2 = io.Fonts->AddFontFromFileTTF("Fonts/DancingScript-Regular.ttf",18.0f);
	if(!font2)
		std::cout << "Fail to load Font from File" << std::endl;


	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		processInput(window);
		
		//GUI
		//GUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_STENCIL_TEST);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		//First Step
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		ssaoGeometryShader.use();
		ssaoGeometryShader.setMat4fv("view", 1, GL_FALSE, cam.getView());
		ssaoGeometryShader.setMat4fv("projection", 1, GL_FALSE, projection);
		ssaoGeometryShader.setMat4fv("model", 1, GL_FALSE, glm::mat4(1.0f));
		scene.Draw(ssaoGeometryShader);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);






		//Second Step
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gColorSpec);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
	

		ssaoChecker.use();
		ssaoChecker.setInt("gPosition", 0);
		ssaoChecker.setInt("gNormal", 1);
		ssaoChecker.setInt("texNoise", 3);
		ssaoChecker.setFloat("radius", radius);
		ssaoChecker.setMat4fv("projection", 1, GL_FALSE, projection);
		for (unsigned int i = 0; i < 64; ++i)
			ssaoChecker.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Blur SSAO Stage and Deferred Shading
		//Blur SSAO Stage and Deferred Shading
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, ditherTexture);
		ssaoFinal.use();
		ssaoFinal.setInt("ssao", 4);
		ssaoFinal.setInt("gPosition", 0);
		ssaoFinal.setInt("gNormal", 1);
		ssaoFinal.setInt("dither", 5);
		ssaoFinal.setFloat("intensity", intensity);
		ssaoFinal.setFloat("val", val);
		ssaoFinal.setMat4fv("view", 1, GL_FALSE, cam.getView());
		for (int i = 0; i < 4; i++) {
			ssaoFinal.setVec3("lights[" + std::to_string(i) + "].pos", glm::vec3(x_g + randomLightPos[i][1], y_g + randomLightPos[i][0] / 10, 0.1f));
			ssaoFinal.setVec3("lights[" + std::to_string(i) + "].diffuse", glm::vec3(1.0f, 0.5f, 1.0f)*4.0f);

		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glStencilMask(0xff);
		//glStencilFunc(GL_ALWAYS, 0, 0xff);
		//glDisable(GL_STENCIL_TEST);






		//Combine with forward rendering
		//Combine with forward rendering
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
		glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		for (int i = 0; i < 4; i++) {
			glm::mat4 model_light(1.0f);
			model_light = glm::translate(model_light, glm::vec3(x_g + randomLightPos[i][1], y_g + randomLightPos[i][0] / 10, 0.1f));
			const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
			const float linear = 0.7f;
			const float quadratic = 1.8f;
			const float maxBrightness = 20.0f;
			float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness)));
			model_light = glm::scale(model_light, glm::vec3(radius / 800));
			light.use();
			light.setVec3("color", glm::vec3(0.0f, 0.5f, 1.0f));
			light.setMat4fv("view", 1, GL_FALSE, cam.getView());
			light.setMat4fv("projection", 1, GL_FALSE, projection);
			light.setMat4fv("model", 1, GL_FALSE, model_light);
			lightSphere.Draw(light);
		}

		//GUI
		//GUI
		static int currentListItemIndex = 0;
		static int currentItemIndex = 0;
		static bool selected[10];
		static const char* comboItems[]={"Apple", "Banana", "Orange", "Mango", "Tangerine", "Persimmon"};
		{
			bool my_tool_active;
			//Passing '&my_tool_active' displays a Close button on the upper - right corner of the window, the pointed value will be set to false when the button is pressed.
			ImGui::Begin("Controller", &my_tool_active, ImGuiWindowFlags_MenuBar);
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
					if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
					if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("Preferences", "Ctrl+Shift+U")) { /* Do stuff */ }
					if (ImGui::MenuItem("Copy", "Ctrl+C")) { /* Do stuff */ }
					if (ImGui::MenuItem("Paste", "Ctrl+V")) { /* Do stuff */ }
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
				
			}
			if (font2) 
			{
				ImGui::PushFont(font2);
				ImGui::Text("CHANGE PARAMETERS");
				ImGui::Text("CHANGE PARAMETERS");
				ImGui::PopFont();
			}
			// Edit a color (stored as ~4 floats)
			ImGui::ColorEdit3("Color", &my_color[0]);


			// Display contents in a scrolling region
			ImGui::TextColored(ImVec4{ my_color[0],my_color[1],my_color[2],1.0f }, "IMPORTANT STUFF");
			ImGui::BeginChild("Scrolling", ImVec2(0, 100), true);
			for (int n = 0; n < 5; n++)
				ImGui::Text("%04d: Some text", n);
			ImGui::EndChild();

			ImGui::BeginChild("test", ImVec2(0, 200), true);

			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::Selectable("Clear"))
				{
				}
				if (ImGui::Selectable("ChangeVal"))
				{
				}
				if (ImGui::Selectable("EditVal"))
				{
				}
				ImGui::EndPopup();
			}
			
			//Listbox and Combo
			ImGui::ListBox("Listbox", &currentListItemIndex, comboItems, IM_ARRAYSIZE(comboItems), 4);
			ImGui::Combo("Combobox", &currentItemIndex, comboItems, IM_ARRAYSIZE(comboItems));

			//MultiSelectable Combo
			if (ImGui::BeginCombo("MultiSelectable Combo", "")){
				std::string preview = "";
				for (int i = 0; i < 10; i++) {
					ImGui::Selectable(("Item " + std::to_string(i)).c_str(),&selected[i],ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
				}
				
				ImGui::EndCombo();
			}

			ImGui::EndChild();

			// Plot some values
			const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f, 2.3f, 1.0f, 4.5f, 10.0f, 6.0f, 10.5f};
			ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

			//Slider
			ImGui::SliderFloat("ssao radius:", &radius, 0.1f, 10.0f);
			ImGui::SliderFloat("intensity:", &intensity, 0.01f, 10.0f);
			ImGui::SliderFloat("Dither:", &val, 1.0f, 256.0f);
			if (!my_tool_active)
				ImGui::Text("Hello");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();


		}


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		y_g += 0.75f*deltaTime;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		y_g -= 0.75f*deltaTime;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		x_g += 0.75f*deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		x_g -= 0.75f*deltaTime;

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
	cam.ProcessMouseMovement(xoffset, yoffset, true, 0.1f);
}
