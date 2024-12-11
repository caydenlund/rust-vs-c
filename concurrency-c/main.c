#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"

// Structure to pass arguments to each thread
typedef struct {
    uint64_t start;
    uint64_t end;
    uint64_t partial_sum;
} ThreadData;

// Function executed by each thread
void* thread_sum(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    data->partial_sum = 0;
    for (uint64_t i = data->start; i <= data->end; i++) {
        data->partial_sum += i;
    }
    return NULL;
}

void print_usage(char* prgm) {
    printf("Usage: `%s -t <num_threads> -n <number> -r <num_repetitions>`",
           prgm);
}

int main(int argc, char* argv[]) {
    uint64_t num_threads = 0;
    ArgParser num_threads_parser = arg_parser("-t", argc, argv, &num_threads,
                                              print_usage);
    uint64_t number = 0;
    ArgParser number_parser = arg_parser("-n", argc, argv, &number,
                                         print_usage);
    uint64_t num_repetitions = 0;
    ArgParser num_repetitions_parser = arg_parser(
            "-r", argc, argv, &num_repetitions, print_usage);

    for (int argi = 1; argi < argc; ++argi) {
        parse_arg(num_threads_parser, &argi);
        parse_arg(number_parser, &argi);
        parse_arg(num_repetitions_parser, &argi);
    }

    if (num_threads == 0) {
        print_usage(argv[0]);
        return 1;
    }
    if (number == 0) {
        print_usage(argv[0]);
        return 1;
    }
    if (num_repetitions == 0) {
        print_usage(argv[0]);
        return 1;
    }

    for (int repetition = 0; repetition < num_repetitions; ++repetition) {
        pthread_t threads[num_threads];
        ThreadData thread_data[num_threads];

        uint64_t numbers_per_thread = number / num_threads;
        uint64_t remainder = number % num_threads;

        uint64_t current_start = 1;
        for (int i = 0; i < num_threads; ++i) {
            thread_data[i].start = current_start;
            thread_data[i].end = current_start + numbers_per_thread - 1;
            if (i < remainder) { thread_data[i].end += 1; }
            current_start = thread_data[i].end + 1;

            if (pthread_create(&threads[i], NULL, thread_sum, &thread_data[i])
                != 0) {
                perror("pthread_create");
                return 1;
            }
        }

        uint64_t total_sum = 0;
        for (int i = 0; i < num_threads; ++i) {
            if (pthread_join(threads[i], NULL) != 0) {
                perror("pthread_join");
                return 1;
            }
            total_sum += thread_data[i].partial_sum;
        }

        // printf("Total sum from 1 to %lu: %lu\n", number, total_sum);
    }

    return 0;
}
