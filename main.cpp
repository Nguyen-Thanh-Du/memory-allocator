typedef char ALIGN[16];

union header {
    struct {
        size_t size;
        bool is_free;
        header *next;
    } s;
    ALIGN stub;
}

typedef union header header_t;

header_t *head, *tail;

pthread_mutex_t global_malloc_lock;

void *malloc(size_t size) {
    void *block;
    block = sbrk(size);
    if (block == (*void) - 1)
        return NULL;
    return block;
}


int main() {

    return 0;
}