#include "pool_alloc.h"

size_t pa_free_chunk_count(pool_alloc_t* pool) {
    size_t count = 0;
    void* current_chunk = pool->head;
    while (current_chunk) {
        current_chunk = *(void**)current_chunk;
        ++count;
    }
    return count;
}

bool pa_init(pool_alloc_t* pool, size_t sizeof_type, void* buffer, size_t buffer_size) {
    if (!pool || !buffer)
        return false;

    const size_t chunk_size  =
          sizeof(void*) > sizeof_type
        ? sizeof(void*) : sizeof_type;
    const size_t chunk_count = buffer_size / chunk_size;

    pool->chunks = buffer;
    pool->head = chunk_count ? buffer : NULL;
    pool->chunk_size  = chunk_size;
    pool->chunk_count = chunk_count;

    pa_free_all(pool);

    return true;
}

void* pa_alloc(pool_alloc_t* pool) {
    void* out = pool->head;
    if (out) pool->head = *(void**)out;
    return out;
}

void pa_free(pool_alloc_t* pool, void* ptr) {
    if (!ptr || ptr < pool->chunks) return;
    size_t len = (char*)ptr - (char*)pool->chunks;
    if (len > pool->chunk_size * pool->chunk_count) return;
    if (len % pool->chunk_size != 0) return;
    
    *(void**)ptr = pool->head;
    pool->head = ptr;
}

void pa_free_all(pool_alloc_t* pool) {
    char* chunk = (char*)pool->chunks;
    for (size_t i = 0; pool->chunk_count > 0 && i < pool->chunk_count - 1; i++) {
        char* next_chunk = chunk + pool->chunk_size;
        *(void**)chunk = next_chunk;
        chunk = next_chunk;
    }
    if (pool->chunk_count)
        *(void**)chunk = NULL;
}