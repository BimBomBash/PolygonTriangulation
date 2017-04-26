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
#include "BinarySearchTree.h"
#include <queue>

const double PI = 3.1415926535897932384626433832795;
const float scale = 10.0f;
const float rotation = 30.0f;
Edge *edges;
Polygon *polygon;
Window *window;
int edgeNumber = 0;
std::priority_queue < Vertex*, std::vector<Vertex*>, OrderByVerticesY > verticesQueue;
BinaryTree binarySearchTree;

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

//Set START, END, MERGE, SPLIT and add key to each edges and verices
void SetVertexType() {
	Edge *iter = polygon->edges;
	int key = 0;
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
		iter->key = key;
		iter->origin->key = key;
		key++;
		iter = iter->next;
	} while (iter != polygon->Start());
}

void PrintVerticesType() {
	Edge *iter = polygon->edges;
	do {
		switch (iter->origin->type)
		{
		case START:
			std::cout << "Edge " << iter->key << ": " << "START" << std::endl;
			break;
		case END:
			std::cout << "Edge " << iter->key << ": " << "END" << std::endl;
			break;
		case MERGE:
			std::cout << "Edge " << iter->key << ": " << "MERGE" << std::endl;
			break;
		case SPLIT:
			std::cout << "Edge " << iter->key << ": " << "SPLIT" << std::endl;
			break;
		case REGULAR:
			std::cout << "Edge " << iter->key << ": " << "REGULAR" << std::endl;
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

void HandleStartVertex(Vertex *_vertex) {
	binarySearchTree.insert(_vertex->incidentEdge);
}

void HandleEndVertex(Vertex *_vertex) {}

void HandleSplitVertex(Vertex *_vertex) {}

void HandleMergeVertex(Vertex *_vertex) {}

void HandleRegularVertex(Vertex *_vertex) {}

void MakeMonotone() {
	while (!verticesQueue.empty()) {
		Vertex *temp = verticesQueue.top();
		switch (temp->type)
		{
		case START:
			HandleStartVertex(temp);
			break;
		case END:
			HandleEndVertex(temp);
			break;
		case MERGE:
			HandleMergeVertex(temp);
			break;
		case SPLIT:
			HandleSplitVertex(temp);
			break;
		case REGULAR:
			HandleRegularVertex(temp);
			break;
		};
		verticesQueue.pop();
	};
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

	MakeMonotone();

	while (true) {};
    return 0;
}

