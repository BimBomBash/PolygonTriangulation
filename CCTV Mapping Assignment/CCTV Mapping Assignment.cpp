// CCTV Mapping Assignment.cpp : Defines the entry point for the console application.
//
#define SCALE 20

#include "stdafx.h"
#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include <string.h>
#include <list>
#include "Window.h"
#include "Polygon.h"
#include <queue>

std::list <Vertex*> vertices;
Polygon *polygon;
Window *window;
std::priority_queue<Vertex*> verticesQueue;

void InitPolygon() {
	vertices.push_back(new Vertex(1.0f, 1.0f));
	vertices.push_back(new Vertex(0.0f, 1.0f));
	vertices.push_back(new Vertex(-1.0f, 1.0f));

	polygon = new Polygon(vertices);
}

void PrintLine(Line *line) {
	window->DrawLine(line->a->x, line->a->y, line->b->x, line->b->y);
}

void initQueue() {
	for (auto iter = vertices.begin(); iter != vertices.end(); iter++) {
		verticesQueue.push(*iter);
	}

}

int main(int argc, char **argv)
{
	InitPolygon();
	initQueue();
	//polygon->Print();
	window =  new Window ("CCTV", 600, 400);
	if (window->initiated) {
		printf("Window Created Successfully");
		window->ClearWindow();
		for (auto iter = polygon->lines.begin(); iter != polygon->lines.end(); iter++) {
			PrintLine(*iter);
		}
		polygon->Print();
		window->SwapWindow();
	}
	while (true) {};
    return 0;
}

