#include "hashNode.h"

HashNode::HashNode(const KEY &_key, const VALUE &_value) : key(_key), value(_value), next(NULL) {};

KEY HashNode::getKey() const {
  return key;
};
VALUE HashNode::getValue() const {
  return value;
};
HashNode HashNode::getNext() const {
  return next;
};

void HashNode::setValue(VALUE val) {
  value = val;
};
void HashNode::setNext(HashNode *nxt) {
  next = nxt;
};
