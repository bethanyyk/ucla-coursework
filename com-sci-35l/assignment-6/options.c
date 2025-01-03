#include "options.h"
#include "rand64-sw.h"

int process_options(int argc, char **argv, long long* nbytes, int* inputChoice, int* outputChoice, long long* chunksize) {
    int opt;
    
    int inputFlag = 0;
    int outputFlag = 0;
    int nonFlaggedCount = 0;
    
    char *inputArg = NULL;
    char *outputArg = NULL;

    // Parse the flags and set the options to their respective arguments
    while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
            case 'i':
                inputFlag++;
                inputArg = optarg;
                break;
            case 'o':
                outputFlag++;
                outputArg = optarg;
                break;
            case '?':
                fprintf(stderr, "Usage: %s -i input -o output\n", argv[0]);
                return 1;
        }
    }

    // Parse non-flagged arguments (the number of bytes)
    for (int i = optind; i < argc; i++) {
        if (nonFlaggedCount >= 1) {
            fprintf(stderr, "Too many arguments. Provide at most one positive integer value for NBYTES");
            return 1;
        }

        nonFlaggedCount++;
        char *endptr;
        int valid = 0;
        errno = 0;
        *nbytes = strtoll (argv[i], &endptr, 10);
        
        if (errno) {
            perror(argv[i]);
        }
        else {
            valid = !*endptr && (0 <= *nbytes);
        }
        if (!valid) {
            fprintf(stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
            return 1;
        }
    }

    // Error Checking...
    // No argument provided
    if (nonFlaggedCount == 0) {
        fprintf(stderr, "Provide a positive integer value for NBYTES (./randall NBYTES)\n");
        return 1;
    }

    // Too many options
    if (inputFlag > 1 || outputFlag > 1) {
        fprintf(stderr, "Too many options provided. Provide at most one -i and one -o\n");
        return 1;
    }

    // Process Input Flag
    if (inputFlag) {
        if (strcmp(inputArg, "rdrand") == 0) {
            *inputChoice = 0;
        }
        else if (strcmp(inputArg, "lrand48_r") == 0) {
            *inputChoice = 1;
        }
        else if (inputArg[0] == '/') {
            FILE *file = fopen(&inputArg[0], "r");
            if (file == NULL) {
                fprintf(stderr, "Failed to open file: %s (No such file or directory)\n", &inputArg[0]);
                return 1;
            }
            
            setFilename(&inputArg[0]);
            fclose(file);
            *inputChoice = 2;
        }
        else {
            fprintf(stderr, "Invalid input argument. Valid arguments are rdrand, mrand48_r, and /F\n");
            return 1;
        }
    }

    // Process Output Flag
    if (outputFlag) {
        if (strcmp(outputArg, "stdio") == 0) {
            *outputChoice = 0;
        }
        else {
            *outputChoice = 1;
            char *endptr2;
            int valid2 = 0;
            *chunksize = strtoll(outputArg, &endptr2, 10);
            if (errno)
                perror(outputArg);
            else
                valid2 = !*endptr2 && (0 <= *nbytes);

            if (!valid2) {
                fprintf(stderr, "Invalid input argument for output (-o). Use stdio or a positive integer N\n");
                return 1;
            }
        }
    }

    return 0;
}