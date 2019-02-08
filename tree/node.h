#ifndef NODE_H
#define NODE_H

#include <utility>

template<typename K, typename V>
struct Node {
public:
    Node() : left{nullptr}, right{nullptr} { }

    Node(const K &key, const V &value) :
        left{nullptr}, right{nullptr}, _kv_pair(std::make_pair(key, value)) { }

    const K& key() const {
        return _kv_pair.first;
    }

    const V& value() const {
        return _kv_pair.second;
    }

    Node<K, V> *left, *right;

private:
    std::pair<K, V> _kv_pair;
};

#endif