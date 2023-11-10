#include <iostream>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


constexpr int SRC_WIDTH = 800;
constexpr int SRC_HEIGHT = 600;


void processInput(GLFWwindow* window)
{
	if (::glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		::glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	::glViewport(0, 0, width, height);
}

int main()
{
	::glfwInit();
	// ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = ::glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Hello OpenGL", nullptr, nullptr);
	
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		return EXIT_FAILURE;
	}

	::glfwMakeContextCurrent(window);
	::glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!::gladLoadGLLoader((GLADloadproc)::glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return EXIT_FAILURE;
	}

	int success{ 0 };

	const char* vertexShaderSource = R"(
		#version 450
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aColor;
		layout (location = 0) out vec3 vertexColor;
		void main()
		{
			gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
			vertexColor = aColor;
		}
	)";

	const char* fragmentShaderSource = R"(
		#version 450
		layout (location = 0) in vec3 vertexColor;
		layout (location = 0) out vec4 fragColor;
		uniform vec4 ourColor;
		void main()
		{
			fragColor = vec4(vertexColor, 1.0f);
		}
	)";

	unsigned int vertexShader{ 0 };
	vertexShader = ::glCreateShader(GL_VERTEX_SHADER);
	::glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	::glCompileShader(vertexShader);
	::glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		::glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::VertexShader: " << infoLog << std::endl;
	}

	unsigned int fragmentShader{ 0 };
	fragmentShader = ::glCreateShader(GL_FRAGMENT_SHADER);
	::glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	::glCompileShader(fragmentShader);
	::glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		::glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::FragmentShader: " << infoLog << std::endl;
	}

	unsigned int shaderProgram;
	shaderProgram = ::glCreateProgram();
	::glAttachShader(shaderProgram, vertexShader);
	::glAttachShader(shaderProgram, fragmentShader);
	::glLinkProgram(shaderProgram);
	::glGetShaderiv(fragmentShader, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		::glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::ShaderLinked: " << infoLog << std::endl;
	}

	::glDeleteShader(vertexShader);
	::glDeleteShader(fragmentShader);

	float vertices[] =
	{
		 0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,  // top right
		 0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,	// bottom right
		-0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f,	// bottom left
		-0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f,	// top left 
	};

	unsigned int indices[] =
	{
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	unsigned int vao{ 0 };
	::glGenVertexArrays(1, &vao);
	::glBindVertexArray(vao);

	unsigned int vbo{ 0 };
	::glGenBuffers(1, &vbo);
	::glBindBuffer(GL_ARRAY_BUFFER, vbo);
	::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	::glEnableVertexAttribArray(0);
	::glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	::glEnableVertexAttribArray(1);

	unsigned int ebo{ 0 };
	::glGenBuffers(1, &ebo);
	::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Unbind
	::glBindVertexArray(0);

	while (!::glfwWindowShouldClose(window))
	{
		processInput(window);

		::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		::glClear(GL_COLOR_BUFFER_BIT);

		::glUseProgram(shaderProgram);

		float timeValue = ::glfwGetTime();
		float greenValue = sin(timeValue) * 0.5f + 0.5f;
		int vertexColorLocation = ::glGetUniformLocation(shaderProgram, "ourColor");
		::glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		::glBindVertexArray(vao);
		::glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		::glBindVertexArray(0);

		::glfwSwapBuffers(window);
		::glfwPollEvents();
	}

	::glDeleteBuffers(1, &vbo);
	::glDeleteBuffers(1, &ebo);
	::glDeleteVertexArrays(1, &vao);
	::glDeleteProgram(shaderProgram);

	::glfwDestroyWindow(window);
	::glfwTerminate();

	return EXIT_SUCCESS;
}
