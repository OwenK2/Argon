#ifndef HASH_NODE
#define HASH_NODE

template <typename K, typename V>
class HashNode {
  private:
    //key <-> value
    K key;
    V value;

    //collisions handled with linked list
    HashNode *next;

  public:
    HashNode(const K &_key, const V &_value);

    K getKey() const;
    V getValue() const;
    HashNode<K, V> getNext() const;

    void setValue(V value);
    void setNext(HashNode<K, V> *next);
};

#endif
