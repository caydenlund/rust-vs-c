#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_ALLOC_SIZE 8
#define MAX_ALLOC_SIZE 1024

uint64_t time_diff_ns(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000000000ULL
         + (end.tv_nsec - start.tv_nsec);
}

uint64_t rand_next(uint64_t seed) {
    const uint64_t MODULUS = (uint64_t)1 << 31;
    const uint64_t MULTIPLIER = 1103515245;
    const uint64_t INCREMENT = 12345;

    return (seed * MULTIPLIER + INCREMENT) % MODULUS;
}

void print_usage(char* prgm) {
    printf("Usage: `%s -n <num_allocs> -r <num_repetitions>`\n", prgm);
}

int main(int argc, char** argv) {
    uint64_t num_allocs = 0;
    uint64_t num_repetitions = 0;

    for (int argi = 1; argi < argc; ++argi) {
        if (strcmp(argv[argi], "-n") == 0) {
            if (++argi == argc) {
                print_usage(argv[0]);
                return 1;
            }
            num_allocs = strtoul(argv[argi], NULL, 0);
        } else if (strcmp(argv[argi], "-r") == 0) {
            if (++argi == argc) {
                print_usage(argv[0]);
                return 1;
            }
            num_repetitions = strtoul(argv[argi], NULL, 0);
        }
    }

    if (num_allocs == 0) {
        print_usage(argv[0]);
        return 1;
    }
    if (num_repetitions == 0) {
        print_usage(argv[0]);
        return 1;
    }

    struct timespec alloc_start;
    struct timespec alloc_end;
    uint64_t total_alloc_time = 0;

    struct timespec free_start;
    struct timespec free_end;
    uint64_t total_free_time = 0;

    uint64_t size = rand_next(0);
    for (uint64_t rep = 0; rep < num_repetitions; ++rep) {
        clock_gettime(CLOCK_MONOTONIC, &alloc_start);
        void** allocations = malloc(num_allocs * sizeof(void*));
        for (size_t i = 0; i < num_allocs; i++) {
            size = MIN_ALLOC_SIZE
                 + rand_next(size) % (MAX_ALLOC_SIZE - MIN_ALLOC_SIZE);
            allocations[i] = calloc(size, 1);
        }
        clock_gettime(CLOCK_MONOTONIC, &alloc_end);
        total_alloc_time += time_diff_ns(alloc_start, alloc_end);

        clock_gettime(CLOCK_MONOTONIC, &free_start);
        for (size_t i = 0; i < num_allocs; i++) free(allocations[i]);
        free(allocations);
        clock_gettime(CLOCK_MONOTONIC, &free_end);
        total_free_time += time_diff_ns(free_start, free_end);
    }

    printf("%lu\t%lu\n", total_alloc_time / num_repetitions,
           total_free_time / num_repetitions);

    return 0;
}
