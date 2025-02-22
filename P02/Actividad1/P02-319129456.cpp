//ARCHIVO SEMESTRE 2024-1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <glew.h>
#include <glfw3.h>

const int WIDTH = 800, HEIGHT = 800;

// VAOs y VBOs para cada letra
GLuint VAO_A, VBO_A;
GLuint VAO_L, VBO_L;
GLuint VAO_R, VBO_R;

GLuint shader;

// Vertex Shader
static const char* vShader = "						\n\
#version 330										\n\
layout (location = 0) in vec3 pos;					\n\
void main()											\n\
{													\n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);	\n\
}";

// Fragment Shader modificado para usar un uniform "ourColor"
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
uniform vec4 ourColor;								\n\
void main()											\n\
{													\n\
    color = ourColor;								\n\
}";

// Función para agregar un shader al programa
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    GLint codeLength[1];
    codeLength[0] = (GLint)strlen(shaderCode);
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error al compilar el shader %d: %s\n", shaderType, eLog);
        return;
    }
    glAttachShader(theProgram, theShader);
}

// Compilación y linkeo de los shaders
void CompileShaders() {
    shader = glCreateProgram();
    if (!shader)
    {
        printf("Error creando el shader\n");
        return;
    }
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error al linkear el shader: %s\n", eLog);
        return;
    }
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error al validar el shader: %s\n", eLog);
        return;
    }
}

// La letra A se compone de tres partes: pierna izquierda, pierna derecha y línea media.
// Cada parte se compone de 2 triángulos (6 vértices).

//GLfloat verticesA[] = {
    // Pierna izquierda
//    -0.9f, -0.5f, 0.0f,
//    -0.8f,  0.8f, 0.0f,
//    -0.8f, -0.5f, 0.0f,
    // Pierna derecha
 //   -0.8f,  0.8f, 0.0f,
 //   -0.7f, -0.5f, 0.0f,
 //   -0.8f, -0.5f, 0.0f,
   // Línea media (parte 1)
 //   -0.85f, 0.1f, 0.0f,
  //  -0.75f, 0.1f, 0.0f,
  //  -0.75f, 0.0f, 0.0f,
    // Línea media (parte 2)
  //  -0.85f, 0.1f, 0.0f,
  //  -0.75f, 0.0f, 0.0f,
  //  -0.85f, 0.0f, 0.0f
//};


void CrearLetraA()
{
    GLfloat verticesA[] = {
        // Pierna izquierda
        // Triángulo 1:
        -0.9f, -0.5f, 0.0f,
        -0.82f, -0.5f, 0.0f,
        -0.80f,  0.8f, 0.0f,
        // Triángulo 2:
        -0.9f, -0.5f, 0.0f,
        -0.80f,  0.8f, 0.0f,
        -0.87f,  0.8f, 0.0f,

        // Pierna derecha
        // Triángulo 1:
        -0.78f, -0.5f, 0.0f,
        -0.7f, -0.5f, 0.0f,
        -0.73f,  0.8f, 0.0f,
        // Triángulo 2:
        -0.78f, -0.5f, 0.0f,
        -0.73f,  0.8f, 0.0f,
        -0.80f,  0.8f, 0.0f,

        // Línea media (rectángulo)
        // Triángulo 1:
        -0.85f, 0.2f, 0.0f,
        -0.75f, 0.2f, 0.0f,
        -0.75f, 0.3f, 0.0f,
        // Triángulo 2:
        -0.85f, 0.2f, 0.0f,
        -0.75f, 0.3f, 0.0f,
        -0.85f, 0.3f, 0.0f
    };

    glGenVertexArrays(1, &VAO_A);
    glBindVertexArray(VAO_A);

    glGenBuffers(1, &VBO_A);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_A);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesA), verticesA, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Creación de la letra L 
void CrearLetraL()
{
    GLfloat verticesL[] = {
        // Barra vertical
        -0.2f, -0.5f, 0.0f,
        -0.2f,  0.8f, 0.0f,
        -0.1f,  0.8f, 0.0f,

        -0.2f, -0.5f, 0.0f,
        -0.1f,  0.8f, 0.0f,
        -0.1f, -0.5f, 0.0f,

        // Barra horizontal
        -0.2f, -0.5f, 0.0f,
         0.2f, -0.5f, 0.0f,
         0.2f, -0.4f, 0.0f,

        -0.2f, -0.5f, 0.0f,
         0.2f, -0.4f, 0.0f,
        -0.2f, -0.4f, 0.0f
    };

    glGenVertexArrays(1, &VAO_L);
    glBindVertexArray(VAO_L);

    glGenBuffers(1, &VBO_L);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_L);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesL), verticesL, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Creación de la letra R
