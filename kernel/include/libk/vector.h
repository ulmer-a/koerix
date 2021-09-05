#pragma once

#include <types.h>
#include <debug.h>
#include <string.h>

namespace ktl {

  template <typename T>
  class vector
  {
    public:
      vector() {
        alloc(0);
      }

      ~vector() {
        delete[] m_array;
      }

      vector(const vector& self) = delete;
      vector& operator=(const vector& self) = delete;

      T& operator[](size_t index) {
        assert(index < m_size);
        return m_array[index];
      }

      const T& operator[](size_t index) const {
        assert(index < m_size);
        return m_array[index];
      }

      void push_back(const T& item) {
        if (m_size + 1 > m_capacity) {
          realloc(m_size + 1);
        }
        m_array[m_size++] = item;
      }

      T pop_back() {
        return m_array[--m_size];
      }

      inline size_t size() const {
        return m_size;
      }

    private:
      void alloc(size_t capacity) {
        capacity = ((capacity >> 4) << 4) + 16;
        m_array = new T[capacity];
        m_capacity = capacity;
      }

      void realloc(size_t capacity) {
        T* old_array = m_array;
        alloc(capacity);
        memcpy(m_array, old_array, sizeof(T) * m_size);
      }

    private:
      size_t m_size;
      size_t m_capacity;
      T* m_array;
  };

}
