#ifndef KERNEL_UTIL_MACROS_H
#define KERNEL_UTIL_MACROS_H

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
#define container_of(ptr, type, member)                                        \
  ({                                                                           \
    const get_member_type(type, member) *__member_ptr = (ptr);                 \
    (type *)((char *)__member_ptr - offsetof(type, member));                   \
  })

#endif // KERNEL_UTIL_MACROS_H
