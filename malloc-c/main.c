#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_ALLOC_SIZE 8
#define MAX_ALLOC_SIZE 1024

typedef struct {
    uint64_t total_alloc_time_ns;
    uint64_t total_dealloc_time_ns;
    size_t total_memory_allocated;
} MemoryBenchmarkResult;

uint64_t rand_next(uint64_t seed) {
    const uint64_t MODULUS = (uint64_t)1 << 31;
    const uint64_t MULTIPLIER = 1103515245;
    const uint64_t INCREMENT = 12345;

    return (seed * MULTIPLIER + INCREMENT) % MODULUS;
}

uint64_t time_diff_ns(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000000000ULL + (end.tv_nsec - start.tv_nsec);
}

MemoryBenchmarkResult benchmark_random_allocs(size_t alloc_count) {
    void** allocations = malloc(alloc_count * sizeof(void*));
    if (!allocations) {
        fprintf(stderr, "Memory allocation for tracking failed.\n");
        exit(EXIT_FAILURE);
    }

    size_t total_memory_allocated = 0;
    uint64_t size = rand_next(0);

    struct timespec allocation_start, allocation_end;
    clock_gettime(CLOCK_MONOTONIC, &allocation_start);

    for (size_t i = 0; i < alloc_count; i++) {
        size = MIN_ALLOC_SIZE + rand_next(size) % (MAX_ALLOC_SIZE - MIN_ALLOC_SIZE);
        total_memory_allocated += size;
        allocations[i] = calloc(size, 1);
    }

    clock_gettime(CLOCK_MONOTONIC, &allocation_end);
    uint64_t total_alloc_time_ns = time_diff_ns(allocation_start, allocation_end);

    struct timespec deallocation_start, deallocation_end;
    clock_gettime(CLOCK_MONOTONIC, &deallocation_start);

    for (size_t i = 0; i < alloc_count; i++) { free(allocations[i]); }
    free(allocations);

    clock_gettime(CLOCK_MONOTONIC, &deallocation_end);
    uint64_t total_dealloc_time_ns = time_diff_ns(deallocation_start, deallocation_end);

    MemoryBenchmarkResult result = {
            .total_alloc_time_ns = total_alloc_time_ns,
            .total_dealloc_time_ns = total_dealloc_time_ns,
            .total_memory_allocated = total_memory_allocated,
    };

    return result;
}

int main(int argc, char** argv) {
    if (argc != 2) { fprintf(stderr, "Usage: `%s <alloc_count>`\n", argv[0]); exit(1); }
    uint64_t alloc_count = atoll(argv[1]);
    MemoryBenchmarkResult result = benchmark_random_allocs(alloc_count);

    printf("Total allocation time:   %lu ns\n", result.total_alloc_time_ns);
    printf("Total deallocation time: %lu ns\n", result.total_dealloc_time_ns);
    printf("Total memory allocated:  %zu B\n", result.total_memory_allocated);
    printf("Total allocations:       %zu\n", alloc_count);

    return 0;
}