void CrearLetraR()
{
    GLfloat verticesR[] = {
        // Barra vertical
         0.5f, -0.5f, 0.0f,
         0.5f,  0.8f, 0.0f,
         0.6f,  0.8f, 0.0f,

         0.5f, -0.5f, 0.0f,
         0.6f,  0.8f, 0.0f,
         0.6f, -0.5f, 0.0f,

         // Parte superior de la R
          0.6f, 0.3f, 0.0f,
          0.6f, 0.8f, 0.0f,
          0.9f, 0.8f, 0.0f,

          0.6f, 0.3f, 0.0f,
          0.9f, 0.8f, 0.0f,
          0.9f, 0.3f, 0.0f,

          // Parte diagonal de la R
           0.6f, 0.3f, 0.0f,
           0.65f, 0.3f, 0.0f,
           0.9f, -0.5f, 0.0f,

           0.6f, 0.3f, 0.0f,
           0.9f, -0.5f, 0.0f,
           0.85f, -0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO_R);
    glBindVertexArray(VAO_R);

    glGenBuffers(1, &VBO_R);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_R);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesR), verticesR, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// MAIN
int main()
{
    if (!glfwInit())
    {
        printf("Falló la inicialización de GLFW\n");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);
    if (!mainWindow)
    {
        printf("Fallo en crearse la ventana con GLFW\n");
        glfwTerminate();
        return 1;
    }

    int BufferWidth, BufferHeight;
    glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        printf("Falló la inicialización de GLEW\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, BufferWidth, BufferHeight);

    // números aleatorios (para el color de fondo)
    srand((unsigned int)time(NULL));

    CrearLetraA();
    CrearLetraL();
    CrearLetraR();
    CompileShaders();

    // Obtener la ubicación del uniform "ourColor"
    GLint uniColor = glGetUniformLocation(shader, "ourColor");

    double lastTime = glfwGetTime();
    float bgR = 0.0f, bgG = 0.0f, bgB = 0.0f;

    // Loop principal
    while (!glfwWindowShouldClose(mainWindow))
    {
        glfwPollEvents();

        // Actualizar el color de fondo cada 2 segundos (RGB aleatorio)
        double currentTime = glfwGetTime();
        if (currentTime - lastTime >= 2.0)
        {
            bgR = (float)rand() / (float)RAND_MAX;
            bgG = (float)rand() / (float)RAND_MAX;
            bgB = (float)rand() / (float)RAND_MAX;
            lastTime = currentTime;
        }

        glClearColor(bgR, bgG, bgB, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Usar el shader
        glUseProgram(shader);

        // --- DIBUJO DE LA LETRA A ---
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniform4f(uniColor, 0.0f, 0.0f, 1.0f, 1.0f);// Azul
        glBindVertexArray(VAO_A);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        glBindVertexArray(0);

        // contorno negro
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);
        glUniform4f(uniColor, 0.0f, 0.0f, 0.0f, 1.0f);
        glBindVertexArray(VAO_A);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // --- DIBUJO DE LA LETRA L ---
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniform4f(uniColor, 0.0f, 1.0f, 0.0f, 1.0f); // Verde
        glBindVertexArray(VAO_L);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        glBindVertexArray(0);
        // Contorno negro
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);
        glUniform4f(uniColor, 0.0f, 0.0f, 0.0f, 1.0f);
        glBindVertexArray(VAO_L);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // --- DIBUJO DE LA LETRA R ---
        // Relleno rojo
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniform4f(uniColor, 1.0f, 0.0f, 0.0f, 1.0f);
        glBindVertexArray(VAO_R);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        glBindVertexArray(0);
        // Contorno negro
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);
        glUniform4f(uniColor, 0.0f, 0.0f, 0.0f, 1.0f);
        glBindVertexArray(VAO_R);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
