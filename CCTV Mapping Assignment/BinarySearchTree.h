#pragma once
struct Node
{
	Edge *edge;
	Vertex *helper;
	float edgeXPos;
	Node *left;
	Node *right;
};

class BinaryTree
{
public:
	BinaryTree();
	~BinaryTree();

	void insert(Edge *edge);
	Node *search(Edge *edge);
	void destroy_tree();

private:
	void destroy_tree(Node *leaf);
	void insert(float key, Edge *edge, Node *leaf);
	Node *search(float key, Edge *edge, Node *leaf);

	Node *root;
};

