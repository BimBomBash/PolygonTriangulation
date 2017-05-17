#include "stdafx.h"
#include <iostream>
#include <list>
#include "Polygon.h"

int Polygon::Length()
{
	return length;
}

Edge * Polygon::Start()
{
	return edges;
}

Edge * Polygon::End()
{
	return edges->prev;
}

void Polygon::Push_Back(Edge * edge)
{
	Edge *iter = edges;
	if (iter == nullptr) {
		edges = edge;
		edges->prev = edges;
		edges->next = edges;
	}
	else {
		while (iter->next != edges) {
			iter = iter->next;
		}
		iter->next = edge;
		edge->prev = iter;
		edges->prev = edge;
		edges->prev->next = edges;
	}
	length++;
}

Polygon::Polygon(Edge * _edges)
{
	edges = _edges;
}

Polygon::~Polygon()
{
}

void Polygon::Print()
{
	Edge *iter = edges;
	do {
		iter->origin->Print();
		iter->Print();
		iter = iter->next;
	} while (iter != edges);
}

Edge::Edge(float xOrigin, float yOrigin, Polygon * _incidentFace)
{
	origin = new Vertex(xOrigin, yOrigin, this);
	incidentFace = _incidentFace;
}

Edge::Edge(Vertex * _origin, Polygon * _incidentFace)
{
	origin = _origin;
	incidentFace = _incidentFace;
}

void Edge::Print()
{
	std::cout << "(" << origin->x << ", " << origin->y << "), (" << next->origin->x << ", " << next->origin->y << ")" << std::endl;
}


Vertex::Vertex(float _x, float _y, Edge * _incidentEdge)
{
	x = _x;
	y = _y;
	incidentEdge = _incidentEdge;
}

void Vertex::Print()
{
	std::cout << "(" << x << ", " << y << ")"<<std::endl;
}

TriangulationNode::TriangulationNode()
{
}

TriangulationNode::TriangulationNode(Edge * _edge)
{
	edge = _edge;
	next = nullptr;
}
