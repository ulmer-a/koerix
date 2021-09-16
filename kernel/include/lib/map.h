// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

/* Map<K,V> is a template class implementing a map container using
 * a binary search tree. I'd like to update this to a red-black map
 * in the future as used by the std::map in the STL to make the find()
 * operation faster. For now, this works fine, though. */

#pragma once

#include <debug.h>
#include <types.h>

namespace lib {

  template <typename K, typename V>
  class Map;

  template <typename K, typename V>
  class MapEntry
  {
      friend class Map<K, V>;

    public:
      MapEntry(const K& key)
        : m_key(key)
        , m_left(nullptr)
        , m_right(nullptr)
      { }

      inline K& key() { return m_key; }
      inline V& value() { return m_value; }

    protected:
      void insert(MapEntry<K, V>*& root,
                  MapEntry<K, V>* parent = nullptr)
      {
        if (root == nullptr) {
          root = this;
          m_parent = parent;
          return;
        }

        if (root->m_key > m_key) {
          insert(root->m_left, root);
        } else {
          insert(root->m_right, root);
        }
      }

      unsigned long height()
      {
        return 1 + max(
          m_left ? m_left->height() : 0,
          m_right ? m_right->height() : 0);
      }

      void erase()
      {
        if (m_right->height() > m_left->height()) {
          m_parent->me(this) = m_right;
          m_right->insert(m_parent);
        } else {
          m_parent->me(this) = m_left;
          m_right->insert(m_parent);
        }
      }

      MapEntry<K, V>*& me(MapEntry<K, V>* ptr) {
        if (m_left == ptr)
          return m_left;
        if (m_right == ptr)
          return m_right;
        assert(false);
      }

      MapEntry<K, V>* find(const K& key)
      {
        if (m_key > key)
        {
          if (m_left == nullptr)
            return nullptr;
          return m_left->find(key);
        }
        else if (m_key < key)
        {
          if (m_right == nullptr)
            return nullptr;
          return m_right->find(key);
        }
        else
        {
          return this;
        }
      }

      MapEntry<K, V>* copy()
      {
        return new MapEntry<K, V>(*this);
      }

      void destroyChildren()
      {
        if (m_left)
        {
          m_left->destroyChildren();
          delete m_left;
        }

        if (m_right)
        {
          m_right->destroyChildren();
          delete m_right;
        }
      }

    private:
      MapEntry(const MapEntry& self)
        : m_key(self.m_key)
        , m_value(self.m_value)
      {
        if (self.m_left)
          m_left = self.m_left->copy();
        if (self.m_right)
          m_right = self.m_right->copy();
      }

    private:
      K m_key;
      V m_value;
      MapEntry *m_parent;
      MapEntry *m_left, *m_right;
  };

  template <typename K, typename V>
  class Map
  {
    public:
      Map()
        : m_root(nullptr)
      { }

      ~Map()
      {
        if (m_root != nullptr)
        {
          m_root->destroyChildren();
          delete m_root;
        }
      }

      Map(const Map& self)
      {
        if (self.m_root)
          m_root = self.m_root->copy();
        else
          m_root = nullptr;
      }

      MapEntry<K, V>* find(const K& key)
      {
        if (m_root == nullptr)
          return nullptr;
        return m_root->find(key);
      }

      bool erase(const K& key)
      {
        MapEntry<K, V>* entry = find(key);
        if (entry == nullptr)
          return false;

        entry->erase();
        delete entry;
        return true;
      }

      V& operator[](const K& key)
      {
        /* check if the entry exists. if it does,
         * return a reference to the value. */
        auto entry = find(key);
        if (entry)
          return entry->value();

        /* otherwise, create an entry and insert it
         * into the map, and then return a reference
         * to the value. */
        entry = new MapEntry<K, V>(key);
        entry->insert(m_root);
        return entry->value();
      }

    private:
      MapEntry<K, V> *m_root;
  };

}
