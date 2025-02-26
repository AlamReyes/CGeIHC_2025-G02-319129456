// práctica 3: Modelado Geométrico y Cámara Sintética. López Reyes Alam
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>

// glm
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/random.hpp>

// clases para dar orden y limpieza al código
#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
#include "Camera.h"

// teclas de ejemplo: E -> Rotar X, R -> Rotar Y, T -> Rotar Z

using std::vector;

// Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0f; // grados a radianes
const float PI = 3.14159265f;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader> shaderList;

// Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";

// Para la esfera (ventana trasera circular)
Sphere sp = Sphere(1.0, 20, 20); // radio = 1.0, slices y stacks = 20


void CrearCubo()
{
    unsigned int cubo_indices[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3
    };

    GLfloat cubo_vertices[] = {
        // front
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        // back
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };
    Mesh* cubo = new Mesh();
    cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
    meshList.push_back(cubo);
}

// Pirámide Triangular
void CrearPiramideTriangular()
{
    unsigned int indices_piramide_triangular[] = {
        0,1,2,
        1,3,2,
        3,0,2,
        1,0,3
    };
    GLfloat vertices_piramide_triangular[] = {
       -0.5f, -0.5f, 0.0f,   //0
        0.5f, -0.5f, 0.0f,   //1
        0.0f,  0.5f, -0.25f, //2
        0.0f, -0.5f, -0.5f,  //3
    };
    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
    meshList.push_back(obj1);
}

// Crear Cilindro
void CrearCilindro(int res, float R)
{
    int n, i;
    GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
    vector<GLfloat> vertices;
    vector<unsigned int> indices;

    // paredes del cilindro
    for (n = 0; n <= res; n++) {
        if (n != res) {
            x = R * cos(n * dt);
            z = R * sin(n * dt);
        }
        else {
            // volver al inicio del círculo
            x = R * cos(0);
            z = R * sin(0);
        }
        // cada vértice se repite dos veces (abajo y arriba)
        for (i = 0; i < 6; i++) {
            switch (i) {
            case 0: vertices.push_back(x); break;
            case 1: vertices.push_back(y); break;
            case 2: vertices.push_back(z); break;
            case 3: vertices.push_back(x); break;
            case 4: vertices.push_back(0.5f); break;
            case 5: vertices.push_back(z); break;
            }
        }
    }
    for (n = 0; n <= res; n++) {
        x = R * cos(n * dt);
        z = R * sin(n * dt);
        vertices.push_back(x);
        vertices.push_back(-0.5f);
        vertices.push_back(z);
    }
    for (n = 0; n <= res; n++) {
        x = R * cos(n * dt);
        z = R * sin(n * dt);
        vertices.push_back(x);
        vertices.push_back(0.5f);
        vertices.push_back(z);
    }

    for (i = 0; i < (int)vertices.size(); i++) {
        indices.push_back(i);
    }

    Mesh* cilindro = new Mesh();
    cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
    meshList.push_back(cilindro);
}

// Crear Cono
void CrearCono(int res, float R)
{
    int n, i;
    GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
    vector<GLfloat> vertices;
    vector<unsigned int> indices;

    // vértice superior del cono (punta)
    vertices.push_back(0.0f);
    vertices.push_back(0.5f);
    vertices.push_back(0.0f);

    // circunferencia base del cono
    for (n = 0; n <= res; n++) {
        x = R * cos(n * dt);
        z = R * sin(n * dt);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }
    // cerrar circunferencia
    vertices.push_back(R * cos(0));
    vertices.push_back(y);
    vertices.push_back(R * sin(0));

    for (i = 0; i < res + 2; i++)
        indices.push_back(i);

    Mesh* cono = new Mesh();
    cono->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
    meshList.push_back(cono);
}

// Pirámide Cuadrangular (Techo)
void CrearPiramideCuadrangular()
{
    vector<unsigned int> indices = {
        0,3,4,
        3,2,4,
        2,1,4,
        1,0,4,
        // base de la pirámide
        0,1,2,
        0,2,3
    };
    vector<GLfloat> vertices = {
        // base cuadrada
         0.5f, -0.5f,  0.5f, //0
         0.5f, -0.5f, -0.5f, //1
        -0.5f, -0.5f, -0.5f, //2
        -0.5f, -0.5f,  0.5f, //3
        // punta
         0.0f,  0.5f,  0.0f  //4
    };
    Mesh* piramide = new Mesh();
    piramide->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
    meshList.push_back(piramide);
}

// 2) CREAR EL PLANO (PISO) BAJO LA CASA
void CrearPlano()
{



    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    GLfloat vertices[] = {
        //  x     y    z
        -10.0f, 0.0f, -10.0f,  // 0
         10.0f, 0.0f, -10.0f,  // 1
         10.0f, 0.0f,  10.0f,  // 2
        -10.0f, 0.0f,  10.0f   // 3
    };

    Mesh* plano = new Mesh();
    plano->CreateMesh(vertices, indices, 12, 6);
    meshList.push_back(plano);
}

