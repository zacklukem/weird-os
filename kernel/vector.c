#include <kernel/kmalloc.h>
#include <kernel/vector.h>

struct vector __vector_create(size_t element_size) {
  struct vector vec;
  vec._data = 0;
  vec._size = 0;
  vec._element_size = element_size;
  vector_reserve(&vec, VECTOR_DEFAULT_INITIAL_CAP);
  return vec;
}

void vector_reserve(struct vector *self, size_t size) {
  assert(size > vector_size(self) && "Must reserve more than the current size");
  size_t num_bytes = size * self->_element_size;
  if (self->_data) {
    self->_data = (void *)krealloc((size_t)self->_data, num_bytes);
  } else {
    self->_data = (void *)kmalloc(num_bytes);
  }
  self->_capacity = num_bytes;
}

void *__vector_alloc_push(struct vector *self, size_t element_size) {
  assert(element_size == self->_element_size && "Invalid element size");
  size_t cap = vector_capacity(self);
  size_t target_size = vector_size(self) + 1;
  if (cap < target_size) {
    vector_reserve(self, target_size);
  }
  self->_size += element_size;
  return __vector_at(self, target_size - 1, element_size);
}

void *__vector_pop(struct vector *self, size_t element_size) {
  assert(element_size == self->_element_size && "Invalid element size");
  self->_size -= element_size;
  void *el = self->_data + self->_size;
  // TODO: shrink capacity
  return el;
}