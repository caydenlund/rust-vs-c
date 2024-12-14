#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    printf("Usage: `%s -n <mat_dim> -r <num_repetitions>`\n", prgm);
}

int main(int argc, char** argv) {
    uint64_t mat_dim = 0;
    uint64_t num_repetitions = 0;

    for (int argi = 1; argi < argc; ++argi) {
        if (strcmp(argv[argi], "-n") == 0) {
            if (++argi == argc) {
                print_usage(argv[0]);
                return 1;
            }
            mat_dim = strtoul(argv[argi], NULL, 0);
        } else if (strcmp(argv[argi], "-r") == 0) {
            if (++argi == argc) {
                print_usage(argv[0]);
                return 1;
            }
            num_repetitions = strtoul(argv[argi], NULL, 0);
        }
    }

    if (mat_dim == 0) {
        print_usage(argv[0]);
        return 1;
    }
    if (num_repetitions == 0) {
        print_usage(argv[0]);
        return 1;
    }

    struct timespec time_start;
    struct timespec time_end;
    uint64_t total_time = 0;

    uint64_t num = rand_next(0);
    for (uint64_t rep = 0; rep < num_repetitions; ++rep) {
        float* mat_a = malloc(mat_dim * mat_dim * sizeof(float));
        float* mat_b = malloc(mat_dim * mat_dim * sizeof(float));
        float* mat_c = malloc(mat_dim * mat_dim * sizeof(float));

        // Prevents `mat_c` from being optimized out
        // See https://github.com/google/benchmark/include/benchmark/benchmark.h
        asm volatile("" : "+r,m"(mat_c) : : "memory");

        for (uint64_t ind = 0; ind < mat_dim * mat_dim; ++ind) {
            num = rand_next(num);
            mat_a[ind] = (float)num;
            num = rand_next(num);
            mat_b[ind] = (float)num;
        }

        clock_gettime(CLOCK_MONOTONIC, &time_start);

        for (int i = 0; i < mat_dim; i++) {
            for (int j = 0; j < mat_dim; j++) {
                mat_c[i * mat_dim + j] = 0;
                for (int k = 0; k < mat_dim; k++) {
                    mat_c[i * mat_dim + j] += mat_a[i * mat_dim + k]
                                            * mat_b[k * mat_dim + j];
                }
            }
        }

        clock_gettime(CLOCK_MONOTONIC, &time_end);
        total_time += time_diff_ns(time_start, time_end);

        free(mat_a);
        free(mat_b);
        free(mat_c);
    }

    printf("%lu\n", total_time / num_repetitions);

    return 0;
}
