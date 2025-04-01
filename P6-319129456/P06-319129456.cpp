/*
Pr�ctica 6: Texturizado
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
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture llantaTexture;
Texture dado8Texture;


//Dado
Model Dado_M;

//Partes de Coche
Model Cabina_M;
Model Cofre_M;
Model Llanta_derecha_M;
Model Llanta_izquierda_M;
Model Llanta_opt;
Model Articulacion_M;


Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




//c�lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}



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

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);



	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};


	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	0.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	0.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	0.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	0.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.5f,  0.34f,		0.0f,	0.0f,	0.0f,	//4
		0.5f, -0.5f,  -0.5f,	0.74f,	0.34f,		0.0f,	0.0f,	0.0f,	//5
		0.5f,  0.5f,  -0.5f,	0.74f,	0.66f,		0.0f,	0.0f,	0.0f,	//6
		0.5f,  0.5f,  0.5f,	    0.5f,	0.66f,		0.0f,	0.0f,	0.0f,	//7

		// back
		-0.5f, -0.5f, -0.5f,	0.99f,  0.34f,		0.0f,	0.0f,	0.0f,	//8
		0.5f, -0.5f, -0.5f,		0.75f,	0.34f,		0.0f,	0.0f,	0.0f,	//9
		0.5f,  0.5f, -0.5f,		0.75f,	0.66f,		0.0f,	0.0f,	0.0f,	//10	
		-0.5f,  0.5f, -0.5f,	0.99f,	0.66f,		0.0f,	0.0f,	0.0f,	//11

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.5f,  0.01f,		0.0f,	0.0f,	0.0f,	//12
		-0.5f, -0.5f,  0.5f,	0.74f,	0.01f,		0.0f,	0.0f,	0.0f,	//13
		-0.5f,  0.5f,  0.5f,	0.74f,	0.33f,		0.0f,	0.0f,	0.0f,	//14
		-0.5f,  0.5f,  -0.5f,	0.5f,	0.33f,		0.0f,	0.0f,	0.0f,	//15

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.01f,  0.66f,		0.0f,	0.0f,	0.0f,	//16
		0.5f,  -0.5f,  0.5f,	0.24f,	0.66f,		0.0f,	0.0f,	0.0f,	//17
		 0.5f,  -0.5f,  -0.5f,	0.24f,	0.34f,		0.0f,	0.0f,	0.0f,	//18
		-0.5f, -0.5f,  -0.5f,	0.01f,	0.34f,		0.0f,	0.0f,	0.0f,	//19

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.51f,	0.67f,		0.0f,	0.0f,	0.0f,	//20
		 0.5f,  0.5f,  0.5f,	0.74f,	0.67f,		0.0f,	0.0f,	0.0f,	//21
		  0.5f, 0.5f,  -0.5f,	0.74f,	1.0f,		0.0f,	0.0f,	0.0f,	//22
		 -0.5f, 0.5f,  -0.5f,	0.51f,	1.0f,		0.0f,	0.0f,	0.0f,	//23

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);
}

void CrearOctaedro()
{
	// Puntos del octaedro (posiciones 3D estándar)
	glm::vec3 top(0.0f, 1.0f, 0.0f);      // Punto superior
	glm::vec3 bottom(0.0f, -1.0f, 0.0f);  // Punto inferior
	glm::vec3 eq1(1.0f, 0.0f, 0.0f);      // Punto del ecuador 1 (+X)
	glm::vec3 eq2(0.0f, 0.0f, 1.0f);      // Punto del ecuador 2 (+Z)
	glm::vec3 eq3(-1.0f, 0.0f, 0.0f);     // Punto del ecuador 3 (-X)
	glm::vec3 eq4(0.0f, 0.0f, -1.0f);     // Punto del ecuador 4 (-Z)

	// Coordenadas UV proporcionadas para cada cara
	// Cara 1
	glm::vec2 F(0.0862286671712f, 0.1142090927844f);
	glm::vec2 E(0.3611248587043f, 0.2739106897703f);
	glm::vec2 D(0.0862286671712f, 0.4205219919213f);

	// Cara 2
	glm::vec2 G(0.3718919993136f, 0.5758324080699f);

	// Cara 3
	glm::vec2 H(0.6545574282462f, 0.4287141438886f);

	// Cara 4
	glm::vec2 I(0.6529044140419f, 0.1179474734832f);

	// Cara 5
	glm::vec2 J(0.3720945299729f, 0.8888065895159f);
	glm::vec2 K(0.6525673585109f, 0.7346394056175f);

	// Cara 7 y 8
	glm::vec2 L(0.9404699308512f, 0.5804722217191f);
	glm::vec2 M(0.9438798226342f, 0.8885860373153f);

	// Array de vértices para las 8 caras (cada cara es un triángulo)
	GLfloat vertices[] = {
		// Cara 1: top, eq1, eq2 - Vertices con coordenadas F, E, D
		top.x, top.y, top.z,         F.x, F.y,      0.0f, 0.0f, 0.0f,
		eq1.x, eq1.y, eq1.z,         E.x, E.y,      0.0f, 0.0f, 0.0f,
		eq2.x, eq2.y, eq2.z,         D.x, D.y,      0.0f, 0.0f, 0.0f,

		// Cara 2: top, eq2, eq3 - Vertices con coordenadas E, D, G
		top.x, top.y, top.z,         E.x, E.y,      0.0f, 0.0f, 0.0f,
		eq2.x, eq2.y, eq2.z,         D.x, D.y,      0.0f, 0.0f, 0.0f,
		eq3.x, eq3.y, eq3.z,         G.x, G.y,      0.0f, 0.0f, 0.0f,

		// Cara 3: top, eq3, eq4 - Vertices con coordenadas E, H, G
		top.x, top.y, top.z,         E.x, E.y,      0.0f, 0.0f, 0.0f,
		eq3.x, eq3.y, eq3.z,         H.x, H.y,      0.0f, 0.0f, 0.0f,
		eq4.x, eq4.y, eq4.z,         G.x, G.y,      0.0f, 0.0f, 0.0f,

		// Cara 4: top, eq4, eq1 - Vertices con coordenadas E, H, I
		top.x, top.y, top.z,         E.x, E.y,      0.0f, 0.0f, 0.0f,
		eq4.x, eq4.y, eq4.z,         H.x, H.y,      0.0f, 0.0f, 0.0f,
		eq1.x, eq1.y, eq1.z,         I.x, I.y,      0.0f, 0.0f, 0.0f,

		// Cara 5: bottom, eq2, eq1 - Vertices con coordenadas J, K, G
		bottom.x, bottom.y, bottom.z, J.x, J.y,     0.0f, 0.0f, 0.0f,
		eq2.x, eq2.y, eq2.z,          K.x, K.y,     0.0f, 0.0f, 0.0f,
		eq1.x, eq1.y, eq1.z,          G.x, G.y,     0.0f, 0.0f, 0.0f,

		// Cara 6: bottom, eq3, eq2 - Vertices con coordenadas G, H, K
		bottom.x, bottom.y, bottom.z, G.x, G.y,     0.0f, 0.0f, 0.0f,
		eq3.x, eq3.y, eq3.z,          H.x, H.y,     0.0f, 0.0f, 0.0f,
		eq2.x, eq2.y, eq2.z,          K.x, K.y,     0.0f, 0.0f, 0.0f,

		// Cara 7: bottom, eq4, eq3 - Vertices con coordenadas H, K, L
		bottom.x, bottom.y, bottom.z, H.x, H.y,     0.0f, 0.0f, 0.0f,
		eq4.x, eq4.y, eq4.z,          K.x, K.y,     0.0f, 0.0f, 0.0f,
		eq3.x, eq3.y, eq3.z,          L.x, L.y,     0.0f, 0.0f, 0.0f,

		// Cara 8: bottom, eq1, eq4 - Vertices con coordenadas K, L, M
		bottom.x, bottom.y, bottom.z, K.x, K.y,     0.0f, 0.0f, 0.0f,
		eq1.x, eq1.y, eq1.z,          L.x, L.y,     0.0f, 0.0f, 0.0f,
		eq4.x, eq4.y, eq4.z,          M.x, M.y,     0.0f, 0.0f, 0.0f
	};

	// Índices para las 8 caras, cada cara es un triángulo
	unsigned int indices[] = {
		0, 1, 2,    // Cara 1
		3, 4, 5,    // Cara 2
		6, 7, 8,    // Cara 3
		9, 10, 11,  // Cara 4
		12, 13, 14, // Cara 5
		15, 16, 17, // Cara 6
		18, 19, 20, // Cara 7
		21, 22, 23  // Cara 8
	};

	// Calcular normales promedio para un sombreado suave
	calcAverageNormals(indices, 24, vertices, 24 * 8, 8, 5);

	Mesh* octaedro = new Mesh();
	octaedro->CreateMesh(vertices, indices, 24 * 8, 24);
	meshList.push_back(octaedro);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CreateShaders();
	CrearOctaedro();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado_animales_recorte.tga");
	dadoTexture.LoadTextureA();


	llantaTexture = Texture("Textures/llanta.tga");
	llantaTexture.LoadTextureA();

	dado8Texture = Texture("Textures/octa.jpg");
	dado8Texture.LoadTextureA();

	





	//----------------DADO-----------------------------
	Dado_M = Model();
	Dado_M.LoadModel("Models/dado_animales.obj");

	//----------------COCHE----------------------------
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
	glm::mat4 modelcar(1.0);
	glm::mat4 modelart_1(1.0);
	glm::mat4 modelart_2(1.0);
	glm::mat4 modelart_3(1.0);
	glm::mat4 modelart_4(1.0);

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
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la informaci�n de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();


		//Dado de Opengl
		//Ejercicio 1: Texturizar su cubo con la imagen dado_animales ya optimizada por ustedes
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//dadoTexture.UseTexture();
		//meshList[4]->RenderMesh();


		//Ejercicio 2:Importar el cubo texturizado en el programa de modelado con 
		//la imagen dado_animales ya optimizada por ustedes

		//Dado importado
		// Reemplazar el código de renderizado del dado de 10 caras
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-8.5f, 6.5f, -2.0f));
		//model = glm::scale(model, glm::vec3(5.0, 5.0, 5.0));
		//model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//dado8Texture.UseTexture();
		//meshList[5]->RenderMesh();


		//*/

		/*Reporte de pr�ctica :
		Ejercicio 1: Crear un dado de 8 caras y texturizarlo por medio de c�digo
		Ejercicio 2: Importar el modelo de su coche con sus 4 llantas acomodadas
		y tener texturizadas las 4 llantas (diferenciar caucho y rin)
		*/

		//OCTA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.5f, 6.5f, -2.0f));
		model = glm::scale(model, glm::vec3(5.0, 5.0, 5.0));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado8Texture.UseTexture();
		meshList[5]->RenderMesh();
		
		//*/



		//------------*INICIA DIBUJO DE COCHE-------------------*
		//Nodo Padre
		// Cabina (actualizada)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 2.5f, 20.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.getmover()));
		modelcar = model;
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cabina_M.RenderModel();



		//Cofre
		model = modelcar;
		model = glm::translate(model, glm::vec3(-0.5f, 4.5f, 4.12f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		//color = glm::vec3(1.0f, 1.0f, 0.0f); //color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_M.RenderModel();

		//cilindro 1
		model = modelcar;
		model = glm::translate(model, glm::vec3(-0.8f, -1.0f, 9.0f));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); //color gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		modelart_1 = model;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.11, 0.1, 0.1));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Articulacion_M.RenderModel();

		glm::vec3 colorLlanta = glm::vec3(0.8f, 0.8f, 0.8f);

		//Llanta_1
		model = modelart_1;
		model = glm::translate(model, glm::vec3(-6.5f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); //color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		Llanta_izquierda_M.RenderModel();


		//Llanta_2
		model = modelart_1;
		model = glm::translate(model, glm::vec3(7.3f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		//color = glm::vec3(0.0f, 0.0f, 0.0f); //color azul
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();

		Llanta_derecha_M.RenderModel();


		//cilindro 2
		model = modelcar;
		model = glm::translate(model, glm::vec3(-0.8f, -1.0f, -6.7f));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); //color gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		modelart_2 = model;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.11f, 0.1, 0.1));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Articulacion_M.RenderModel();

		//Llanta_3
		model = modelart_2;
		model = glm::translate(model, glm::vec3(-6.5f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		//color = glm::vec3(0.0f, 0.0f, 0.0f); //color gris
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		Llanta_izquierda_M.RenderModel();

		//Llanta_4
		model = modelart_2;
		model = glm::translate(model, glm::vec3(7.3f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		//color = glm::vec3(0.0f, 0.0f, 1.0f); //color azul
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorLlanta));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		Llanta_derecha_M.RenderModel();

		//*/
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/