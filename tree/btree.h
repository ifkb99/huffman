#ifndef B_TREE
#define B_TREE

/*
	Basic binary tree class
*/

#include "node.h"

template <typename K, typename V>
class BTree {
public:
	BTree(Node<K, V>*);
	~BTree();
	Node<K, V>* insert(Node<K, V>*);
	Node<K, V>* insert(Node<K, V>*, Node<K, V>*);
	const V* find(K*);
	const V* find(K* key, Node<K, V>* cur);
	void delete_tree();
	const size_t size();
	size_t leaves();
	const Node<K, V>* root();
private:
	Node<K, V>* _root;
	size_t _size;
};

#endif