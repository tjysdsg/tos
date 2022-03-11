#ifndef TOS_TEST_MALLOC_TEST_H
#define TOS_TEST_MALLOC_TEST_H

#include "kernel/kpanic.h"
#include "kernel/memory.h"
#include "kernel/kprintf.h"

class TestStruct {
 public:
  int x = 0;
  int y = 0;
  TestStruct(int _x, int _y) : x(_x), y(_y) {}
  virtual ~TestStruct() = default;
};

inline void malloc_test() {
  /// 1. basic malloc()ing
  uint32_t size = 100;
  uint8_t *a = (uint8_t *) malloc(size);
  memory_block_header_t *header_a = block_header_from_addr(a);
  {
    kassert(a, "malloc() returns nullptr");
    for (uint32_t i = 0; i < size; ++i) {
      a[i] = i * i;
    }
    free(a);

    // now there should be a big unused block
    kassert(header_a->size == size, "Wrong block size");
    kassert(header_a->used == 0, "Should be a free block");
  }

  /// 2. reusing the previous block
  uint32_t sub_size = (size - sizeof(memory_block_header_t)) / 2;
  auto *b = (uint8_t *) malloc(sub_size);
  auto *c = (uint8_t *) malloc(sub_size);
  {
    // kprintf("b=0x%x, c=0x%x\n", b, c);
    kassert(b, "malloc() returns nullptr");
    kassert(c, "malloc() returns nullptr");
    kassert(b == a, "malloc() failed to reused existing memory blocks");

    memory_block_header_t *header_b = block_header_from_addr(b);
    memory_block_header_t *header_c = block_header_from_addr(c);
    kassert(header_b->size == sub_size, "Wrong block size for b");
    kassert(header_c->size == sub_size, "Wrong block size for c");
    // kprintf("header_b=0x%x, header_c=0x%x\n", header_b, header_c);
    kassert(header_c->prev == header_b, "Headers of b and c should be consecutive");

    free(b);
    kassert(header_b->used == 0, "Failed to free memory");
    kassert(header_c->used == 1, "Memory should be marked as used");

    free(c);
    kassert(header_a->size == size, "Free block should be merged");
    kassert(header_a->used == 0, "Should be a free block");
  }

  /// 3. new() and delete()
  {
    int x = 1, y = 2;
    TestStruct *ts = new TestStruct(x, y);
    kassert(ts->x == x, "Incorrect struct member value");
    kassert(ts->y == y, "Incorrect struct member value");

    delete ts;
  }

  kprintf("malloc() tests passed\n");
}

#endif //TOS_TEST_MALLOC_TEST_H