// 3) CREAR SHADERS
void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);

    Shader* shader2 = new Shader();
    shader2->CreateFromFiles(vShaderColor, fShader);
    shaderList.push_back(*shader2);
}

// 4) main()
int main()
{
    mainWindow = Window(800, 600);
    mainWindow.Initialise();


    // 0 -> Cubo
    CrearCubo();
    // 1 -> Pirámide Triangular
    CrearPiramideTriangular();
    // 2 -> Cilindro
    CrearCilindro(5, 1.0f);
    // 3 -> Cono
    CrearCono(25, 2.0f);
    // 4 -> Pirámide Cuadrangular (techo)
    CrearPiramideCuadrangular();
    // 5 -> Plano (piso)
    CrearPlano();

    CreateShaders();

    // Configuración de la cámara
    camera = Camera(
        glm::vec3(0.0f, 2.0f, 5.0f),   // posición inicial de la cámara
        glm::vec3(0.0f, 1.0f, 0.0f),   // vector up
        -90.0f,                       // yaw
        0.0f,                        // pitch
        0.3f,                        // velocidad de desplazamiento
        0.3f                         // velocidad de giro
    );

    // Variables para manejo de transformaciones
    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformColor = 0;

    // Proyección en perspectiva
    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),
        (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(),
        0.1f,
        100.0f
    );

    // Inicializar y cargar esfera (ventana trasera circular)
    sp.init();
    sp.load();

    // Bucle principal
    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;

        // Recibir eventos de usuario
        glfwPollEvents();

        // Controles de cámara
        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        // Limpiar la ventana y buffers
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();
        uniformView = shaderList[0].getViewLocation();
        uniformColor = shaderList[0].getColorLocation();

        // Matriz de modelo inicial
        glm::mat4 model(1.0f);

 
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));


        // (A) Piso
        model = glm::mat4(1.0f);
        // Trasladar el plano un poco hacia abajo
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        // Escalamos un poco si lo queremos más grande o más pequeño
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[5]->RenderMesh(); // Render del plano

        //
        // (B) Cubo grande de la casa (rojo)
        //
        model = glm::mat4(1.0f);
        // Trasladar para que se vea encima del piso
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -3.0f));
        // Escalado para que sea “grande” (la casa)
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        // color rojo
        color = glm::vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        // Recordar: meshList[0] = cubo
        meshList[0]->RenderMesh();

        //
        // (C) Techo piramidal cuadrangular (azul)
        //
        model = glm::mat4(1.0f);
        // Colocarla por encima del cubo de la casa (en y=1.0 + algo)
        model = glm::translate(model, glm::vec3(-3.0f, 1.55f, -3.0f));
        model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.0f, 0.0f, 1.0f); // azul
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[4]->RenderMeshGeometry(); // pirámide cuadrangular

        //
        // (D) Dos troncos (cubos cafés)
        //
        // Tronco árbol 1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0f, -0.5f, -3.0f));
        model = glm::scale(model, glm::vec3(0.3f, 1.0f, 0.3f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.5f, 0.3f, 0.1f); // café
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[0]->RenderMesh();

        // Tronco árbol 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, -0.5f, -3.0f));
        model = glm::scale(model, glm::vec3(0.3f, 1.0f, 0.3f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        // mismo color café
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[0]->RenderMesh();

        //
        // (E) Dos conos verdes (copas de los árboles)
        //
        // Copa árbol 1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0f, 0.3f, -3.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.3f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.0f, 0.6f, 0.0f); // verde oscuro
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[3]->RenderMeshGeometry();

        // Copa árbol 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.3f, -3.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.3f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        // mismo color verde
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[3]->RenderMeshGeometry();

        //
        // (F) Esfera para ventana trasera circular (azul)
        //
        model = glm::mat4(1.0f);
        // Por ejemplo, ponerla detrás del cubo de la casa
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -4.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.05f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        // Color azul para la esfera
        color = glm::vec3(0.0f, 0.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        sp.render();

   

        // 1) Ventana lado frontal 1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.3f, 0.3f, -1.95f)); // un poco al frente
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.1f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.0f, 1.0f, 0.0f); // verde
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[0]->RenderMesh();

        // 2) Ventana lado frontal 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.7f, 0.3f, -1.95f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.1f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[0]->RenderMesh();

        // Puerta (otro cubo)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, -0.6f, -2.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.8f, 0.1f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.3f, 0.6f, 0.3f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[0]->RenderMesh();

        // Ventanas laterales
        // Ventana lateral 1 (der)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.95f, 0.3f, -3.3f));
        model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        // color verde
        color = glm::vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[0]->RenderMesh();

        // Ventana lateral 2 (der)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.95f, 0.3f, -2.7f));
        model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[0]->RenderMesh();

        // Ventanas laterales
        // Ventana lateral 1 (izq)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.0f, 0.3f, -3.3f));
        model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        // color verde
        color = glm::vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[0]->RenderMesh();

        // Ventana lateral 2 (izq)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.0f, 0.3f, -2.7f));
        model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[0]->RenderMesh();
        // Terminamos de dibujar con este shader
        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}
