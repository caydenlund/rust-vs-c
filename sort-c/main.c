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
    printf("Usage: `%s -n <num_elems> -r <num_repetitions>`\n", prgm);
}

void insertion_sort(uint64_t* arr, size_t num_elems) {
    for (size_t r_ind = 1; r_ind < num_elems; r_ind++) {
        uint64_t key = arr[r_ind];
        size_t l_ind = r_ind - 1;

        while (l_ind >= 0 && arr[l_ind] > key) {
            arr[l_ind + 1] = arr[l_ind];
            l_ind--;
        }

        arr[l_ind + 1] = key;
    }
}

void merge(uint64_t* arr, size_t left, size_t mid, size_t right) {
    size_t left_len = mid - left + 1;
    size_t right_len = right - mid;

    uint64_t* left_arr = (uint64_t*)malloc(left_len * sizeof(uint64_t));
    uint64_t* right_arr = (uint64_t*)malloc(right_len * sizeof(uint64_t));

    memcpy(left_arr, arr, left_len * sizeof(uint64_t));
    memcpy(right_arr, arr + mid + 1, right_len * sizeof(uint64_t));

    size_t left_ind = 0;
    size_t right_ind = 0;
    size_t curr_ind = left;

    while (left_ind < left_len && right_ind < right_len) {
        if (left_arr[left_ind] <= right_arr[right_ind]) {
            arr[curr_ind++] = left_arr[left_ind++];
        } else {
            arr[curr_ind++] = right_arr[right_ind++];
        }
    }

    for (; left_ind < left_len; ++left_ind)
        arr[curr_ind++] = left_arr[left_ind];
    for (; right_ind < right_len; ++right_ind)
        arr[curr_ind++] = right_arr[right_ind];

    free(left_arr);
    free(right_arr);
}

void merge_sort(uint64_t* arr, size_t num_elems) {
    if (num_elems <= 1) { return; }

    size_t mid = num_elems / 2;

    merge_sort(arr, mid);
    merge_sort(arr + mid, num_elems - mid);

    merge(arr, 0, mid - 1, num_elems - 1);
}

void radix_sort(uint64_t* arr, size_t num_elems) {
    if (num_elems <= 1) return;

    uint64_t* temp = malloc(num_elems * sizeof(uint64_t));

    int counts[256];

    for (int shift = 0; shift < 64; shift += 8) {
        memset(counts, 0, sizeof(counts));

        for (size_t i = 0; i < num_elems; i++) {
            uint8_t byte = (arr[i] >> shift) & 0xFF;
            counts[byte]++;
        }

        for (int i = 1; i < 256; i++) { counts[i] += counts[i - 1]; }

        for (size_t i = num_elems; i > 0; i--) {
            uint64_t current = arr[i - 1];
            uint8_t byte = (current >> shift) & 0xFF;
            temp[--counts[byte]] = current;
        }

        memcpy(arr, temp, num_elems * sizeof(uint64_t));
    }

    free(temp);
}

void validate(const uint64_t* arr, size_t num_elems) {
    for (size_t ind = 0; ind < num_elems - 1; ++ind) {
        if (arr[ind] > arr[ind + 1]) {
            (void)fprintf(stderr, "Error: invalid sort\n");
            exit(1);
        }
    }
}

int main(int argc, char** argv) {
    size_t num_elems = 0;
    size_t num_repetitions = 0;

    for (int argi = 1; argi < argc; ++argi) {
        if (strcmp(argv[argi], "-n") == 0) {
            if (++argi == argc) {
                print_usage(argv[0]);
                return 1;
            }
            num_elems = strtoul(argv[argi], NULL, 0);
        } else if (strcmp(argv[argi], "-r") == 0) {
            if (++argi == argc) {
                print_usage(argv[0]);
                return 1;
            }
            num_repetitions = strtoul(argv[argi], NULL, 0);
        }
    }

    if (num_elems == 0) {
        print_usage(argv[0]);
        return 1;
    }
    if (num_repetitions == 0) {
        print_usage(argv[0]);
        return 1;
    }

    struct timespec time_start;
    struct timespec time_end;

    uint64_t total_insertion_time = 0;
    uint64_t total_merge_time = 0;
    uint64_t total_radix_time = 0;
    uint64_t total_validate_time = 0;

    uint64_t val = rand_next(0);

    size_t num_bytes = num_elems * sizeof(uint64_t);

    uint64_t* elems = malloc(num_bytes);
    uint64_t* elems_insertion_clone = malloc(num_bytes);
    uint64_t* elems_merge_clone = malloc(num_bytes);
    uint64_t* elems_radix_clone = malloc(num_bytes);

    for (uint64_t rep = 0; rep < num_repetitions; ++rep) {
        for (size_t ind = 0; ind < num_elems; ++ind) {
            val = rand_next(val);
            elems[ind] = val;
        }
        memcpy(elems_insertion_clone, elems, num_bytes);
        memcpy(elems_merge_clone, elems, num_bytes);
        memcpy(elems_radix_clone, elems, num_bytes);

        clock_gettime(CLOCK_MONOTONIC, &time_start);
        insertion_sort(elems_insertion_clone, num_elems);
        clock_gettime(CLOCK_MONOTONIC, &time_end);
        total_insertion_time += time_diff_ns(time_start, time_end);

        clock_gettime(CLOCK_MONOTONIC, &time_start);
        merge_sort(elems_merge_clone, num_elems);
        clock_gettime(CLOCK_MONOTONIC, &time_end);
        total_merge_time += time_diff_ns(time_start, time_end);

        clock_gettime(CLOCK_MONOTONIC, &time_start);
        radix_sort(elems_radix_clone, num_elems);
        clock_gettime(CLOCK_MONOTONIC, &time_end);
        total_radix_time += time_diff_ns(time_start, time_end);

        clock_gettime(CLOCK_MONOTONIC, &time_start);
        validate(elems_insertion_clone, num_elems);
        validate(elems_insertion_clone, num_elems);
        validate(elems_insertion_clone, num_elems);
        validate(elems_insertion_clone, num_elems);
        clock_gettime(CLOCK_MONOTONIC, &time_end);
        total_validate_time += time_diff_ns(time_start, time_end);
    }

    printf("%lu\t%lu\t%lu\t%lu\t%lu\n", num_elems,
           total_insertion_time / num_repetitions,
           total_merge_time / num_repetitions,
           total_radix_time / num_repetitions,
           total_validate_time / num_repetitions / 3);

    return 0;
}
