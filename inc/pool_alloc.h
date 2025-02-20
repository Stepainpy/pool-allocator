/**
 * Chunk structure:
 * 
 * |<---------- chunk_size --------->|
 * |  ptr to next chunk  |  padding  |
 * 
 * chunk_size = max(sizeof ptr, sizeof type)
 * 
 * 
 * Allocation algorithm:
 * When need allocate block, allocator return current value of
 * 'head' and assing him the field 'next' in current chunk.
 * This structure has name free-list.
 * 
 * head/chunks
 * | .---------. .---------. .---------. .---------. .---> NULL
 * V |         V |         V |         V |         V |
 * +---+-------+---+-------+---+-------+---+-------+---+-------+
 * | n |  pad  | n |  pad  | n |  pad  | n |  pad  | n |  pad  |
 * +---+-------+---+-------+---+-------+---+-------+---+-------+
 * 
 * chunks      head
 * |           | .---------. .---------. .---------. .---> NULL
 * V           V |         V |         V |         V |
 * +-----------+---+-------+---+-------+---+-------+---+-------+
 * | alloc mem | n |  pad  | n |  pad  | n |  pad  | n |  pad  |
 * +-----------+---+-------+---+-------+---+-------+---+-------+
 * 
 * chunks                  head
 * |                       | .---------. .---------. .---> NULL
 * V                       V |         V |         V |
 * +-----------------------+---+-------+---+-------+---+-------+
 * |       alloc mem       | n |  pad  | n |  pad  | n |  pad  |
 * +-----------------------+---+-------+---+-------+---+-------+
 * 
 * Free memory:
 * When need free block, allocator write to 'ptr' value of 'head'
 * and assing 'head' as 'ptr'.
 * 
 * chunks                  head
 * |                       | .---------. .---------. .---> NULL
 * V                       V |         V |         V |
 * +-----------------------+---+-------+---+-------+---+-------+
 * |       alloc mem       | n |  pad  | n |  pad  | n |  pad  |
 * +-----------------------+---+-------+---+-------+---+-------+
 * ^
 * |
 * free ptr
 * 
 * chunks/head
 * | .---------------------. .---------. .---------. .---> NULL
 * V |                     V |         V |         V |
 * +---+-------+-----------+---+-------+---+-------+---+-------+
 * | n |  pad  | alloc mem | n |  pad  | n |  pad  | n |  pad  |
 * +---+-------+-----------+---+-------+---+-------+---+-------+
 *             ^
 *             |
 *             free ptr
 * 
 * chunks
 * | ,---------------------. ,---------. ,---------. ,---> NULL
 * V |                     V |         V |         V |
 * +---+-------+---+-------+---+-------+---+-------+---+-------+
 * | n |  pad  | n |  pad  | n |  pad  | n |  pad  | n |  pad  |
 * +---+-------+---+-------+---+-------+---+-------+---+-------+
 * ^           ^ |
 * '-------------'
 *             |
 *             head
 */

#ifndef POOL_ALLOC_H
#define POOL_ALLOC_H

#include <stddef.h>
#include <stdbool.h>

#ifndef PA_DEF
#define PA_DEF
#endif

typedef struct {
    void* head;          // pointer to free node
    void* chunks;        // base pointer to chunks
    size_t chunk_size;   // size of one chunk
    size_t chunk_count;  // count of chunks
} pool_alloc_t;

PA_DEF bool pa_init(pool_alloc_t* pool, size_t sizeof_type,
                    void* buffer, size_t buffer_size);
PA_DEF size_t pa_free_chunk_count(pool_alloc_t* pool);
PA_DEF void* pa_alloc(pool_alloc_t* pool);
PA_DEF void pa_free(pool_alloc_t* pool, void* ptr);
PA_DEF void pa_free_all(pool_alloc_t* pool);

#endif // POOL_ALLOC_H