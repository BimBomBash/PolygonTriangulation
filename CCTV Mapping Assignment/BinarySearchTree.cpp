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
		if (key == leaf->edgeXPos || edge == leaf->edge) {
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

void BinaryTree::Delete(Node *leaf)
{
	//delete no child, one child, two child
	if (leaf->left == nullptr && leaf->right == nullptr) {
		delete leaf;
	}
	/*if (leaf->left->left != nullptr && leaf->left->right != nullptr) {
		Node *temp = traverseMinimum(leaf->left);
		temp->left = leaf->left->left;
		temp->right = leaf->left->right;
		leaf->left = temp;
	}*/
	else {
		if (leaf->left->left != nullptr) leaf->left = leaf->left->left;
		else leaf->left = leaf->left->right;
	}
}

Node * traverseMinimum(Node *node) {
	Node *temp = node;
	while (temp->right->right != nullptr) {
		temp = temp->right;
	}

	if (temp->left != nullptr) { 
		if (temp->left < temp->right && temp->left->left == nullptr && temp->left->right == nullptr);
		return temp; }
	else {
		return temp;
	}

}