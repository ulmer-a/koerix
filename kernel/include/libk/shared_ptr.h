// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>
#include <arch/asm.h>
#include <debug.h>

namespace ktl {
  namespace _internal {
    template <typename T>
    class shared_ptr_data
    {
      public:
        shared_ptr_data(T* ptr) {
          m_ptr = ptr;
          m_refs = 1;
        }

        size_t addRef() {
          return atomic_add(&m_refs, 1);
        }

        size_t release() {
          return atomic_add(&m_refs, -1);
        }

        inline T* ptr() {
          return m_ptr;
        }

      private:
        T* m_ptr;
        size_t m_refs;
    };
  }

  template <typename T>
  class shared_ptr
  {
    public:
      explicit shared_ptr() {
        m_data = nullptr;
      }

      explicit shared_ptr(T* ptr) {
        m_data = new _internal::shared_ptr_data<T>(ptr);
      }

      ~shared_ptr() {
        clear();
      }

      shared_ptr(const shared_ptr& self)
        : m_data(self.m_data) {
        assert_verify(m_data->addRef() != 0);
      }

      shared_ptr& operator=(const shared_ptr& self) {
        clear();
        m_data = self.m_data;
        assert_verify(m_data->addRef() != 0);
        return *this;
      }

      shared_ptr& operator=(T* ptr) {
        clear();
        m_data = new _internal::shared_ptr_data<T>(ptr);
      }

      T& operator*() {
        return *get();
      }

      T* operator->() {
        return get();
      }

      bool operator==(void* ptr) const {
        if (m_data != nullptr)
          return m_data->ptr() == ptr;
        return ptr == nullptr;
      }

      bool isNull() const {
        return *this == nullptr;
      }

      T* get() {
        T* ptr = nullptr;
        if (m_data) {
          ptr = m_data->ptr();
          assert(ptr);
        }
        return ptr;
      }

      void clear() {
        if (m_data == nullptr)
          return;
        if (m_data->release() == 1) {
          delete m_data;
          m_data = nullptr;
        }
      }

    private:
      _internal::shared_ptr_data<T> *m_data;
  };
}
