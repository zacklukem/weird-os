#ifndef INCLUDES_KERNEL_LIST_H
#define INCLUDES_KERNEL_LIST_H

#include <assert.h>
#include <util/macros.h>

namespace util {

/**
 * @brief A basic doubly linked list
 */
class list_node {
  list_node *prev;
  list_node *next;
};

template <class T> class list {
private:
  list_node *first;
};

} // namespace util

#endif // INCLUDES_KERNEL_LIST_H
