#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* arg_name;
    int argc;
    char** argv;
    uint64_t* arg_var;
    void (*print_usage)(char*);
} ArgParser;

static inline ArgParser arg_parser(char* arg_name, int argc, char** argv,
                                   uint64_t* arg_var,
                                   void (*print_usage)(char*)) {
    ArgParser result = {
            .arg_name = arg_name,
            .argc = argc,
            .argv = argv,
            .arg_var = arg_var,
            .print_usage = print_usage,
    };
    return result;
}

static inline void parse_arg(ArgParser parser, int* argi) {
    if (strcmp(parser.argv[*argi], parser.arg_name) == 0) {
        *argi += 1;
        if (*argi == parser.argc) {
            parser.print_usage(parser.argv[0]);
            exit(1);
        }
        *(parser.arg_var) = strtoul(parser.argv[*argi], NULL, 0);
    }
}
