/*
 * UlmerOS kernel heap implementation
 * Copyright (C) 2019-2021 Alexander Ulmer
 */

#include <types.h>
#include <mutex.h>
#include <mm.h>
#include <debug.h>
#include <platform.h>

struct HeapBlock
{
  bool available;
  uint32_t magic;
  HeapBlock *prev;
  HeapBlock *next;
#ifdef DEBUG
  const char* function;
  unsigned int line;
#endif
  size_t size;
} _PACKED;

#define HEAP_MAGIC  (0xabcdefabu)
#define HEADER_SIZE (sizeof(HeapBlock))

char* kheap_start_ = (char*)KHEAP_START;
char* kheap_break_ = (char*)KHEAP_START;

static HeapBlock *heap_start = nullptr;
static HeapBlock *heap_last = nullptr;
static Mutex s_heapMutex;

static void initHeap()
{
    new (&s_heapMutex) Mutex();
}

void kheap_print()
{
  s_heapMutex.lock();

  debug() << "--- HEAP DUMP ---\n";
  HeapBlock *entry;
  for (HeapBlock* entry = heap_start;
       entry != nullptr;
       entry = entry->next)
  {
    if (entry->magic != HEAP_MAGIC) {
      panic("kheap: heap corruption detected");
    }

    if (entry->available)
    {
        debug() << "  @ " << (entry + 1) << ": " << entry->size << "B [free]\n";
    }
    else
    {
#ifdef DEBUG
      debug(loglevel, "  @ %p: %zu bytes (%s:%u)\n", entry + 1, entry->size,
            entry->function, entry->line);
#else
        debug() << "  @ " << (entry + 1) << ": " << entry->size << "B [used]\n";
#endif
    }
  }

  s_heapMutex.unlock();
}

void _kheap_check_corrupt(const char* func, unsigned line)
{
  for (HeapBlock* entry = (HeapBlock*)kheap_start_;
       entry != nullptr;
       entry = entry->next)
  {
    if (entry->magic != HEAP_MAGIC)
    {
      debug() << "heap corruption detected in " << func << "():" << line << "\n";
      panic("heap corruption detected");
    }
  }
}

static void* kbrk(ssize_t increment)
{
  void *orig_brk = kheap_break_;
  kheap_break_ += increment;

  if (orig_brk > kheap_break_)
  {
    size_t first_unmap = (size_t)kheap_break_ / PAGE_SIZE;
    if ((size_t)kheap_break_ % PAGE_SIZE != 0)
      first_unmap += 1;
    size_t last_unmap = (size_t)orig_brk / PAGE_SIZE;

    // debug() << "kheap: freeing pages " << first_unmap << " to " << last_unmap << "\n";
    for (size_t page = first_unmap; page <= last_unmap; page++)
      AddrSpace::kernel().unmap(page);
  }
  else
  {
    size_t first_map = (size_t)orig_brk >> PAGE_SHIFT;
    if ((size_t)orig_brk % PAGE_SIZE != 0)
      first_map += 1;
    size_t last_map = (size_t)kheap_break_ >> PAGE_SHIFT;

    //debug() << "kheap: alloc " << (last_map - first_map + 1)
    //        << " pages @ "
    //        << (void*)(first_map << PAGE_SHIFT) << "\n";
    for (size_t page = first_map; page <= last_map; page++)
    {
        AddrSpace::kernel().map(page, PageMap::get().alloc(),
            AddrSpace::MAP_NOEXEC | AddrSpace::MAP_WRITE);
    }
  }

  return orig_brk;
}

static void crop(HeapBlock *block, size_t new_size)
{
  size_t excess_size = block->size - new_size;
  if (excess_size <= HEADER_SIZE) {
    // don't do anything
    return;
  }

  // update current block
  HeapBlock *original_next = block->next;
  block->size = new_size;
  block->next = (HeapBlock*)((size_t)block + new_size);

  // allocate new block
  block->next->available = 1;
  block->next->magic = HEAP_MAGIC;
  block->next->prev = block;
  block->next->next = original_next;
  block->next->size = excess_size;

  if (block->next->next != nullptr) {
    // update the next block's prev pointer
    block->next->next->prev = block->next;
  } else {
    // update heap info
    heap_last = block->next;
  }
}

#ifdef DEBUG
void *_kmalloc(size_t size, unsigned line, const char* function)
#else
void *kmalloc(size_t size)
#endif
{
#ifdef DEBUG
  debug(KHEAP, "kmalloc(): size %zd from %s():%u\n", size, function, line);
#endif

  //debug() << "kmalloc(): " << size << "B\n";

  s_heapMutex.lock();

  HeapBlock *entry;
  for (entry = heap_start; entry != nullptr; entry = entry->next)
  {
    if (entry->magic != HEAP_MAGIC) {
      panic("kheap: heap corruption detected");
    }

    size_t required_size = HEADER_SIZE + size;
    if (entry->available && entry->size >= required_size)
    {
      entry->available = 0;

#ifdef DEBUG
      entry->function = function;
      entry->line = line;
#endif

      crop(entry, required_size);
      s_heapMutex.unlock();
      return entry + 1;
    }
  }

  HeapBlock *blk = (HeapBlock*)kbrk(size + HEADER_SIZE);
  if (blk == (void*)-1) {
    s_heapMutex.unlock();
    return nullptr;
  }

  blk->available = 0;
  blk->magic = HEAP_MAGIC;
  blk->next = nullptr;
  blk->prev = heap_last;
  blk->size = HEADER_SIZE + size;

#ifdef DEBUG
  blk->function = function;
  blk->line = line;
#endif

  if (heap_last != nullptr)
    heap_last->next = blk;
  heap_last = blk;

  if (heap_start == nullptr)
    heap_start = blk;

  s_heapMutex.unlock();

  return blk + 1;
}

static void mergeBlocks(HeapBlock *first, HeapBlock *second)
{
  if (first->magic != HEAP_MAGIC || second->magic != HEAP_MAGIC) {
    panic("kheap: heap corruption detected");
  }

  first->size += second->size;
  first->next = second->next;
  if (second->next != nullptr) {
    second->next->prev = first;
  } else {
    heap_last = first;
  }

  second->magic = 0;
}

void kfree(void *ptr)
{
  //debug() << "kfree(): " << ptr << "\n";

  if (ptr == nullptr) {
    return;
  }

  if (heap_last == nullptr || heap_start == nullptr ||
        ptr < (void*)(heap_start) || ptr > (void*)(heap_last + 1)) {
    panic("kheap: heap corruption detected");
  }

  s_heapMutex.lock();

  HeapBlock *tofree = (HeapBlock *)ptr - 1;
  if (tofree->magic != HEAP_MAGIC || tofree->available) {
    panic("kheap: heap corruption detected");
  }

  tofree->available = true;

  // merge free blocks
  if (tofree->next != nullptr && tofree->next->available)
    mergeBlocks(tofree, tofree->next);
  if (tofree->prev != nullptr && tofree->prev->available)
    mergeBlocks(tofree->prev, tofree);

  // decrease heap size when possible
  while (heap_last != nullptr && heap_last->available)
  {
    // invalidate
    heap_last->magic = 0;

    size_t decrement = heap_last->size;
    if (heap_last->prev != nullptr) {
      heap_last->prev->next = nullptr;
      heap_last = heap_last->prev;
    } else {
      heap_start = nullptr;
      heap_last = nullptr;
    }

    // decrease heap break
    kbrk(-decrement);
  }

  s_heapMutex.unlock();
}
