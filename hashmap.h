#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename KeyT, typename ValT>
class HashMap {
 private:
  struct ChainNode {
    const KeyT key;
    ValT value;
    ChainNode* next;

    ChainNode(KeyT key, ValT value) : key(key), value(value), next(nullptr) {
    }

    ChainNode(KeyT key, ValT value, ChainNode* next)
        : key(key), value(value), next(next) {
    }
  };

  ChainNode** data;
  size_t sz;
  size_t capacity;

  // Utility members for begin/next
  ChainNode* curr;
  size_t curr_idx;

 public:
  /**
   * Creates an empty `HashMap` with 10 buckets.
   */
  HashMap() {
    // TODO_STUDENT
    capacity = 10;
    sz = 0;
    data = new ChainNode*[capacity];

    for (size_t i = 0; i < capacity; i++) {
      data[i] = nullptr;
    }
    curr = nullptr;
    curr_idx = 0;
  }

  /**
   * Creates an empty `HashMap` with `capacity` buckets.
   */
  HashMap(size_t capacity) {
    // TODO_STUDENT
    this->capacity = capacity;
    sz = 0;
    data = new ChainNode*[capacity];
    for (size_t i = 0; i < capacity; i++) {
      data[i] = nullptr;
    }
    curr = nullptr;
    curr_idx = 0;
  }

  /**
   * Checks if the `HashMap` is empty. Runs in O(1).
   */
  bool empty() const {
    // TODO_STUDENT
    if (sz == 0) {
      return true;
    } else {
      return false;
    }
  }

  /**
   * Returns the number of mappings in the `HashMap`. Runs in O(1).
   */
  size_t size() const {
    // TODO_STUDENT
    return sz;
  }

