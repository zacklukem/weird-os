/**
 * @file vector.h
 * @author Zachary Mayhew (mayhew.zachary2003@gmail.com)
 * @brief Includes structures and methods to allow for easy dynamic sized arrays
 * @version 0.1
 * @date 2021-09-12
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef INCLUDES_KERNEL_VECTOR_H
#define INCLUDES_KERNEL_VECTOR_H

#ifndef VECTOR_DEFAULT_INITIAL_SIZE
#define VECTOR_DEFAULT_INITIAL_CAP 10
#endif

#include <assert.h>
#include <stdint.h>

/**
 * @brief
 *
 */
struct vector {
  void *_data;
  /**
   * raw cap in bytes
   */
  size_t _capacity;
  /**
   * raw size in bytes
   */
  size_t _size;
  /**
   * Element size
   */
  size_t _element_size;
};

struct vector __vector_create(size_t element_size);
void *__vector_alloc_push(struct vector *vec, size_t element_size);
void *__vector_pop(struct vector *vec, size_t element_size);

/**
 * @brief Reserve n bytes for the vector
 *
 * @param vec
 * @return void
 */
void vector_reserve(struct vector *vec, size_t size);

static inline void *__vector_at(struct vector *vec, size_t index,
                                size_t element_size) {
  assert(element_size == vec->_element_size &&
         "Invalid element type for vector");
  assert(vec->_size > index && "Index out of bounds");
  return vec->_data + index * element_size;
}

/**
 * @brief Get the number of elements in a vector
 *
 * @param vec the vector
 * @return size_t the number of elements
 */
static inline size_t vector_size(struct vector *vec) {
  return vec->_size / vec->_element_size;
}

/**
 * @brief Get the capacity of a vector in number of elemnts
 *
 * @param vec the vector
 * @return size_t the number of elements
 */
static inline size_t vector_capacity(struct vector *vec) {
  return vec->_capacity / vec->_element_size;
}

/**
 * @brief Create a new vector with type
 *
 * @param ty the type of the vector
 * @return struct vector
 */
#define vector_create(ty) (__vector_create(sizeof(ty)))

/**
 * @brief Get an element with type ty at the given index.
 *
 * Evaluates to a lvalue or * rvalue, so element can be set:
 * ```
 * vector_at(int, my_vector, 3) = 32; // OK
 * int a = vector_at(int, my_vector, 2); // OK
 * ```
 *
 * @param ty the element type in the vector
 * @param vector the vector to index
 * @param index the index of the element
 */
#define vector_at(ty, vector, index)                                           \
  (*((ty *)__vector_at(vector, index, sizeof(ty))))

/**
 * @brief Push a value to a vector with given type
 *
 */
#define vector_push(ty, vector, data)                                          \
  (*((ty *)__vector_alloc_push(vector, sizeof(ty))) = data)

/**
 * @brief Pop a value off a vector and return it
 *
 * @return ty value
 */
#define vector_pop(ty, vector) (*((ty *)__vector_pop(vector, sizeof(ty))));

#endif // INCLUDES_KERNEL_VECTOR_H
