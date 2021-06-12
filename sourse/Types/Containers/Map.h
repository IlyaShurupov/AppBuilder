
#pragma once

#include "Macros.h"
#include "List.h"
#include "Strings/Strings.h"

#include "Memory/Mem.h"

#define HASHMAP_LOAD_FACTOR 2/3.f
#define HASHMAP_MIN_SIZE 4
#define HASHMAP_PERTURB_SHIFT 5
#define HASHMAP_DELETED_SLOT(table, idx) table[idx] == (HashNode<V, K>*)-1

template <typename Val>
struct CopyBytes {
	inline const Val& operator() (const Val& val) {
		return val;
	}
};

template <typename V, typename K>
struct HashNode {
	K key;
	V val;
};

template <typename V, typename K, typename Hashfunc, typename CopyValfunc = CopyBytes<V>, int table_size = HASHMAP_MIN_SIZE>
class HashMap {

public:

	HashNode<V, K>** table;
	int nslots;
	int nentries = 0;

	Hashfunc hash;
	CopyValfunc copy_val;

	void rehash() {
		int nslots_old = nslots;
		HashNode<V, K>** table_old = table;

		nslots = next_pow_of_2((1.f / (HASHMAP_LOAD_FACTOR)) * nentries + 1);
		table = new HashNode<V, K>*[nslots]();
		nentries = 0;

		for (int i = 0; i < nslots_old; i++) {
			if (!table_old[i] || (int)table_old[i] == -1) {
				continue;
			}

			int idx = find_slot(table_old[i]->key, false);
			table[idx] = table_old[i];
		}
		delete table_old;
	}

	int find_slot(const K& key, bool existing) {
		int hashed_key = hash(key);
		int mask = nslots - 1;
		int idx = hashed_key & mask;
		int shift = (hashed_key >> HASHMAP_PERTURB_SHIFT) & ~1;

	NEXT:

		if (HASHMAP_DELETED_SLOT(table, idx)) {
			if (!existing) {
				return idx;
			}
		}
		else if (!table[idx] && !existing) {
			return idx;
		}
		else if (table[idx]->key == key) {
			return idx;
		}

		idx = ((5 * idx) + 1 + shift) & mask;
		goto NEXT;

	}

	HashMap() {
		table = new HashNode<V, K>*[table_size]();
		nslots = next_pow_of_2(table_size - 1);
	}

	void Put(const K& key, const V& val) {
		int idx = find_slot(key, false);

		if (!table[idx] || HASHMAP_DELETED_SLOT(table, idx)) {
			table[idx] = new HashNode<V, K>;
			table[idx]->key = key;
			nentries++;
		}

		table[idx]->val = val;

		if ((float)nentries / nslots > HASHMAP_LOAD_FACTOR) {
			rehash();
		}
	}

	V& Get(const K& key) {
		int idx = find_slot(key, true);
		return table[idx]->val;
	}

	void Remove(const K& key) {
		int idx = find_slot(key, true);
		delete table[idx];
		table[idx] = (HashNode<V, K>*)-1;

		nentries--;
		if ((float)nentries / nslots < 1 - HASHMAP_LOAD_FACTOR) {
			rehash();
		}
	}

	void operator=(const HashMap<V, K, Hashfunc, CopyValfunc, table_size>& in) {
		clear();

		nslots = in.nslots;
		table = new HashNode<V, K>*[nslots]();

		for (int i = 0; i < nslots; i++) {
			if (in.table[i] && (int)in.table[i] != -1) {
				Put(in.table[i]->key, copy_val(in.table[i]->val));
			}
		}
	}

	void clear() {
		for (int i = 0; i < nslots; i++) {
			if (table[i] && (int)table[i] != -1) {
				delete table[i];
			}
		}
		delete table;
	}

	~HashMap() {
		clear();
	}
};

struct StrHashPolicy {
	inline int operator()(const Str& str) {
		return hash_string(str.str);
	}
};

template< typename Type, typename CopyValfunc = CopyBytes<Type>, int table_size = HASHMAP_MIN_SIZE >
using Dict = HashMap<Type, Str, StrHashPolicy, CopyValfunc, table_size>;