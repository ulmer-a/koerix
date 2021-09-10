#pragma once

namespace ktl {
  template <typename T>
  class unique_ptr
  {
    public:
      explicit unique_ptr() {
        m_ptr = nullptr;
      }

      unique_ptr(T* ptr) {
        m_ptr = ptr;
      }

      unique_ptr(const unique_ptr& self) = delete;
      unique_ptr& operator=(const unique_ptr& self) = delete;

      unique_ptr& operator=(T* ptr) {
        delete m_ptr;
        m_ptr = ptr;
        return *this;
      }

      unique_ptr& operator=(void* ptr) {
        delete m_ptr;
        m_ptr = (T*)ptr;
        return *this;
      }

      bool operator==(T* ptr) {
        return ptr == m_ptr;
      }

      T& operator*() {
        return *m_ptr;
      }

      T* operator->() const {
        return m_ptr;
      }

      T* get() {
        return m_ptr;
      }

      void clear() {
        *this = nullptr;
      }

      ~unique_ptr() {
        delete m_ptr;
      }

    private:
      T* m_ptr;
  };
}
