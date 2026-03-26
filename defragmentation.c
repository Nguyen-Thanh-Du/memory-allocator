#include <unistd.h>
#include <string.h>
#include <pthread.h>
/* Only for the debug printf */
#include <stdio.h>
#include <stdbool.h> 

typedef char ALIGN[16];

/*
    size: To keep track of that memory block's size - used for free()
    is_free: Differentiate free vs. release-able blocks;
    stub: To align the header to 16-byte
*/
union header {
    struct {
        size_t size;
        bool is_free;
        header_t *next;
    } s;
    ALIGN stub; 
};

typedef union header header_t;
header_t *head, *tail;
pthread_mutex_t global_malloc_lock;

void *malloc(size_t size);
header_t *get_free_block(size_t size);
void free(void *block);
void coalesce(header_t *header);
void *calloc(size_t num, size_t nsize);
void* realloc(void *block, size_t size);
void split_block(header_t *block, size_t size);

// !!!
size_t get_actual_data_size(header_t *header);


// Memomry manager
typedef struct  {
    header_t *head;
    header_t *tail;
    size_t total_size;
    size_t used_size;
    size_t internal_fragmentation;
    size_t external_fragmentation;
} MemoryManager;

void analyze_fragmentation(MemoryManager *manager) {
    header_t *current = manager->head;
    size_t total_internal = 0;
    size_t total_external = 0;
    size_t largest_free_block = 0;

    while (current) {
        if (!current->s.is_free) {
            // Calculate internal fragmentation
            total_internal += current->s.size - get_actual_data_size(current);
        }
        else {
            // Track external fragmentation
            total_external += current->s.size;
            if (current->s.size > largest_free_block)
                largest_free_block = current->s.size;
        }
        current = current->s.next;
    }

    manager->internal_fragmentation = total_internal;
    manager->external_fragmentation = total_external - largest_free_block;
}

typedef struct {
    void *old_address;
    void *new_address;
    size_t size;
} RelocationEntry;

typedef struct {
    RelocationEntry *entries;
    size_t count;
    size_t capacity;
} RelocationTable;

// Phase 1: Analyze and plan relocations


// Phase 2: Perform relocations


// Phase 3: Update memory block list
