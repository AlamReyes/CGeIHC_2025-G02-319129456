/*
Pr�ctica 5: Optimizaci�n y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Model Goddard_M;
Model Goddard_sinColaOBJ_M;
Model Goddard_sinColaFBX_M;

//Partes de Goodard
Model Cuerpo_M;
Model Pata1_M;
Model Pata2_M;
Model Pata3_M;
Model Pata4_M;
Model Cabeza_M;
Model Mandibula_Inferior_M;
Model Cola_M;


//Partes de Coche
Model Cabina_M;
Model Cofre_M;
Model Llanta_derecha_M;
Model Llanta_izquierda_M;
Model Articulacion_M;

//Cielo
Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";





void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};


	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.7f, 1.0f);

	Goddard_M = Model();
	Goddard_M.LoadModel("Models/goddard_base.obj");
	Goddard_sinColaOBJ_M = Model();

	//Partes Goddar
	Cuerpo_M = Model();
	Cuerpo_M.LoadModel("Models/cuerpo.obj");

	Pata1_M = Model();
	Pata1_M.LoadModel("Models/pierna_delantera_1.obj");

	Pata2_M = Model();
	Pata2_M.LoadModel("Models/pierna_delantera_2.obj");

	Pata3_M = Model();
	Pata3_M.LoadModel("Models/pierna_trasera_1.obj");

	Pata4_M = Model();
	Pata4_M.LoadModel("Models/pierna_trasera_2.obj");

	Cabeza_M = Model();
	Cabeza_M.LoadModel("Models/cabeza.obj");

	Mandibula_Inferior_M = Model();
	Mandibula_Inferior_M.LoadModel("Models/mandibula_inferior.obj");

	Cola_M = Model();
	Cola_M.LoadModel("Models/cola.obj");

	// PARTES DEL COCHE 

	Cabina_M = Model();
	Cabina_M.LoadModel("Models/jeep_cabina.obj");

	Cofre_M = Model();
	Cofre_M.LoadModel("Models/jeep_cofre.obj");

	Llanta_derecha_M = Model();
	Llanta_derecha_M.LoadModel("Models/jeep_llanta_derecha.obj");


	Llanta_izquierda_M = Model();
	Llanta_izquierda_M.LoadModel("Models/jeep_llanta_izquierda.obj");

	Articulacion_M = Model();
	Articulacion_M.LoadModel("Models/jeep_articulacion.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelhead(1.0);

	glm::mat4 modelcar(1.0);
	glm::mat4 modelart_1(1.0);
	glm::mat4 modelart_2(1.0);

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();


		//El cuerpo de Goddard
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(0.0f, 2.6f, -1.5f)); //0.0f, 0.4f, -1.5f
		//modelaux = model;
		//modelhead = model;
		//color = glm::vec3(0.0f, 0.0f, 0.0f); //modelo de goddard de color negro
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Cuerpo_M.RenderModel();


		////Cola
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(-1.5f, -0.2f, 0.0));
		//model = glm::rotate(model, glm::radians(mainWindow.getangulocola()), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Cola_M.RenderModel();


		////pata delantera izquierda
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(1.0f, -0.53f, -0.7f));
		//modelaux = model;
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_1()), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Pata1_M.RenderModel();

		//// pata delantera derecha
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(0.0f, -0.037f, 1.4f));
		//modelaux = model;
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_2()), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Pata2_M.RenderModel();


		////pata trasera derecha
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(-1.8f, -0.63f, 0.0f));
		//modelaux = model;
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_3()), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Pata4_M.RenderModel();

		////pata trasera izquierda
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(0.1f, -0.06f, -1.4f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_4()), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Pata3_M.RenderModel();

		////cabeza
		//model = modelhead;
		//model = glm::translate(model, glm::vec3(1.35f, 0.6f, 0.0f));
		//modelhead = model;
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.0f, 0.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//Cabeza_M.RenderModel();

		////mandibula
		//model = modelhead;
		//model = glm::translate(model, glm::vec3(0.98f, 0.12f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloboca()), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//Mandibula_Inferior_M.RenderModel();

		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.getmover()));
		modelcar = model;
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));

		color = glm::vec3(0.55f, 0.27f, 0.07f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cabina_M.RenderModel();


		//Cofre
		model = modelcar;
		model = glm::translate(model, glm::vec3(0.0f, 4.5f, 4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		color = glm::vec3(0.33f, 0.42f, 0.18f); // color verde militar
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_M.RenderModel();

		//cilindro 1
		model = modelcar;
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 9.0f));
		modelart_1 = model;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		color = glm::vec3(1.0f, 0.0f, 0.0f); //color rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Articulacion_M.RenderModel();

		//Llanta_1
		model = modelart_1;
		model = glm::translate(model, glm::vec3(-6.5f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //negra
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_derecha_M.RenderModel();
		Llanta_izquierda_M.RenderModel();

		//Llanta_2
		model = modelart_1;
		model = glm::translate(model, glm::vec3(6.5f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Negra
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_izquierda_M.RenderModel();
		Llanta_derecha_M.RenderModel();

		//cilindro 2
		model = modelcar;
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -7.0f));
		modelart_2 = model;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		color = glm::vec3(1.0f, 0.0f, 0.0f); //color rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Articulacion_M.RenderModel();

		//Llanta_3
		model = modelart_2;
		model = glm::translate(model, glm::vec3(-6.5f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Negra
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_derecha_M.RenderModel();
		Llanta_izquierda_M.RenderModel();

		//Llanta_4
		model = modelart_2;
		model = glm::translate(model, glm::vec3(6.5f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //Negra
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_izquierda_M.RenderModel();
		Llanta_derecha_M.RenderModel();



		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}