  /**
   * Adds the mapping `{key -> value}` to the `HashMap`. If the key already
   * exists, does not update the mapping (like the C++ STL map).
   *
   * Uses the hash value of the key to determine the location in the
   * underlying hash table. Creates exactly one new node; resizes by doubling
   * when the load factor exceeds 1.5.
   *
   * On resize, doesn't create new nodes, but rearranges existing ones.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  void insert(KeyT key, ValT value) {
    // TODO_STUDENT
    size_t index = hash<KeyT>{}(key) % capacity;
    ChainNode* current = data[index];
    // Have to check whether key is already in bucket first then we can insert
    // if not
    while (current != nullptr) {
      if (current->key == key) {
        return;
      }
      current = current->next;
    }

    // add the new node to the front of the chain
    ChainNode* new_node = new ChainNode(key, value, data[index]);
    data[index] = new_node;
    sz++;
    // if the load factor gets too high, resize and rehash
    double load_factor = static_cast<double>(sz) / capacity;
    if (load_factor > 1.5) {
      size_t old_capacity = capacity;
      ChainNode** old_data = data;

      capacity *= 2;
      data = new ChainNode*[capacity];
      for (size_t i = 0; i < capacity; i++) {
        data[i] = nullptr;
      }

      // need to rehash and so move existing nodes into new bucket array
      for (size_t i = 0; i < old_capacity; i++) {
        ChainNode* node = old_data[i];
        while (node != nullptr) {
          ChainNode* next_node = node->next;
          size_t new_index = hash<KeyT>{}(node->key) % capacity;
          node->next = data[new_index];
          data[new_index] = node;
          node = next_node;
        }
      }

      delete[] old_data;
    }
  }

  /**
   * Return a reference to the value stored for `key` in the map.
   *
   * If key is not present in the map, throw `out_of_range` exception.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT& at(const KeyT& key) const {
    // TODO_STUDENT
    size_t index = hash<KeyT>{}(key) % capacity;
    ChainNode* current = data[index];

    while (current != nullptr) {
      if (current->key == key) {
        return current->value;
      }
      current = current->next;
    }

    throw out_of_range("key not found");
  }

  /**
   * Returns `true` if the key is present in the map, and false otherwise.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  bool contains(const KeyT& key) const {
    // TODO_STUDENT
    size_t index = hash<KeyT>{}(key) % capacity;
    ChainNode* current = data[index];

    while (current != nullptr) {
      if (current->key == key) {
        return true;
      }
      current = current->next;
    }

    return false;
  }

  /**
   * Empties the `HashMap`, freeing all nodes. The bucket array may be left
   * alone.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  void clear() {
    // TODO_STUDENT
    for (size_t i = 0; i < capacity; i++) {
      ChainNode* current = data[i];

      while (current != nullptr) {
        ChainNode* next_node = current->next;
        delete current;
        current = next_node;
      }

      data[i] = nullptr;
    }

    sz = 0;
    curr = nullptr;
    curr_idx = 0;
  }

  /**
   * Destructor, cleans up the `HashMap`.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  ~HashMap() {
    // TODO_STUDENT
    clear();
    delete[] data;
  }

  /**
   * Removes the mapping for the given key from the `HashMap`, and returns the
   * value.
   *
   * Throws `out_of_range` if the key is not present in the map. Creates no new
   * nodes, and does not update the key or value of any existing nodes.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT erase(const KeyT& key) {
    // TODO_STUDENT
    size_t index = hash<KeyT>{}(key) % capacity;
    ChainNode* current = data[index];
    ChainNode* previous = nullptr;

    while (current != nullptr) {
      if (current->key == key) {
        ValT removed_value = current->value;

        // check the node before current is nullptr because that would mean
        // head of chain
        if (previous == nullptr) {
          data[index] = current->next;
        } else {
          previous->next =
              current->next;  // skip over the current node in the chain
        }

        delete current;
        sz--;
        return removed_value;
      }

      previous = current;
      current = current->next;
    }

    throw out_of_range("key not found");
  }

  /**
   * Copy constructor.
   *
   * Copies the mappings from the provided `HashMap`.
   *
   * Runs in O(N+B), where N is the number of mappings in `other`, and B is the
   * number of buckets.
   */
  HashMap(const HashMap& other) {
    // TODO_STUDENT
    capacity = other.capacity;
    sz = 0;
    data = new ChainNode*[capacity];

    for (size_t i = 0; i < capacity; i++) {
      data[i] = nullptr;
    }

    curr = nullptr;
    curr_idx = 0;

    for (size_t i = 0; i < other.capacity; i++) {
      ChainNode* current = other.data[i];
      while (current != nullptr) {
        insert(current->key, current->value);
        current = current->next;
      }
    }
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this table, and copies the mappings from the provided `HashMap`.
   *
   * Runs in O((N1+B1) + (N2+B2)), where N1 and B1 are the number of mappings
   * and buckets in `this`, and N2 and B2 are the number of mappings and buckets
   * in `other`.
   */
  HashMap& operator=(const HashMap& other) {
    if (this == &other) {  // protect against self-assignment
      return *this;
    }

    clear();
    delete[] data;

    capacity = other.capacity;
    sz = 0;
    data = new ChainNode*[capacity];

    for (size_t i = 0; i < capacity; i++) {
      data[i] = nullptr;
    }

    curr = nullptr;
    curr_idx = 0;
    // rebuild this map using other's bucket count and mappings
    for (size_t i = 0; i < other.capacity; i++) {
      ChainNode* current = other.data[i];
      while (current != nullptr) {
        insert(current->key, current->value);
        current = current->next;
      }
    }

    return *this;
  }

  // =====================

  /**
   *
   * Checks if the contents of `this` and `other` are equivalent.
   *
   * Two `HashMap` objects are equivalent if they contain the same
   * mappings from key to value. To be equivalent, the two `HashMap`
   * objects need not have the elements saved in the same order
   * inside of the buckets.
   *
   * Runs in worst-case O(B*L*L) time, where B is the maximum number
   * of buckets in either of the `HashMap` objects and L is the length
   * of the  largest chain on any of the buckets.
   *
   */
  bool operator==(const HashMap& other) const {
    // every key in this map must exist in other with the same value
    // TODO_STUDENT
    if (sz != other.sz) {
      return false;
    }

    for (size_t i = 0; i < capacity; i++) {
      ChainNode* current = data[i];

      while (current != nullptr) {
        if (!other.contains(current->key)) {
          return false;
        }

        if (other.at(current->key) != current->value) {
          return false;
        }

        current = current->next;
      }
    }

    return true;
  }

  /**
   * Resets internal state for an iterative traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr` and
   * `curr_idx`.
   *
   * Runs in worst-case O(B), where B is the number of buckets.
   */
  void begin() {
    // TODO_STUDENT
    // have to find the first non-empty bucket and start there
    curr = nullptr;
    curr_idx = 0;

    for (size_t i = 0; i < capacity; i++) {
      if (data[i] != nullptr) {
        curr = data[i];
        curr_idx = i;
        return;
      }
    }
  }

  /**
   * Uses the internal state to return the "next" key and value
   * by reference, and advances the internal state. Returns `true` if the
   * reference parameters were set, and `false` otherwise.
   *
   * Example usage:
   *
   * ```c++
   * HashMap<string, int> hm;
   * hm.begin();
   * string key;
   * int value;
   * while (hm.next(key, val)) {
   *   cout << key << ": " << val << endl;
   * }
   * ```
   *
   * Does not visit the mappings in any defined order.
   *
   * Modifies nothing except for `curr` and `curr_idx`.
   *
   * Runs in worst-case O(B) where B is the number of buckets.
   */
  bool next(KeyT& key, ValT& value) {
    // TODO_STUDENT
    if (curr == nullptr) {
      return false;
    }

    key = curr->key;
    value = curr->value;

    // before anything happens try moving forward in the current chain
    if (curr->next != nullptr) {
      curr = curr->next;
      return true;
    }

    // if the chain ended, move to the next non-empty bucket
    for (size_t i = curr_idx + 1; i < capacity; i++) {
      if (data[i] != nullptr) {
        curr = data[i];
        curr_idx = i;
        return true;
      }
    }

    curr = nullptr;
    curr_idx = capacity;
    return true;
  }

  // ===============================================

  /**
   * Returns a pointer to the underlying memory managed by the `HashMap`.
   * For autograder testing purposes only.
   */
  void* get_data() const {
    return this->data;
  }

  /**
   * Returns the capacity of the underlying memory managed by the `HashMap`. For
   * autograder testing purposes only.
   */
  size_t get_capacity() {
    return this->capacity;
  }
};
