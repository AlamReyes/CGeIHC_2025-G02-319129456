// López Reyes Alam 319129456
#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
// Variables por cada figura
GLuint squareVAO, squareVBO;
GLuint diamondVAO, diamondVBO;
GLuint shaderProgram;

//Vertex Shader
//No lo cambié
static const char* vShaderSource = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";
//recibir Vcolor y dar de salida color
static const char* fShaderSource = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,0.5f,0.7f,1.0f);	 			\n\
}";



// Función para crear el cuadrado
// Se ubica en la parte izquierda
void CrearCuadrado() {
	// Se define 2 triángulos para formar el cuadrado
	GLfloat squareVertices[] = {
		// Primer triángulo
		-0.8f,  0.8f, 0.0f,  // esquina superior izquierda
		-0.2f,  0.8f, 0.0f,  // esquina superior derecha
		-0.2f,  0.2f, 0.0f,  // esquina inferior derecha
		// Segundo triángulo
		-0.8f,  0.8f, 0.0f,  // esquina superior izquierda
		-0.2f,  0.2f, 0.0f,  // esquina inferior derecha
		-0.8f,  0.2f, 0.0f   // esquina inferior izquierda
	};

	glGenVertexArrays(1, &squareVAO);
	glBindVertexArray(squareVAO);

	glGenBuffers(1, &squareVBO);
	glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Función para crear el rombo (diamond)
// Se ubica en la parte derecha
void CrearRombo() {
	// Se define un rombo usando 2 triángulos.
	// Los 4 vértices del rombo son:
	// Top:    (0.5,  0.8)
	// Right:  (0.8,  0.5)
	// Bottom: (0.5,  0.2)
	// Left:   (0.2,  0.5)
	GLfloat diamondVertices[] = {
		// Primer triángulo (Top, Right, Bottom)
		 0.5f,  0.8f, 0.0f,  // top
		 0.8f,  0.5f, 0.0f,  // right
		 0.5f,  0.2f, 0.0f,  // bottom
		 // Segundo triángulo (Top, Bottom, Left)
		  0.5f,  0.8f, 0.0f,  // top
		  0.5f,  0.2f, 0.0f,  // bottom
		  0.2f,  0.5f, 0.0f   // left
	};

	glGenVertexArrays(1, &diamondVAO);
	glBindVertexArray(diamondVAO);

	glGenBuffers(1, &diamondVBO);
	glBindBuffer(GL_ARRAY_BUFFER, diamondVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(diamondVertices), diamondVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



// Función para compilar y linkear shaders
void CompileShaders() {
	GLuint vertexShader, fragmentShader;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR: Vertex Shader no compiló\n%s\n", infoLog);
	}

	// Fragment Shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR: Fragment Shader no compiló\n%s\n", infoLog);
	}

	// Linkear shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR: Programa no linkeado\n%s\n", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
int main()
{
	// Inicialización de GLFW
	if (!glfwInit()) {
		printf("Falló al inicializar GLFW\n");
		glfwTerminate();
		return -1;
	}
	// Configurar GLFW para OpenGL 4.3 Core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Crear ventana
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Ventana - Cuadrado y Rombo", NULL, NULL);
	if (!mainWindow) {
		printf("Falló en crearse la ventana con GLFW\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mainWindow);

	// Inicialización de GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		printf("Falló la inicialización de GLEW\n");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return -1;
	}

	// Configurar Viewport
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Compilar y linkear shaders
	CompileShaders();

	// Crear las figuras: cuadrado y rombo
	CrearCuadrado();
	CrearRombo();

	// Loop principal
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Procesar eventos
		glfwPollEvents();

		// --- CAMBIO DE COLOR DE FONDO DE FORMA CÍCLICA ---
		// Usé glfwGetTime para obtener el tiempo en segundos
		float time = (float)glfwGetTime();
		// Cada 2 segundos se cambia de color
		int fase = ((int)time / 2) % 3;
		if (fase == 0) {
			// Rojo
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else if (fase == 1) {
			// Verde
			glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		}
		else if (fase == 2) {
			// Azul
			glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		}
		glClear(GL_COLOR_BUFFER_BIT);

		// --- DIBUJAR LAS FIGURAS ---
		glUseProgram(shaderProgram);

		// Dibujar el cuadrado
		glBindVertexArray(squareVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// Dibujar el rombo
		glBindVertexArray(diamondVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	// Limpieza: liberar recursos (opcional pero recomendado)
	glDeleteVertexArrays(1, &squareVAO);
	glDeleteBuffers(1, &squareVBO);
	glDeleteVertexArrays(1, &diamondVAO);
	glDeleteBuffers(1, &diamondVBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(mainWindow);
	glfwTerminate();
	return 0;
}