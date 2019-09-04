#include "main.h"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

POINT CurPos;

Camera Cam(radius);

int main()
{
	
	///////////////////////////////////Settings/////////////////////////////////////////////////

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Animation", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window, check the opengl version!" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	////////////////////////////////////////Initialization///////////////////////////////////////
	unsigned int sphereShaderProgram = InitSphereShader();
	unsigned int particleShaderProgram = InitParticleShader();
	WireSphere sphere(radius, 20, 20);
	sphere.Init();
	particles Particles(num_particles, radius, inter);
	Particles.Init();
	GetCursorPos(&CurPos);
	////////////////////////////////////////////OPENGL////////////////////////////////////////////
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	

	/////////////////////////////////Optimization////////////////////////////////////
	glfwSwapInterval(1);
	/////////////////////////////////////////////////////////////////////////////////
	

	
	/////////////////////////////////////tranformation////////////////////////////////////////
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
	while (!glfwWindowShouldClose(window)) {


		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		glUniformMatrix4fv(glGetUniformLocation(particleShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(Cam.GetViewMatrix()));

		glUniformMatrix4fv(glGetUniformLocation(particleShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUseProgram(particleShaderProgram);
		Particles.Draw();

		glUniformMatrix4fv(glGetUniformLocation(sphereShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(Cam.GetViewMatrix()));

		glUniformMatrix4fv(glGetUniformLocation(sphereShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUseProgram(sphereShaderProgram);
		sphere.Draw();

		glfwPollEvents();
		
		glfwSwapBuffers(window);
		
	}
	glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
	Cam.ProcessMouseMovement(xpos - CurPos.x, ypos - CurPos.y);
	CurPos.x = xpos;
	CurPos.y = ypos;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		Cam.ArcRotation = !Cam.ArcRotation;
		Cam.Front = glm::vec3(0.0f, 0.0f, -1.0f);
		Cam.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		Sleep(100);
	}
	if (!Cam.ArcRotation) {
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			Cam.ProcessKeyboard(FORWARD);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			Cam.ProcessKeyboard(BACKWARD);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			Cam.ProcessKeyboard(LEFT);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			Cam.ProcessKeyboard(RIGHT);
	}
	
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (Cam.ArcRotation) {
		Cam.Distance += yoffset * 10;
		Cam.UpdateRotation();
	}
}