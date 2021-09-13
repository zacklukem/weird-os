#ifndef INCLUDES_KERNEL_LIST_H
#define INCLUDES_KERNEL_LIST_H

#include <assert.h>
#include <macros.h>

/**
 * @brief A basic doubly linked list
 */
struct list_head {
  struct list_head *prev;
  struct list_head *next;
};

static inline void list_circular(struct list_head *head) {
  head->next = head;
  head->prev = head;
}

/**
 * @brief Remove an item from the linked list structure
 *
 * @param head
 * @return struct list_head*
 */
static inline struct list_head *list_remove(struct list_head *head) {
  head->prev->next = head->next;
  head->next->prev = head->prev;
}

/**
 * @brief Get the nth head after a list head
 *
 * @param head
 * @param n
 * @return struct list_head*
 */
static inline struct list_head *list_nth(struct list_head *head, size_t n) {
  struct list_head *current = head;
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
#define list_for_each(head, head_name)                                         \
  for (struct list_head *head_name = head; head_name;                          \
       head_name = head_name->next)

/**
 * @brief Insert a new list element after the given head
 *
 * @param head the head of the element to insert after
 * @param source the element to insert after the given head
 */
static inline void list_insert(struct list_head *head,
                               struct list_head *source) {
  struct list_head *after = head->next;
  head->next = source;
  source->prev = head;
  source->next = after;
  after->prev = source;
}

/**
 * @brief Get the offset in bytes of a member of a type
 */
#define offsetof(type, member) ((size_t) & ((type *)0)->member)

/**
 * @brief Get the type of a struct member
 *
 * ```
 * struct my_struct {
 *   uint32_t my_member;
 * }
 * get_member_type(type, member) // uint32_t
 */
#define get_member_type(type, member) typeof(((type *)0)->member)

/**
 * @brief Get the structure containing the given entry
 */
#define list_entry(ptr, type, member) container_of(ptr, type, member)

#endif // INCLUDES_KERNEL_LIST_H
