
#pragma once

#include "Macros.h"
#include "List.h"
#include "Tuple.h"

template <typename K>
struct StrHashPolicy {

    uint8 operator()(const K &key, uint8 size) const {
        char* string = key.str;

        unsigned long hash = 5381;
        int c;
        while ((c = *string++)) {
            hash = ((hash << 5) + hash) + c;
        }
        return hash % size;

    }
};

template <typename K, typename V, size_t tableSize, typename F >
class HashTable;

template <typename Type, int Size = 10 >
using Dict = HashTable<Str, Type*, Size, StrHashPolicy< Str > >;


template <typename K, typename V>
struct HashNode {

    K key;
    V val;

    HashNode<K, V>* next = nullptr;
    
    HashNode(const HashNode& node) {}
    HashNode& operator=(const HashNode& in) { return *this; }

    HashNode(const K &key, const V &val) {
        this->key = key;
        this->val = val;
    }
};


template <typename K, typename V, size_t tableSize, typename Func >
class HashTable {

    HashNode<K, V>* table[tableSize];
    Func hash;
    uint8 size = tableSize;

public:

    HashTable() {}
    HashTable(const HashTable& in) { operator=(in); }

    void Put(const K &key, const V &val) {
        uint8 idx = hash(key, size);
        
        if (!table[idx]) {
            table[idx] = new HashNode<K, V>(key, val);

        } else if (table[idx]->key == key) {
            table[idx]->val = val;

        } else {
            HashNode<K, V>* node = new HashNode<K, V>(key, val);
            node->next = table[idx]->next;
            table[idx]->next = node; 
        }
    }

    void Remove(const K &key) {
        uint8 idx = hash(key, size);
        HashNode<K, V>* prev = nullptr;
        for (HashNode<K, V>* node = table[idx]; node; node = node->next) {
            if (node->key == key) {

                if (prev) {
                    prev->next = node->next;

                } else {
                    table[idx] = node->next;
                }
                
                delete node;
                break;
            }

            prev = node;
        }
    }

    bool Get(const K& key, V* value) { 
        for (HashNode<K, V>* node = table[hash(key, size)]; node; node = node->next) {
            if (node->key == key) {
                *value = node->val;
                return true;
            }
        }    

        return false;
    }

    void ToList(List<Tuple<K, V>>* list) {
        
        list->Clear();

        for (uint8 idx = 0; idx < size; idx++) {

            if (!table[idx]) {
                continue;
            }

            for (HashNode<K, V>* node = table[idx]; node; node = node->next) {
                list->PushBack(new Tuple<K, V>(node->key, node->val));
            }
            
        }
    }

    void Clear() {
        for (uint8 idx = 0; idx < size; idx++) {

            if (!table[idx]) {
                continue;
            }

            HashNode<K, V>* node = table[idx];
            HashNode<K, V>* del_node;
            while (node) {
                del_node = node;
                node = node->next;
                delete del_node;
            }
            
            table[idx] = nullptr;
        }
    }

    HashTable & operator=(const HashTable& in) { 

        Clear();

        for (uint8 idx = 0; idx < in.size; idx++) {

            if (!in.table[idx]) {
                continue;
            }

            for (HashNode<K, V>* node = in.table[idx]; node; node = node->next) {
                Put(node->key, node->val);
            }
            
        }
        return *this; 
    }

    ~HashTable() {
        for (uint8 idx = 0; idx < size; idx++) {

            HashNode<K, V>* next = nullptr;
            HashNode<K, V>* node = table[idx];
            
            while (node) {
                next = node->next;
                delete node;
                node = next;
            }
        }
          
    }
};