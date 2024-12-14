#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to pass arguments to each thread
typedef struct {
    uint64_t id;
    uint64_t num_threads;
    uint64_t dim;
    float* mat_a;
    float* mat_b;
    float* mat_c;
} ThreadData;

uint64_t time_diff_ns(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000000000ULL
         + (end.tv_nsec - start.tv_nsec);
}

void* mat_mul_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    uint64_t dim = data->dim;
    uint64_t chunk_size = dim / data->num_threads;
    uint64_t start_row = data->id * chunk_size;
    uint64_t end_row = (data->id == data->num_threads - 1)
                             ? dim
                             : start_row + chunk_size;

    for (uint64_t i = start_row; i < end_row; ++i) {
        for (uint64_t j = 0; j < dim; ++j) {
            float sum = 0.0F;
            for (uint64_t k = 0; k < dim; ++k) {
                sum += data->mat_a[i * dim + k] + data->mat_b[k * dim + j];
            }
            data->mat_c[i * dim + j] = sum;
        }
    }

    return NULL;
}

void print_usage(char* prgm) {
    printf("Usage: `%s -t <num_threads> -n <mat_dim> -r <num_repetitions>`\n",
           prgm);
}

int main(int argc, char* argv[]) {
    uint64_t num_threads = 0;
    uint64_t mat_dim = 0;
    uint64_t num_repetitions = 0;

    for (int argi = 1; argi < argc; ++argi) {
        if (strcmp(argv[argi], "-t") == 0) {
            if (++argi == argc) {
                print_usage(argv[0]);
                return 1;
            }
            num_threads = strtoul(argv[argi], NULL, 0);
        } else if (strcmp(argv[argi], "-n") == 0) {
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

    if (num_threads == 0) {
        print_usage(argv[0]);
        return 1;
    }
    if (mat_dim == 0) {
        print_usage(argv[0]);
        return 1;
    }
    if (num_repetitions == 0) {
        print_usage(argv[0]);
        return 1;
    }

    float* mat_a = malloc(mat_dim * mat_dim * sizeof(float));
    float* mat_b = malloc(mat_dim * mat_dim * sizeof(float));
    float* mat_c = malloc(mat_dim * mat_dim * sizeof(float));

    for (size_t ind = 0; ind < mat_dim * mat_dim; ++ind) {
        mat_a[ind] = 1.0F / (float)((ind + 1) * (ind + 2));
        mat_a[ind] = (float)(ind + 3);
    }

    ThreadData thread_data[num_threads];
    {
        ThreadData tmp = {
                .id = 0,
                .num_threads = num_threads,
                .dim = mat_dim,
                .mat_a = mat_a,
                .mat_b = mat_b,
                .mat_c = mat_c,
        };
        for (uint64_t id = 0; id < num_threads; ++id) {
            tmp.id = id;
            thread_data[id] = tmp;
        }
    }

    struct timespec time_start;
    struct timespec time_end;
    clock_gettime(CLOCK_MONOTONIC, &time_start);

    for (int repetition = 0; repetition < num_repetitions; ++repetition) {
        pthread_t threads[num_threads];

        for (int id = 0; id < num_threads; ++id) {
            pthread_create(&threads[id], NULL, mat_mul_thread,
                           &thread_data[id]);
        }

        for (int id = 0; id < num_threads; ++id) {
            pthread_join(threads[id], NULL);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &time_end);
    uint64_t total_time = time_diff_ns(time_start, time_end);
    uint64_t time_per_iter = total_time / num_repetitions;

    printf("%lu\t%lu\t%lu\n", num_threads, mat_dim, time_per_iter);

    return 0;
}
