#ifndef INCLUDES_UTIL_LIST_H
#define INCLUDES_UTIL_LIST_H

#include <assert.h>
#include <stdint.h>
#include <util/macros.h>

namespace util {

template <class T> struct list_node {
  list_node() : prev(nullptr), next(nullptr){};
  list_node *prev;
  list_node *next;
  T value;
};

template <class T> struct list_iterator {
  list_node<T> *p;
  list_node<T> &operator*() { return *p; }
  bool operator!=(const list_iterator<T> &rhs) { return p != rhs.p; }
  void operator++() { p = p->next; }
};

/**
 * @brief Doubly linked list implementation
 *
 * @tparam T the value of the element
 */
template <class T> class list {
public:
  list() : first(nullptr){};
  // fb78
  ~list() {
    for (auto node = first; node;) {
      auto next = node = node->next;
      delete node;
      node = next;
    }
  };

  /**
   * @brief Get an element by its index
   */
  T &at(size_t idx) const {
    auto node = first;
    for (size_t i = 0; i < idx; ++i) {
      node = node->next;
    }
    return node->value;
  };

  /**
   * @brief Push an element to the front of the linked list
   */
  T &push_front(const T &value) {
    auto node = new list_node<T>();
    node->value = value;
    if (!first)
      return (first = node)->value;
    node->prev = nullptr;
    node->next = first;
    first = node;
    return node->value;
  };

  /**
   * @brief Push an element to the end of the linked list
   */
  T &push_back(const T &value) {
    auto node = new list_node<T>();
    node->value = value;
    if (!first)
      return (first = node)->value;
    list_node<T> *last = first;
    while (last->next)
      last = last->next;
    node->prev = last;
    node->next = nullptr;
    last->next = node;
    return node->value;
  };

  /**
   * @brief Pop an element from the front of the list
   */
  T pop_front() {
    auto old = first;
    first = first->next;
    old->next = nullptr;
    old->prev = nullptr;
    auto out = old->value;
    delete old;
    return out;
  };

  /**
   * @brief Pop an element from the back of the list
   */
  T pop_back() {
    list_node<T> *last = first;
    while (last->next)
      last = last->next;
    auto out = last->value;
    last->prev->next = nullptr;
    delete last;
    return out;
  };

  // auto return requires C++14
  list_iterator<T> begin() const { return list_iterator<T>{first}; }

  list_iterator<T> end() const {
    list_node<T> *last = first;
    while (last->next)
      last = last->next;
    return list_iterator<T>{last};
  }

private:
  list_node<T> *first;
};

} // namespace util

#endif // INCLUDES_UTIL_LIST_H
