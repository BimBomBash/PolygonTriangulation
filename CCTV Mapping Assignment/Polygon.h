#pragma once
struct Vertex {
	Vertex(float _x, float _y);
	float x, y;
	void Print();
	Vertex *prev;
	Vertex *next;
};

struct Line {
	Vertex *a, *b;
	Line(Vertex *_a, Vertex *_b);
	Line(float x1, float y1, float x2, float y2);
	void Print();
};

class Polygon
{
public:
	std::list <Vertex*> vertices;
	std::list <Line*> lines;
	Polygon(std::list<Vertex *>&_vertices);
	~Polygon();
	void Print();
};

