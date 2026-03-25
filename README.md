# Memory Allocator in C
> A custom implementation of `malloc`, `free`, `calloc`, and `realloc` built from scratch to understand how memory allocation works under the hood.

---

## Core Concepts Learned

### 1. The Heap and `sbrk()`
The heap is a region of memory a program can grow dynamically. `sbrk(n)` asks the OS to extend the heap by `n` bytes and returns a pointer to the new memory. This is how all raw memory in this allocator is obtained.

---

### 2. The Header
Every allocated block is prefixed with a hidden metadata header.

```
Memory layout of a single allocation:
┌──────────────────────┬──────────────────────────────┐
│       header_t       │         user data             │
│  size | free | next  │  <-- pointer returned to user │
└──────────────────────┴──────────────────────────────┘
^                       ^
header pointer          header + 1  (what malloc returns)
```

The header is defined as a union to enforce 16-byte alignment:

```c
typedef char ALIGN[16];

union header {
    struct {
        size_t  size;     // how big is the data block
        bool    is_free;  // is this block available?
        header *next;     // pointer to the next block
    } s;
    ALIGN stub;           // forces the union to be exactly 16 bytes
};
```

---

### 3. Why 16-Byte Alignment?
CPUs read memory in chunks and prefer data to start at addresses divisible by certain numbers (8 or 16 on 64-bit systems). Misaligned access is either slower or causes a crash on some architectures.

By making the header exactly 16 bytes, the user data that follows it always starts at a 16-byte aligned address — as long as the OS gives us an aligned starting address (which it guarantees).

## The Four Core Functions

### `malloc(size_t size)`
Allocates a block of `size` bytes.

**How it works:**
1. Lock the mutex
2. Search the linked list for a free block big enough (`get_free_block`)
3. If found → try to split it, mark it used, return pointer
4. If not found → call `sbrk()` for new memory, build a header, append to list
5. Unlock the mutex

---

## What's Next

- **Block coalescing** — merge adjacent free blocks to prevent fragmentation
x- **Best-fit search** — find the smallest sufficient block instead of first-fit
- **`mmap` support** — use `mmap(MAP_ANONYMOUS)` for large allocations instead of `sbrk`
