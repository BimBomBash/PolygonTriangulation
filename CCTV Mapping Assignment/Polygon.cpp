#include "stdafx.h"
#include <iostream>
#include <list>
#include "Polygon.h"


Polygon::Polygon(std::list<Vertex *>&_vertices)
{
	vertices = _vertices;
	Vertex* prev = vertices.back();
	for (auto iter = vertices.begin(); iter != vertices.end(); ++iter) {
		lines.push_back(new Line(prev, *iter));
		prev = *iter;
	}
}

Polygon::~Polygon()
{
}

void Polygon::Print()
{
	for (auto iter = vertices.begin(); iter != vertices.end(); ++iter) {
		(*iter)->Print();
	}
	for (auto iter = lines.begin(); iter != lines.end(); ++iter) {
		(*iter)->Print();
	}
}

Line::Line(Vertex *_a, Vertex *_b)
{
	a = _a;
	b = _b;
	a->next = b;
	b->prev = a;
}

Line::Line(float x1, float y1, float x2, float y2)
{
	a = new Vertex(x1, y1);
	b = new Vertex(x2, y2);

	a->next = b;
	b->prev = a;
}

void Line::Print()
{
	std::cout << "(" << a->x << ", " << a->y << "), (" << b->x << ", " << b->y << ")" << std::endl;                                                                                                                                                            
}

Vertex::Vertex(float _x, float _y)
{
	x = _x;
	y = _y;
}

void Vertex::Print()
{
	std::cout << "(" << x << ", " << y << ")"<<std::endl;
}
