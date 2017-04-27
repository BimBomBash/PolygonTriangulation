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
#include <vector>

const double PI = 3.1415926535897932384626433832795;
const float scale = 10.0f;
const float rotation = 30.0f;
Edge *edges;
Polygon *polygon;
Window *window;
int edgeNumber = 0;
std::priority_queue < Vertex*, std::vector<Vertex*>, OrderByVerticesY > verticesQueue;
BinaryTree binarySearchTree;
std::vector<Node*>fakeTree;
std::vector<Vertex*>splitterSource;
std::vector<Edge*>splitterTarget;
std::vector<Polygon*> monotones;

void InitPolygon() {
	polygon = new Polygon(edges);

	polygon->Push_Back(new Edge(-10.0f, -4.0f, polygon));
	polygon->Push_Back(new Edge(-7.0f, -4.0f, polygon));
	polygon->Push_Back(new Edge(-7.0f, -2.0f, polygon));
	polygon->Push_Back(new Edge(-6.0f, -2.0f, polygon));
	polygon->Push_Back(new Edge(-6.0f, -5.0f, polygon));
	polygon->Push_Back(new Edge(2.0f, -5.0f, polygon));
	polygon->Push_Back(new Edge(2.0f, -4.0f, polygon));
	polygon->Push_Back(new Edge(5.0f, -4.0f, polygon));
	polygon->Push_Back(new Edge(5.0f, -5.0f, polygon));
	polygon->Push_Back(new Edge(10.0f, -5.0f, polygon));
	polygon->Push_Back(new Edge(10.0f, 5.0f, polygon));
	polygon->Push_Back(new Edge(5.0f, 5.0f, polygon));
	polygon->Push_Back(new Edge(5.0f, 0.0f, polygon));
	polygon->Push_Back(new Edge(-1.0f, 0.0f, polygon));
	polygon->Push_Back(new Edge(-1.0f, 1.0f, polygon));
	polygon->Push_Back(new Edge(4.0f, 1.0f, polygon));
	polygon->Push_Back(new Edge(4.0f, 5.0f, polygon));
	polygon->Push_Back(new Edge(-3.0f, 5.0f, polygon));
	polygon->Push_Back(new Edge(-3.0f, 0.0f, polygon));
	polygon->Push_Back(new Edge(-4.0f, 0.0f, polygon));
	polygon->Push_Back(new Edge(-4.0f, 5.0f, polygon));
	polygon->Push_Back(new Edge(-10.0f, 5.0f, polygon));
	polygon->Push_Back(new Edge(-10.0f, 1.0f, polygon));
	polygon->Push_Back(new Edge(-6.0f, 1.0f, polygon));
	polygon->Push_Back(new Edge(-6.0f, 0.0f, polygon));
	polygon->Push_Back(new Edge(-10.0f, 0.0f, polygon));
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
			if (iter->prev->origin->x > iter->next->origin->x) {
				iter->origin->type = START;
			} else if (iter->prev->origin->x < iter->next->origin->x) {
				iter->origin->type = SPLIT;
			}
		}else if(iter->prev->origin->y > iter->origin->y && iter->next->origin->y > iter->origin->y) {
			if (iter->prev->origin->x > iter->next->origin->x) {
				iter->origin->type = MERGE;
			}
			else if (iter->prev->origin->x < iter->next->origin->x) {
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

void InsertToFakeTree(Edge *_edge, Vertex *_helper) {
	Node *node = new Node();
	node->edge = _edge;
	node->helper = _helper;
	if (fakeTree.empty())fakeTree.push_back(node);
	else {
		for (int i = 0; i < fakeTree.size(); i++) {
			if (i == fakeTree.size() - 1) {
				if (_edge->origin->x >= fakeTree[i]->edge->origin->x) fakeTree.push_back(node);
				else fakeTree.insert(fakeTree.begin() + i, node);
				break;
			}else if (_edge->origin->x <= fakeTree[i]->edge->origin->x) {
				std::cout << _edge->origin->x << fakeTree[i]->edge->origin->x;
				fakeTree.insert(fakeTree.begin() + i, node);
				break;
			}
		}
	}
}

Node *DirectLeft(Vertex *_helper) {
	for (int i = fakeTree.size()-1; i >= 0; i--) {
		if (i == 0) {
			return fakeTree[i];
			break;
		}
		else if (_helper->x >= fakeTree[i]->edge->origin->x) {
			return fakeTree[i];
			break;
		}
	}

}

Node *SearchNode(Edge *_edge) {
	for (int i = 0; i < fakeTree.size(); i++) {
		if (fakeTree[i]->edge == _edge) {
			return fakeTree[i];
		}
	}
}

void DeleteEdge(Edge *_edge) {
	for (int i = 0; i < fakeTree.size(); i++) {
		if (fakeTree[i]->edge == _edge) {
			fakeTree.erase(fakeTree.begin() + i);
			break;
		}
	}
}

void InsertSplitter(Vertex *_source, Edge *_target) {
	splitterSource.push_back(_source);
	splitterTarget.push_back(_target);
}

void SplitPolygon(Vertex *_source, Edge *_target) {
	Polygon *oldPolygon = _target->incidentFace;
	Polygon *newPolygon = new Polygon();
	Edge *sourceEdge = new Edge(_source, newPolygon);
	Edge *sourceTwin = new Edge(_target->origin, oldPolygon);
	sourceTwin->prev = _target->prev;
	_target->prev->next = sourceTwin;
	sourceTwin->next = _source->incidentEdge->prev->next;

	sourceEdge->next = _target;
	sourceEdge->prev = _source->incidentEdge->prev;
	_target->prev = sourceEdge;
	sourceEdge->prev->next = sourceEdge;
	newPolygon->edges = sourceEdge;

	sourceEdge->origin->incidentEdge = sourceEdge;
	sourceTwin->origin->incidentEdge = sourceTwin;

	Edge *temp = sourceEdge;
	do {
		if (temp == oldPolygon->edges) edges = sourceTwin;
		temp->incidentFace = newPolygon;
		temp = temp->next;
	} while (temp != sourceEdge);

	newPolygon->edges = sourceEdge;
	monotones.push_back(newPolygon);
}

void HandleStartVertex(Vertex *_vertex) {
	InsertToFakeTree(_vertex->incidentEdge, _vertex);
}

void HandleEndVertex(Vertex *_vertex) {
	Vertex *prevHelper = SearchNode(_vertex->incidentEdge->prev)->helper;
	if (prevHelper->type == MERGE) {
		InsertSplitter(_vertex, prevHelper->incidentEdge);
	}
	DeleteEdge(_vertex->incidentEdge->prev);
}

void HandleSplitVertex(Vertex *_vertex) {
	Node *left = DirectLeft(_vertex);
	InsertSplitter(_vertex, left->helper->incidentEdge);
	left->helper = _vertex;
	InsertToFakeTree(_vertex->incidentEdge, _vertex);
}

void HandleMergeVertex(Vertex *_vertex) {
	Vertex *prevHelper = SearchNode(_vertex->incidentEdge->prev)->helper;
	if (SearchNode(_vertex->incidentEdge->prev)->helper->type == MERGE) {
		InsertSplitter(_vertex, prevHelper->incidentEdge);
	}
	DeleteEdge(_vertex->incidentEdge->prev);
	Node *left = DirectLeft(_vertex);
	Vertex *helper = SearchNode(left->edge)->helper;
	if (helper->type == MERGE) {
		InsertSplitter(_vertex, helper->incidentEdge);
	}
	left->helper = _vertex;
}

void HandleRegularVertex(Vertex *_vertex) {
	if (_vertex->y > _vertex->incidentEdge->next->origin->y) {
		Vertex *prevHelper = SearchNode(_vertex->incidentEdge->prev)->helper;
		if (prevHelper->type == MERGE) {
			InsertSplitter(_vertex,prevHelper->incidentEdge);
		}
		DeleteEdge(SearchNode(_vertex->incidentEdge->prev)->edge);
		InsertToFakeTree(_vertex->incidentEdge, _vertex);
	}
	else {
		Node *left = DirectLeft(_vertex);
		if (left->helper->type == MERGE) {
			InsertSplitter(_vertex, left->helper->incidentEdge);
		}
		left->helper = _vertex;
	}
}

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
		for (int i = 0; i < fakeTree.size(); i++) {
			std::cout << "(" << fakeTree[i]->edge->key << ", " << fakeTree[i]->helper->key << ") ";
		}
		std::cout << std::endl;
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
	//PrintVerticesType();
	//polygon->Print();
	
	InitVerticesQueue();

	MakeMonotone();

	//for (int i = 0; i < splitterSource.size(); i++) {
		//SplitPolygon(splitterSource[i], splitterTarget[i]);
	//}
	
	if (window->initiated) {
		window->ClearWindow();
		Edge *iter = polygon->edges;
		do {
			PrintLine(iter);
			iter = iter->next;
		} while (iter != polygon->Start());
		for (int i = 0; i < splitterSource.size(); i++) {
			Edge *temp = new Edge(splitterSource[i], new Polygon());
			temp->next = splitterTarget[i];
			PrintLine(temp);
		}
		window->SwapWindow();
	}

	while (true) {};
    return 0;
}

