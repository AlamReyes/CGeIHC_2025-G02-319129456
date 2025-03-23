#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }

	GLfloat getangulocola() { return angulocola; }
	GLfloat getanguloboca() { return anguloboca; }

	GLfloat getarticulacion_1() { return articulacion_1; }
	GLfloat getarticulacion_2() { return articulacion_2; }
	GLfloat getarticulacion_3() { return articulacion_3; }
	GLfloat getarticulacion_4() { return articulacion_4; }

	GLfloat getrotacionLlantas() { return rotacionLlantas; }
	GLfloat getmover() { return mover; }
	GLfloat getrotacionCofre() { return rotacionCofre; }

	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLfloat rotax, rotay, rotaz;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat angulocola, anguloboca, articulacion_1, articulacion_2, articulacion_3, articulacion_4;
	GLfloat rotacionLlantas, mover, rotacionCofre;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};