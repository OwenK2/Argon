#ifndef HASH_NODE
#define HASH_NODE

template <typename KEY, typename VALUE>
class HashNode {
  private:
    //key <-> value
    KEY key;
    VALUE value;

    //collisions handled with linked list
    HashNode *next;

  public:
    HashNode(const KEY &_key, const VALUE &_value);

    KEY getKey() const;
    VALUE getValue() const;
    HashNode getNext() const;

    void setValue(VALUE value);
    void setNext(HashNode *next);
};

#endif
