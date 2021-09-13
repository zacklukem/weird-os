#ifndef INCLUDES_KERNEL_LLIST_H
#define INCLUDES_KERNEL_LLIST_H

#include <assert.h>
#include <macros.h>

/**
 * @brief A basic linked list
 */
struct llist_head {
  struct llist_head *next;
};

/**
 * @brief Get the nth head after a list head
 *
 * @param head
 * @param n
 * @return struct llist_head*
 */
static inline struct llist_head *llist_nth(struct llist_head *head, size_t n) {
  struct llist_head *current = head;
  for (size_t i = 0; i < n; ++i) {
    current = current->next;
    assert(current && "Illegal list access");
  }
  return current;
}

/**
 * @brief Iterate over a list
 *
 * ```
 * list_for_each(list_of_dogs, dog_list_head) {
 * // do something
 * }
 * ```
 */
#define llist_for_each(head, head_name)                                        \
  for (struct llist_head *head_name = head; head_name;                         \
       head_name = head_name->next)

/**
 * @brief Insert a new list element after the given head
 *
 * @param head the head of the element to insert after
 * @param source the element to insert after the given head
 */
static inline void llist_insert(struct llist_head *head,
                                struct llist_head *source) {
  source->next = head->next;
  head->next = source;
}

/**
 * @brief Get the structure containing the given entry
 */
#define llist_entry(ptr, type, member) container_of(ptr, type, member)

#endif // INCLUDES_KERNEL_LLIST_H
