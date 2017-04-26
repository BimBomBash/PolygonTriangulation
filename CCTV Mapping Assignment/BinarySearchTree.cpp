#include "stdafx.h"
#include "Polygon.h"
#include "BinarySearchTree.h"

BinaryTree::BinaryTree()
{
	root = nullptr;	
}

BinaryTree::~BinaryTree()
{
	destroy_tree();
}

void BinaryTree::insert(Edge * edge)
{
	if (root != nullptr) {
		insert(edge->origin->x, edge, root);
	}
	else {
		root = new Node;
		root->edgeXPos = edge->origin->x;
		root->edge = edge;
		root->helper = edge->origin;
		root->left = nullptr;
		root->right = nullptr;
	}
}

Node * BinaryTree::search(Edge * edge)
{
	return search(edge->origin->x, edge, root);
}

void BinaryTree::destroy_tree()
{
	destroy_tree(root);
}

void BinaryTree::destroy_tree(Node * leaf)
{
	if (leaf != nullptr) {
		destroy_tree(leaf->left);
		destroy_tree(leaf->right);
		delete leaf;
	}
}

void BinaryTree::insert(float key, Edge *edge, Node *leaf)
{
	if (key < leaf->edgeXPos) {
		if (leaf->left != nullptr) insert(key, edge,leaf->left);
		else {
			leaf->left = new Node;
			leaf->left->edgeXPos = key;
			leaf->left->edge = edge;
			leaf->left->helper = edge->origin;
			leaf->left->left = nullptr;
			leaf->left->right = nullptr;
		}
	} else if (key >= leaf->edgeXPos) {
		if (leaf->right != nullptr) insert(key, edge, leaf->right);
		else {
			leaf->right = new Node;
			leaf->right->edgeXPos = key;
			leaf->right->edge = edge;
			leaf->right->helper = edge->origin;
			leaf->right->left = nullptr;
			leaf->right->right = nullptr;
		}
	}
}

Node * BinaryTree::search(float key, Edge *edge, Node *leaf)
{
	if (leaf != nullptr) {
		if (key == leaf->edgeXPos) {
			return leaf;
		}
		if (key < leaf->edgeXPos) {
			return search(key, edge, leaf->left);
		}
		else {
			return search(key, edge, leaf->right);
		}
	}
	else return nullptr;
}

