// CCTV Mapping Assignment.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include <string.h>
#include <list>
#include "Window.h"
#include "Polygon.h"
#include <queue>

const double PI = 3.1415926535897932384626433832795;
const float scale = 10.0f;
const float rotation = 30.0f;
Edge *edges;
Polygon *polygon;
Window *window;
int edgeNumber = 0;
std::priority_queue < Vertex*, std::vector<Vertex*>, OrderByVerticesY > verticesQueue;

void InitPolygon() {
	polygon = new Polygon(edges);

	polygon->Push_Back(new Edge(-10.0f, -4.0f, polygon));
	polygon->Push_Back(new Edge(-10.0f, 0.0f, polygon));
	polygon->Push_Back(new Edge(-6.0f, 0.0f, polygon));
	polygon->Push_Back(new Edge(-6.0f, 1.0f, polygon));
	polygon->Push_Back(new Edge(-10.0f, 1.0f, polygon));
	polygon->Push_Back(new Edge(-10.0f, 5.0f, polygon));
	polygon->Push_Back(new Edge(-4.0f, 5.0f, polygon));
	polygon->Push_Back(new Edge(-4.0f, 0.0f, polygon));
	polygon->Push_Back(new Edge(-3.0f, 0.0f, polygon));
	polygon->Push_Back(new Edge(-3.0f, 5.0f, polygon));
	polygon->Push_Back(new Edge(4.0f, 5.0f, polygon));
	polygon->Push_Back(new Edge(4.0f, 1.0f, polygon));
	polygon->Push_Back(new Edge(-1.0f, 1.0f, polygon));
	polygon->Push_Back(new Edge(-1.0f, 0.0f, polygon));
	polygon->Push_Back(new Edge(5.0f, 0.0f, polygon));
	polygon->Push_Back(new Edge(5.0f, 5.0f, polygon));
	polygon->Push_Back(new Edge(10.0f, 5.0f, polygon));
	polygon->Push_Back(new Edge(10.0f, -5.0f, polygon));
	polygon->Push_Back(new Edge(5.0f, -5.0f, polygon));
	polygon->Push_Back(new Edge(5.0f, -4.0f, polygon));
	polygon->Push_Back(new Edge(2.0f, -4.0f, polygon));
	polygon->Push_Back(new Edge(2.0f, -5.0f, polygon));
	polygon->Push_Back(new Edge(-6.0f, -5.0f, polygon));
	polygon->Push_Back(new Edge(-6.0f, -2.0f, polygon));
	polygon->Push_Back(new Edge(-7.0f, -2.0f, polygon));
	polygon->Push_Back(new Edge(-7.0f, -4.0f, polygon));
}

void RotatePolygon() {
	Edge *iter = polygon->edges;
	do {
		float tempX = iter->origin->x;
		float tempY = iter->origin->y;
		iter->origin->x = tempX * cos((float)rotation*PI / 180.0f) - tempY * sin((float)rotation*PI / 180.0f);
		iter->origin->y = tempX * sin((float)rotation*PI / 180.0f) + tempY*cos((float)rotation*PI / 180.0f);
		iter = iter->next;
	}while (iter != polygon->Start());
}

void SetVertexType() {
	Edge *iter = polygon->edges;
	do {
		if (iter->prev->origin->y < iter->origin->y && iter->next->origin->y < iter->origin->y) {
			if (iter->prev->origin->x < iter->next->origin->x) {
				iter->origin->type = START;
			} else if (iter->prev->origin->x > iter->next->origin->x) {
				iter->origin->type = SPLIT;
			}
		}else if(iter->prev->origin->y > iter->origin->y && iter->next->origin->y > iter->origin->y) {
			if (iter->prev->origin->x < iter->next->origin->x) {
				iter->origin->type = MERGE;
			}
			else if (iter->prev->origin->x > iter->next->origin->x) {
				iter->origin->type = END;
			}
		}
		else {
			iter->origin->type = REGULAR;
		}
		iter = iter->next;
	} while (iter != polygon->Start());
}

void PrintVerticesType() {
	Edge *iter = polygon->edges;
	do {
		switch (iter->origin->type)
		{
		case START:
			std::cout << "START" << std::endl;
			break;
		case END:
			std::cout << "END" << std::endl;
			break;
		case MERGE:
			std::cout << "MERGE" << std::endl;
			break;
		case SPLIT:
			std::cout << "SPLIT" << std::endl;
			break;
		case REGULAR:
			std::cout << "REGULAR" << std::endl;
			break;
		};
		iter = iter->next;
	} while (iter != polygon->Start());
}

void PrintLine(Edge *line) {
	window->DrawLine(line->origin->x / scale, line->origin->y / scale, line->next->origin->x / scale, line->next->origin->y / scale);
}

void InitVerticesQueue() {
	Edge *iter = polygon->edges;
	do {
		verticesQueue.push(iter->origin);
		iter = iter->next;
	} while (iter != polygon->Start());
}

//FOR DEBUGGING! DON'T USE IT WHILE NOT DEBUGGING!
void PrintVerticesQueue() {
	while (!verticesQueue.empty())
	{
		verticesQueue.top()->Print();
		verticesQueue.pop();
	}
}

int main(int argc, char **argv)
{
	window =  new Window ("CCTV", 600, 400);
	
	InitPolygon();
	if (polygon->edges == nullptr)printf("NULLPTR");
	
	RotatePolygon();
	SetVertexType();
	polygon->Print();
	
	InitVerticesQueue();
	
	if (window->initiated) {
		window->ClearWindow();
		Edge *iter = polygon->edges;
		do {
			PrintLine(iter);
			iter = iter->next;
		} while (iter != polygon->Start());
		window->SwapWindow();
	}
	while (true) {};
    return 0;
}

