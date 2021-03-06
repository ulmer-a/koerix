// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>
#include <debug.h>
#include <lib/string.h>

namespace lib {

  template <typename T>
  class vector
  {
    public:
      vector()
        : m_size(0)
        , m_array(nullptr)
      {
        alloc(0);
      }

      ~vector() {
        delete[] m_array;
      }

      vector(const vector& self)
        : m_size(self.m_size)
      {
        alloc(self.m_size);
        memcpy((void*)m_array, (void*)self.m_array, m_size * sizeof(T));
      }

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
        delete old_array;
      }

    private:
      size_t m_size;
      size_t m_capacity;
      T* m_array;
  };

}
