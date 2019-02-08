#include "btree.h"

template<typename K, typename V>
BTree::BTree() _root(nullptr) { }
BTree::BTree(Node<K, V>* node) _root(node) { }

Node<K, V>* BTree::insert(Node<K, V>* node) {
	if (_root == nullptr) {
		_root = node;
		return true;
	}
	return insert(node, _root);
}

Node<K, V>* BTree::insert(Node<K, V>* node, Node<K, V>* cur) {
	if (cur == nullptr) {
		return node;
	}

	if (node->key() > cur->key()) {
		cur->right = insert(node, cur->right);
	} else if (node->key() < cur->key()) {
		cur->left = insert(node, cur->left);
	}
}

V* BTree::find(K* key) {
	return find(K* key, Node<K, V>* cur);
}

V* BTree::find(K* key, Node<K, V>* cur) {
	if (cur == nullptr)
		return nullptr;
	if (key == cur->key())
		return cur->value();
	if (key > cur->key())
		return find(key, cur->right);
	else
		return find(key, cur->left);
}

void BTree::delete_tree(Node<K, V>* node) {
	if (node != nullptr) {
		remove(node->left);
		remove(node->right);
		delete node;
	}
}

BTree::~BTree() {
	delete_tree(_root);
}