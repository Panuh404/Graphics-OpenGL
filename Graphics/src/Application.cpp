#include <iostream>

#include <string>
#include <fstream>
#include <sstream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

enum shader_type {
	VERTEX_SHADER,
	FRAGMENT_SHADER
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void check_shader(unsigned int shader, bool program = false) {
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success and !program) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else if (!success and program) {
		glGetProgramInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	if (success and !program) {
		std::cout << "SHADER::COMPILATION_SUCCESSFUL" << std::endl;
	}
	else if (success and program) {
		std::cout << "PROGRAM::COMPILATION_SUCCESSFUL" << std::endl;
	}
}

unsigned int compile_shader(std::string filepath, shader_type type) {
	std::ifstream ShaderStream(filepath);
	std::string shaderSource;
	std::string Line;
	while (std::getline(ShaderStream, Line)) {
		shaderSource += "\n" + Line;
	}
	
	unsigned int shader;
	const char* src = shaderSource.c_str();

	if (type == VERTEX_SHADER) {
		shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);
		check_shader(shader);
		return shader;
		
	}
	else if (type == FRAGMENT_SHADER) {
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);
		check_shader(shader);
		return shader;
	}
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "Graphic Programming", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//glViewport(0, 0, 800, 600);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	unsigned int vertexShader = compile_shader("res/shaders/vertex_basic.shader", VERTEX_SHADER);
	unsigned int fragmentShader = compile_shader("res/shaders/fragment_basic.shader", FRAGMENT_SHADER);
	
	unsigned int shader; 
	shader = glCreateProgram();
	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);
	
	check_shader(shader, true);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	
	unsigned int VBO; // Vertex Buffer Object
	unsigned int VAO; // Vertex Array Object
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//Render Loop
	while (!glfwWindowShouldClose(window)) {
		//Input
		processInput(window);
		
		//Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		//Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader);
	
	glfwTerminate();
	return 0;
}