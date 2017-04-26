#include "stdafx.h"
#include <GL\glew.h>
#include <SDL.h>
#include <iostream>
#include <string.h>
#include "Window.h"

Window::Window(char* _windowName, int _windowX, int _windowY)
{
	initiated = Init(_windowName, _windowX, _windowY);
	windowHeight = _windowY;
	windowWidth = _windowX;
	if (windowHeight > windowWidth) {
		yMultiplier = (float)windowWidth / (float)windowHeight;
		xMultiplier = 1.0f;
	}
	else {
		xMultiplier = (float)windowHeight / (float)windowWidth;
		yMultiplier = 1.0f;
	}
}

Window::~Window()
{
}

void Window::Close()
{
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}


void Window::DrawLine(float x1, float y1, float x2, float y2)
{
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glVertex2f(x1*xMultiplier, y1*yMultiplier);
	glVertex2f(x2*xMultiplier, y2*yMultiplier);
	glEnd();

}

void Window::SwapWindow()
{
	SDL_GL_SwapWindow(gWindow);
	SDL_Delay(1000);
}

void Window::ClearWindow()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



bool Window::Init(char* _windowName, int _windowX, int _windowY)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf ("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
		return false;
	}
	gWindow = SDL_CreateWindow(_windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowX, _windowY, SDL_WINDOW_OPENGL);

	if (gWindow == nullptr) {
		printf("Window could not be created. SDL Error: %s\n", SDL_GetError());
		return false;
	}

	gContext = SDL_GL_CreateContext(gWindow);
	
	if (gContext == nullptr) {
		printf("OpenGL Context could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	if (glewInit() != GLEW_OK) {
		printf("OpenGL could not be initiated.");
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	return true;
}
