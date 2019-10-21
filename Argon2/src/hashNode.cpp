#include "hashNode.h"

HashNode::HashNode(const K &_key, const V &_value) : key(_key), value(_value), next(NULL) {};

K HashNode::getKey() const {
  return key;
};
V HashNode::getValue() const {
  return value;
};
HashNode<K, T> HashNode::getNext() const {
  return next;
};

void HashNode::setValue(V val) {
  value = val;
};
void HashNode::setNext(HashNode<K, T> *nxt) {
  next = nxt;
};
