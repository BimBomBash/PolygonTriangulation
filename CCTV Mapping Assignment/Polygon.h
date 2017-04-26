#pragma once
struct Edge;
struct Polygon;

enum VertexType {START, END, SPLIT, MERGE, REGULAR};

struct Vertex {
	Vertex(float _x, float _y, Edge *_incidentEdge);
	float x, y;
	Edge *incidentEdge;
	void Print();
	VertexType type;
	int key;
};

struct Edge {
	Vertex *origin;
	Edge *twinEdge;
	Edge *next;
	Edge *prev;
	Polygon *incidentFace;
	Edge(float xOrigin, float yOrigin, Polygon *_incidentFace);
	void Print();
	int key;
};

class Polygon
{
	int length=0;
public:
	Edge* edges;
	int Length();

	Edge *Start();
	Edge *End();

	void Push_Back(Edge *edge);
	Polygon(Edge *_edges);
	~Polygon();
	void Print(); 
};

struct OrderByVerticesY
{
	bool operator() (Vertex* const &a, Vertex* const &b) { 
		if (a->y != b->y) return a->y < b->y;
		return a->x > b->x;
	}
};


