#ifndef INCLUDES_KERNEL_LIST_H
#define INCLUDES_KERNEL_LIST_H

#include <assert.h>
#include <util/macros.h>

namespace util {

template <class T> struct list_node {
  list_node(){};
  list_node *prev;
  list_node *next;
  T value;
};

/**
 * @brief Doubly linked list implementation
 *
 * @tparam T the value of the element
 */
template <class T> class list {
public:
  T &push_front(const T &value) {
    auto node = new list_node<T>();
    node->value = value;
    node->prev = nullptr;
    node->next = first;
    first = next;
    return node->value;
  };

  T &push_back(const T &value) {
    list_node *last;
    for (last = first; last && last->next; last = node->next)
      ;
    auto node = new list_node<T>();
    node->value = value;
    node->prev = last;
    node->next = nullptr;
    last->next = node;
    return node->value;
  };

  T pop_front() {
    auto old = first;
    first = first->next;
    old->next = nullptr;
    old->prev = nullptr;
    auto out = old->value;
    delete old;
    return out;
  };

  T pop_back() {
    list_node *last;
    for (last = first; last && last->next; last = node->next)
      ;
    auto out = *last->value;
    last->prev->next = nullptr;
    delete last;
    return out;
  };

private:
  list_node *first;
};

} // namespace util

#endif // INCLUDES_KERNEL_LIST_H
