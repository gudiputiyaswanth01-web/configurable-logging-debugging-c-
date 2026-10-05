#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "file_inclusion.h"

int include_file(const char *line, FILE *output_file)
{
    const char *p = line;
    char end_char, filename[300];
    int i = 0;

    while (isspace((unsigned char)*p)) p++;
    if (strncmp(p, "#include", 8) != 0) return 0;
    p += 8;
    while (isspace((unsigned char)*p)) p++;
    if (*p == '<') end_char = '>';
    else if (*p == '"') end_char = '"';
    else return -1;
    p++;

    while (*p && *p != end_char && i < (int)sizeof(filename) - 1)
        filename[i++] = *p++;
    filename[i] = '\0';
    if (*p != end_char || !filename[0]) return -1;

    FILE *input = fopen(filename, "r");
    if (!input) {
        fprintf(stderr, "Warning: cannot open included file '%s'.\n", filename);
        return -1;
    }
    char buffer[2048];
    while (fgets(buffer, sizeof(buffer), input)) fputs(buffer, output_file);
    fclose(input);
    return 1;
}
