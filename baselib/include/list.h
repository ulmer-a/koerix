#pragma once

#include <types.h>
#include <canary.h>

namespace ktl {

    namespace _internal {
        template<typename T>
        struct ListItem
        {
          T item;
          ListItem<T>* next;
          ListItem<T>* prev;

          T& operator->() {
            return item;
          }

          T& operator*() {
            return item;
          }
        };
    }

    template<typename T>
    class List : public Canary
    {
      public:
        List() = default;

        ~List() {
          verify();
          clear();
        }

        void clear() {
          verify();
          while (m_size > 0)
            remove(m_first);
        }

        size_t size() const {
          verify();
          return m_size;
        }

        void push_back(const T& item) {
            verify();
            auto it = new _internal::ListItem<T> {
                item, nullptr, m_last
            };

            if (m_last)
              m_last->next = it;
            else
              m_first = it;
            m_last = it;

            m_size += 1;
        }

        _internal::ListItem<T>* find(const T& item) {
          verify();
          for (auto it = m_first; it != nullptr; it = it->next)
          {
            if (it->item == item)
              return it;
          }
          return nullptr;
        }

        void remove(_internal::ListItem<T>* it) {
          assert(it != nullptr);
          verify();

          if (it->prev)
            it->prev->next = it->next;
          if (it->next)
            it->next->prev = it->prev;

          if (it == m_first)
            m_first = it->next;
          if (it == m_last)
            m_last = it->prev;

          m_size -= 1;
          delete it;
        }

        void rotate() {
          verify();

          if (m_size < 2)
            return;

          auto it = m_first;
          it->next->prev = nullptr;
          it->prev = m_last;
          m_first = it->next;
          it->next = nullptr;
          m_last->next = it;
          m_last = it;
        }

        T front() {
          verify();
          assert(m_first && m_size > 0);
          return m_first->item;
        }

        T pop_front() {
          verify();
          if (!m_first || m_size == 0)
            assert(false);
          T item = m_first->item;
          remove(m_first);
          return item;
        }

        T back() {
          verify();
          assert(m_last && m_size > 0);
          return m_last->item;
        }

        _internal::ListItem<T>* begin() {
          return m_first;
        }

        _internal::ListItem<T>* end() {
          return m_last;
        }

      private:
        size_t m_size;
        _internal::ListItem<T>* m_first;
        _internal::ListItem<T>* m_last;
    };

}
