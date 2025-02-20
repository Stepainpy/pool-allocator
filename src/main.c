#include <stdio.h>
#include "pool_alloc.h"

void pool_info(pool_alloc_t* pool, size_t count) {
    char info[64] = {0};
    sprintf(info, "Chunk size/free count: %zu/%zu",
        pool->chunk_size, pa_free_chunk_count(pool));

#define DASHES "--------------------------------------------------"

    printf("+-"DASHES"-+\n");
    printf("| %-50s |\n", info);
    printf("+-"DASHES"-+\n");
    void* current = pool->head;
    while (current && count--) {
        printf("| Address: %p | Next: %p |\n",
            current, *(void**)current);
        current = *(void**)current;
    }
    if (current) printf("| %-50s |\n", "And so on ...");
    printf("+-"DASHES"-+\n");
}

#define FIRST_N_CHUNKS 4
#define BUFFER_MAX 65536
char buffer[BUFFER_MAX];

int main(void) {
    pool_alloc_t pool = {0};
    if (!pa_init(&pool, sizeof(int), buffer, BUFFER_MAX))
        return 1;

    pool_info(&pool, FIRST_N_CHUNKS);

    int* a = (int*)pa_alloc(&pool); *a =  69;
    int* b = (int*)pa_alloc(&pool); *b = 420;
    printf("First  alloc: %d\n", *a);
    printf("Second alloc: %d\n", *b);

    pool_info(&pool, FIRST_N_CHUNKS);

    pa_free(&pool, a);
    pa_free(&pool, b);

    pool_info(&pool, FIRST_N_CHUNKS);
    return 0;
}