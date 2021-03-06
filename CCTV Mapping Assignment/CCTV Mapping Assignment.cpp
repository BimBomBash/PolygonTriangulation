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
#include <stack>

const double PI = 3.1415926535897932384626433832795;
const float scale = 10.0f;
const float rotation = 45.0f;
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

void InsertPolygon(Polygon *_polygon) {
	for (int i = 0; i < monotones.size(); i++) {
		if (monotones[i] == _polygon) return;
	}
	monotones.push_back(_polygon);
}

void PolygonSplitter(Polygon * oldPolygon, Vertex *_source, Edge *_target) {
	Polygon *newPolygon = new Polygon();
	Edge *cache = _source->incidentEdge;

	Edge *sourceEdge = new Edge(_source, oldPolygon);
	Edge *sourceTwin = new Edge(_target->origin, newPolygon);

	sourceEdge->next = _target;
	sourceTwin->next = _source->incidentEdge;
	sourceEdge->prev = _source->incidentEdge->prev;
	sourceTwin->prev = _target->prev;
	
	sourceEdge->next->prev = sourceEdge;
	sourceEdge->prev->next = sourceEdge;
	sourceTwin->next->prev = sourceTwin;
	sourceTwin->prev->next = sourceTwin;

	_source->incidentEdge = sourceEdge;
	sourceTwin->origin->incidentEdge = sourceTwin;
	
	
	if (sourceEdge->next->key > sourceEdge->prev->key) {
		oldPolygon->edges = sourceEdge;
		newPolygon->edges = sourceTwin;
	}
	else {
		oldPolygon->edges = sourceTwin;
		newPolygon->edges = sourceEdge;
	}

	sourceEdge->twinEdge = sourceTwin;
	sourceTwin->twinEdge = sourceEdge;
	InsertPolygon(newPolygon);
}

void SplitPolygon(Polygon *oldPolygon) {
	int i = 0;
	for (int i = 0; i < splitterSource.size(); i++) {
		PolygonSplitter(oldPolygon,splitterSource[i], splitterTarget[i]);
	}
	InsertPolygon(oldPolygon);
	splitterSource.clear();
	splitterTarget.clear();
}

bool DifferentChain(Edge * a, Edge *b) {
	if (a->next->origin->y > a->origin->y && b->next->origin->y < b->origin->y)return true;
	else if (a->next->origin->y < a->origin->y && b->next->origin->y > b->origin->y) return true;
	else 
		return false;
}

bool DiagonalIsInsidePolygon(Edge * a, Edge *b) {
	float v1[2]{ a->next->origin->x - a->origin->x, a->next->origin->y - a->origin->y };
	float v2[2]{ a->prev->origin->x - a->origin->x, a->prev->origin->y - a->origin->y };
	float v3[2]{ b->origin->x - a->origin->x, b->origin->y - a->origin->y };
	float crossProduct[3]{ v1[1] * v2[2] - v1[2] * v2[1], v1[1] * v3[2] - v1[2] * v3[1], v3[1] * v2[2] - v3[2] * v2[1] };

	int trueCheck = 0;
	for (int i = 0; i < 3; i++) {
		if (crossProduct[i] >= 0) trueCheck++;
	}
	if (trueCheck == 3 || trueCheck == 0) {
		std::cout <<std::endl<< "THIS DIAGONAL IS INSIDE POLYGON:" << std::endl;
		a->Print();
		b->Print();
		std::cout << "==================================" << std::endl;
	}
	if (trueCheck == 3 || trueCheck == 0) return true;
	else return false;
}

std::vector<Edge *> checkEdges;
std::vector<Edge*> tempEdges;
void Triangulate(Polygon * _polygon) {
	Edge *iter = _polygon->edges;
	tempEdges.clear();
	do {
		if (tempEdges.empty())tempEdges.push_back(iter);
		else {
			for (int n = 0; n < tempEdges.size(); n++) {
				if (iter->origin->y >= tempEdges[n]->origin->y) { 
					tempEdges.insert(tempEdges.begin() + n, iter);
					break; 
				}
				else if (n == tempEdges.size() - 1 && iter->origin->y < tempEdges[n]->origin->y) tempEdges.push_back(iter);
			}
		}
		iter = iter->next;
	} while (iter != _polygon->Start());

	if (tempEdges.size() > 3) {
		std::stack<Edge *>triangulationEdges;
		triangulationEdges.push(tempEdges[0]);
		triangulationEdges.push(tempEdges[1]);

		for (int i = 2; i < tempEdges.size() - 1; i++){
			if (DifferentChain(tempEdges[i], triangulationEdges.top())) {
				while (triangulationEdges.size() > 1) {
					//InsertSplitter(tempVertex[i], triangulationVertex.top()->incidentEdge);
					Edge *temp = new Edge (tempEdges[i]->origin, nullptr);
					temp->next = triangulationEdges.top();
					checkEdges.push_back(temp);
					triangulationEdges.pop();
				}
				triangulationEdges.pop();
				if (triangulationEdges.empty())std::cout << "ABIS";
				triangulationEdges.push(tempEdges[i-1]);
				triangulationEdges.push(tempEdges[i]);
			}
			else {
				//HOW THE FUCK DO I KNOW THAT THE DIAGONAL IS INSIDE OF POLYGON!?
				Edge * lastPopped = triangulationEdges.top();
				triangulationEdges.pop();
				//triangulationEdges.top()->Print();
				while (!triangulationEdges.empty() && DiagonalIsInsidePolygon(tempEdges[i], triangulationEdges.top())) {
					std::cout << "SPLITTED";
					lastPopped = triangulationEdges.top();
					//InsertSplitter(tempVertex[i], triangulationVertex.top()->incidentEdge);
					Edge *temp = new Edge(tempEdges[i]->origin, nullptr);
					temp->next = triangulationEdges.top();
					checkEdges.push_back(temp);
					triangulationEdges.pop();
				}
				if (lastPopped!=nullptr)triangulationEdges.push(lastPopped);
				triangulationEdges.push(tempEdges[i]);
			}
		}

		triangulationEdges.pop();
		while (triangulationEdges.size() > 1) {
			InsertSplitter(tempEdges[tempEdges.size()-1]->origin, triangulationEdges.top());
			triangulationEdges.pop();
		}

		//SplitPolygon(_polygon);
	}
}

int main(int argc, char **argv)
{
	window = new Window("CCTV", 900, 600);

	InitPolygon();
	if (polygon->edges == nullptr)printf("NULLPTR");
	RotatePolygon();

	SetVertexType();
	InitVerticesQueue();
	MakeMonotone();
	SplitPolygon(polygon);

	int monotonesNumber = monotones.size();
	for (int i = 0; i < monotones.size();i++) Triangulate(monotones[i]);

	if (window->initiated) {
		window->ClearWindow();
		for (int i = 0; i < monotones.size(); i++) {
			Edge *iter = monotones[i]->edges;
			do {
				PrintLine(iter);
				iter = iter->next;
			} while (iter != monotones[i]->Start());
		}

		for (int j = 0; j < checkEdges.size(); j++)PrintLine(checkEdges[j]);
	}
	window->SwapWindow();

	while (true) {};
    return 0;
}

