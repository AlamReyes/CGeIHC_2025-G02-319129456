// main.cpp
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;   // Pirámide y cubo
std::vector<Shader> shaderList;

// Shaders
static const char* vRojo = "shaders/shaderRojo.vert";
static const char* fRojo = "shaders/shaderRojo.frag";
static const char* vVerde = "shaders/shaderVerde.vert";
static const char* fVerde = "shaders/shaderVerde.frag";
static const char* vAzul = "shaders/shaderAzul.vert";
static const char* fAzul = "shaders/shaderAzul.frag";
static const char* vCafe = "shaders/shaderCafe.vert";
static const char* fCafe = "shaders/shaderCafe.frag";
static const char* vVerdeOsc = "shaders/shaderVerdeOsc.vert";
static const char* fVerdeOsc = "shaders/shaderVerdeOsc.frag";

void CreaPiramide()
{
    // Indices y vertices de ejemplo simple (piramide sin base)
    unsigned int indices[] = {
        0, 1, 2,
        1, 3, 2,
        3, 0, 2
    };
    GLfloat vertices[] = {
        // Base "triangular" en plano XY y un vértice superior algo desplazado en Z
        -0.5f, -0.5f,  0.0f,  // 0
         0.5f, -0.5f,  0.0f,  // 1
         0.0f,  0.5f, -0.25f, // 2 (punta)
         0.0f, -0.5f, -0.5f,  // 3 (tercer vértice de la base)
    };

    Mesh* piramide = new Mesh();
    piramide->CreateMesh(vertices, indices, 12, 9);
    meshList.push_back(piramide); // meshList[0]
}

void CrearCubo()
{
    unsigned int cuboIndices[] = {
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

    GLfloat cuboVertices[] = {
        // front
        -0.5f, -0.5f,  0.5f, // 0
         0.5f, -0.5f,  0.5f, // 1
         0.5f,  0.5f,  0.5f, // 2
        -0.5f,  0.5f,  0.5f, // 3
        // back
        -0.5f, -0.5f, -0.5f, // 4
         0.5f, -0.5f, -0.5f, // 5
         0.5f,  0.5f, -0.5f, // 6
        -0.5f,  0.5f, -0.5f  // 7
    };

    Mesh* cubo = new Mesh();
    cubo->CreateMesh(cuboVertices, cuboIndices, 24, 36);
    meshList.push_back(cubo); // meshList[1]
}

void CreateShaders()
{
    // Rojo
    {
        Shader* s = new Shader();
        s->CreateFromFiles(vRojo, fRojo);
        shaderList.push_back(*s); // 0 -> rojo
    }
    // Verde
    {
        Shader* s = new Shader();
        s->CreateFromFiles(vVerde, fVerde);
        shaderList.push_back(*s); // 1 -> verde
    }
    // Azul
    {
        Shader* s = new Shader();
        s->CreateFromFiles(vAzul, fAzul);
        shaderList.push_back(*s); // 2 -> azul
    }
    // Café
    {
        Shader* s = new Shader();
        s->CreateFromFiles(vCafe, fCafe);
        shaderList.push_back(*s); // 3 -> café
    }
    // Verde oscuro
    {
        Shader* s = new Shader();
        s->CreateFromFiles(vVerdeOsc, fVerdeOsc);
        shaderList.push_back(*s); // 4 -> verde oscuro
    }
}

int main()
{
    // Inicializar ventana
    mainWindow = Window(1024, 768);
    mainWindow.Initialise();

    //Habilitar depth test:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Crear geometrías
    CreaPiramide(); // meshList[0]
    CrearCubo();    // meshList[1]

    // Cargar shaders
    CreateShaders();

    // Proyección ortográfica
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f,
        -1.0f, 1.0f,
        0.1f, 100.0f);

    GLuint uniformModel = 0, uniformProjection = 0;

    while (!mainWindow.getShouldClose())
    {
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //
        // -------------------------------- CASA (Cubo Rojo) --------------------------------
        //
        shaderList[0].useShader(); // 0 -> rojo
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();

        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -0.05f, -4.0f));
            model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            meshList[1]->RenderMesh();
        }

        //
        // -------------------------------- TECHO (Pirámide Azul) ----------------------------
        //
        shaderList[2].useShader(); // 2 -> azul
        uniformModel = shaderList[2].getModelLocation();
        uniformProjection = shaderList[2].getProjectLocation();

        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.6f, -3.9f));
            model = glm::scale(model, glm::vec3(0.7f, 0.6f, 0.7f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            // Dibuja pirámide -> techo
            meshList[0]->RenderMesh();
        }

        //
        // --------------------------------- VENTANAS (Cubos Verdes) -------------------------
        //
        shaderList[1].useShader(); // 1 -> verde
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();

        // Ventana Izquierda
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-0.15f, 0.12f, -3.64f));
            model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.1f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            meshList[1]->RenderMesh();
        }

        // Ventana Derecha
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.15f, 0.12f, -3.64f));
            model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.1f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            meshList[1]->RenderMesh();
        }

        //
        // --------------------------------- PUERTA (Cubo Verde) -----------------------------
        //
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -0.23f, -3.64f));
            model = glm::scale(model, glm::vec3(0.2f, 0.35f, 0.1f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            meshList[1]->RenderMesh();
        }

        //
        // --------------------------------- ÁRBOL IZQUIERDO ---------------------------------
        //
        // Tronco Café (Cubo)
        shaderList[3].useShader(); // 3 -> café
        uniformModel = shaderList[3].getModelLocation();
        uniformProjection = shaderList[3].getProjectLocation();

        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-0.7f, -0.3f, -4.0f));
            model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.1f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            meshList[1]->RenderMesh(); // cubo
        }

        // Copa Verde Oscura (Pirámide)
        shaderList[4].useShader(); // 4 -> verde oscuro
        uniformModel = shaderList[4].getModelLocation();
        uniformProjection = shaderList[4].getProjectLocation();

        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-0.7f, 0.05f, -4.0f));
            model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.3f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            meshList[0]->RenderMesh(); // pirámide
        }

        //
        // --------------------------------- ÁRBOL DERECHO ------------------------------------
        //
        // Tronco café
        shaderList[3].useShader();
        uniformModel = shaderList[3].getModelLocation();
        uniformProjection = shaderList[3].getProjectLocation();

        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.7f, -0.3f, -4.0f));
            model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.1f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            meshList[1]->RenderMesh();
        }

        // Copa verde oscuro
        shaderList[4].useShader();
        uniformModel = shaderList[4].getModelLocation();
        uniformProjection = shaderList[4].getProjectLocation();

        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.7f, 0.05f, -4.0f));
            model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.3f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            meshList[0]->RenderMesh();
        }

        // Desactiva el shader
        glUseProgram(0);

        // Intercambiar buffers
        mainWindow.swapBuffers();
    }

    return 0;
}
