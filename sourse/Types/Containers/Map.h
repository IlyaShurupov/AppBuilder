
#pragma once

#include "Macros.h"
#include "List.h"
#include "Tuple.h"

#include "Memory/Mem.h"

template <typename K, typename V, size_t tableSize, typename HashPolicy, typename RemovePolicy, typename Copying>
class HashTable;

template <typename K, typename V>
struct HashNode;

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

template <typename V>
struct CopyVal {
    const V& operator()(const V& in) const {
        return in;
    }
};

template <typename V>
struct DelRemovePolicy {
    void operator()(V* val) {
        DEL(V, val);
    }
};

template <typename V>
struct ReleaseRemovePolicy {
    void operator()(V* val) {
    }
};

template <typename Type, int Size = 10 , typename Copying = CopyVal<Type*> >
using Dict = HashTable<Str, Type*, Size, StrHashPolicy< Str >, DelRemovePolicy<Type>, Copying >;


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


template <typename K, typename V, size_t tableSize, typename HashPolicy, typename RemovePolicy = ReleaseRemovePolicy<V>, typename Copying = CopyVal<V> >
class HashTable {

    HashNode<K, V>* table[tableSize];
    HashPolicy hash;
    RemovePolicy remove_val;
    Copying copy_val;
    uint8 size = tableSize;

    typedef HashNode<K, V> HSHNODE;

public:

    HashTable() : table() {}
    HashTable(const HashTable& in) : table() { operator=(in); }

    void Put(const K &key, const V &val) {
        uint8 idx = hash(key, size);
        
        if (!table[idx]) {
            table[idx] = NEW (HSHNODE)(key, val);

        } else if (table[idx]->key == key) {
            table[idx]->val = val;

        } else {
            HashNode<K, V>* node = NEW (HSHNODE)(key, val);
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
                
                remove_val(node->val);
                DEALLOC(node);
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

    V Get(const K& key) { 
        V val;
        bool suc = Get(key, &val);
        assert(suc);
        return val;
    }

    void ToList(List<Tuple<K*, V>>* list) {
        
        list->Clear();

        for (uint8 idx = 0; idx < size; idx++) {

            if (!table[idx]) {
                continue;
            }

            for (HashNode<K, V>* node = table[idx]; node; node = node->next) {
                typedef Tuple<K*, V> tpl;
                list->PushBack(NEW (tpl) (&node->key, node->val));
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
                remove_val(del_node->val);
                DEALLOC(del_node);
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
                const V copied_val = copy_val(node->val);
                Put(node->key, copied_val);
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
                remove_val(node->val);
                DEALLOC(node);
                node = next;
            }
        }
          
    }
};