#include <stdio.h>
#include <string.h>
#include "preprocessor.h"

int main(int argc, char *argv[])
{
    char output_name[300];
    char *dot;

    if (argc != 2) {
        printf("Usage: ./mypreprocessor inputfile.c\n");
        return 1;
    }

    if (strlen(argv[1]) + 3 >= sizeof(output_name)) {
        printf("Filename is too long.\n");
        return 1;
    }

    strcpy(output_name, argv[1]);
    dot = strrchr(output_name, '.');
    if (dot != NULL)
        strcpy(dot, ".i");
    else
        strcat(output_name, ".i");

    if (preprocess_file(argv[1], output_name) == 0) {
        printf("Created output file: %s\n", output_name);
        return 0;
    }
    return 1;
}
