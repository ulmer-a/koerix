// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

namespace ktl {

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
      void insert(MapEntry<K, V>*& root)
      {
        if (root == nullptr) {
          root = this;
          return;
        }

        if (root->m_key > m_key) {
          insert(root->m_left);
        } else {
          insert(root->m_right);
        }
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

      void del()
      {
        if (m_left)
        {
          m_left->del();
          delete m_left;
        }

        if (m_right)
        {
          m_right->del();
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
        if (m_root)
        {
          m_root->del();
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
        // TODO: implement
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
