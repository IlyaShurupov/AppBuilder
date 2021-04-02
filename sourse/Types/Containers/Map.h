
#pragma once

#include "Macros.h"

#include <cstddef>

template <typename K, int size>
struct StrHashPolicy {

    uint8 operator()(const K &key) const {

        unsigned long hash = 5381;
        int c;
        while ((c = *key.str++))
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        return hash % size;

    }
};

template <typename K, typename V, size_t tableSize, typename F >
class HashMap;

template <typename Type, int Size = 10 >
using Dict = HashMap<Str, Type*, Size, StrHashPolicy< Str, Size > >;


template <typename K, typename V>
class HashNode {

    K key;
    V value;
    
    HashNode *next;

public:

    HashNode(const HashNode &) {}
    HashNode & operator=(const HashNode &) { return *this; }

    HashNode(const K &key, const V &value) {
        this->key = key;
        this->value = value;
    }

};


template <typename K, typename V, size_t tableSize, typename Func >
class HashMap {

    HashNode<K, V> *table[tableSize];
    Func hashFunc;

public:

    HashMap(const HashMap & other) { }
    HashMap & operator=(const HashMap & other) { return *this; }

    HashMap() {}

    void Put(const K &key, const V &value) {}

    void Remove(const K &key) {}

    bool Get(const K &key, V *value) { return false; }

    ~HashMap() {}
};