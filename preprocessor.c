#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "preprocessor.h"
#include "comment_removal.h"
#include "macro_handler.h"
#include "file_inclusion.h"

int preprocess_file(const char *input_name, const char *output_name)
{
    FILE *input = fopen(input_name, "r");
    if (!input) {
        perror("Could not open input file");
        return 1;
    }
    FILE *output = fopen(output_name, "w");
    if (!output) {
        perror("Could not create output file");
        fclose(input);
        return 1;
    }

    char line[4096], clean[4096], expanded[20000];
    int in_block_comment = 0;
    clear_macros();

    while (fgets(line, sizeof(line), input)) {
        remove_comments(line, clean, &in_block_comment);
        const char *p = clean;
        while (isspace((unsigned char)*p)) p++;

        if (strncmp(p, "#define", 7) == 0) {
            if (store_macro(p) < 0)
                fprintf(stderr, "Warning: invalid or excess macro: %s", p);
            continue;
        }
        int inc = include_file(p, output);
        if (inc != 0) {
            if (inc < 0) fputs(clean, output);
            continue;
        }
        substitute_macros(clean, expanded, sizeof(expanded));
        fputs(expanded, output);
        if (!strchr(clean, '\n')) fputc('\n', output);
    }

    fclose(input);
    fclose(output);
    return 0;
}
