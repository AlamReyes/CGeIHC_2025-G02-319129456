#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	angulocola = 0.0f;
	anguloboca = 0.0f;
	articulacion_1 = 0.0f;
	articulacion_2 = 0.0f;
	articulacion_3 = 0.0f;
	articulacion_4 = 0.0f;
	rotacionLlantas = 0.0f;
	mover = 0.0f;
	rotacionCofre = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 05: Optimizacion y Carga de Modelos", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
	// Asignar valores de la ventana y coordenadas

	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}


	if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}

	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}

	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}

	if (key == GLFW_KEY_C)
	{
		static bool bandera = true;
		// Verificar si el incremento nos llevar� m�s all� de los l�mites
		if (theWindow->angulocola != 50.0f && bandera)
		{
			// Si no excede los l�mites, se puede aplicar el incremento
			theWindow->angulocola += 10.0;
			if (theWindow->angulocola == 50.0)
			{
				//Se cambia la bandera para indicar que se llego al maximo y cambiara de direccion
				bandera = false;
			}
		}
		else if (theWindow->angulocola != -50.0f && !bandera)
		{
			// Si excede los l�mites, se puede aplicar el decremento
			theWindow->angulocola -= 10.0;
			if (theWindow->angulocola == -50.0f)
			{
				//Se cambia la bandera para indicar que se llego al minimo y cambiara de direccion
				bandera = true;
			}
		}
	}


	if (key == GLFW_KEY_B)
	{
		static bool bandera = true;
		if (theWindow->anguloboca != 14.0f && bandera)
		{
			theWindow->anguloboca += 1.0;
			if (theWindow->anguloboca == 14.0)
			{
				//Se cambia la bandera para indicar que se llego al maximo y cambiara de direccion
				bandera = false;
			}
		}
		else if (theWindow->anguloboca != -3.0f && !bandera)
		{
			theWindow->anguloboca -= 1.0;
			if (theWindow->anguloboca == -3.0f)
			{
				bandera = true;
			}
		}
	}

	if (key == GLFW_KEY_H)
	{
		static bool bandera = true;
		if (theWindow->articulacion_1 != 45.0f && bandera)
		{
			theWindow->articulacion_1 += 5.0;
			if (theWindow->articulacion_1 == 45.0)
			{
				bandera = false;
			}
		}
		else if (theWindow->articulacion_1 != -45.0f && !bandera)
		{
			theWindow->articulacion_1 -= 5.0;
			if (theWindow->articulacion_1 == -45.0f)
			{
				bandera = true;
			}
		}
	}

	if (key == GLFW_KEY_J)
	{
		static bool bandera = true;
		if (theWindow->articulacion_2 != 45.0f && bandera)
		{
			theWindow->articulacion_2 += 5.0;
			if (theWindow->articulacion_2 == 45.0)
			{
				bandera = false;
			}
		}
		else if (theWindow->articulacion_2 != -45.0f && !bandera)
		{
			theWindow->articulacion_2 -= 5.0;
			if (theWindow->articulacion_2 == -45.0f)
			{
				bandera = true;
			}
		}
	}

	if (key == GLFW_KEY_K)
	{
		static bool bandera = true;
		if (theWindow->articulacion_3 != 45.0f && bandera)
		{
			theWindow->articulacion_3 += 5.0;
			if (theWindow->articulacion_3 == 45.0)
			{
				bandera = false;
			}
		}
		else if (theWindow->articulacion_3 != -45.0f && !bandera)
		{
			theWindow->articulacion_3 -= 5.0;
			if (theWindow->articulacion_3 == -45.0f)
			{
				bandera = true;
			}
		}
	}

	if (key == GLFW_KEY_L)
	{
		static bool bandera = true;
		if (theWindow->articulacion_4 != 45.0f && bandera)
		{
			theWindow->articulacion_4 += 5.0;
			if (theWindow->articulacion_4 == 45.0)
			{
				bandera = false;
			}
		}
		else if (theWindow->articulacion_4 != -45.0f && !bandera)
		{
			theWindow->articulacion_4 -= 5.0;
			if (theWindow->articulacion_4 == -45.0f)
			{
				bandera = true;
			}
		}
	}


	if (key == GLFW_KEY_Z)
	{
		theWindow->mover += 1.0;
		theWindow->rotacionLlantas += 10.0;
	}

	if (key == GLFW_KEY_X)
	{
		theWindow->mover -= 1.0;
		theWindow->rotacionLlantas -= 10.0;

	}

	if (key == GLFW_KEY_C)
	{
		static bool bandera = true;
		if (theWindow->rotacionCofre != -55.0f && bandera)
		{
			theWindow->rotacionCofre -= 1.0;
			if (theWindow->rotacionCofre == -55.0)
			{
				bandera = false;
			}
		}
		else if (theWindow->rotacionCofre != 0.0f && !bandera)
		{
			theWindow->rotacionCofre += 1.0;
			if (theWindow->rotacionCofre == 0.0f)
			{
				bandera = true;
			}
		}
	}


	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key >= 0 && key < 1024)
	{

		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}

		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}

}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}