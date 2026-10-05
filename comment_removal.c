#include <string.h>
#include "comment_removal.h"

void remove_comments(const char *line, char *output, int *in_block_comment)
{
    int i = 0, j = 0, in_string = 0, in_char = 0;

    while (line[i] != '\0') {
        if (*in_block_comment) {
            if (line[i] == '*' && line[i + 1] == '/') {
                *in_block_comment = 0;
                i += 2;
            } else {
                i++;
            }
        } else if (!in_string && !in_char &&
                   line[i] == '/' && line[i + 1] == '*') {
            *in_block_comment = 1;
            i += 2;
        } else if (!in_string && !in_char &&
                   line[i] == '/' && line[i + 1] == '/') {
            break;
        } else {
            output[j++] = line[i];
            if (line[i] == '"' && (i == 0 || line[i - 1] != '\\') && !in_char)
                in_string = !in_string;
            else if (line[i] == '\'' && (i == 0 || line[i - 1] != '\\') && !in_string)
                in_char = !in_char;
            i++;
        }
    }
    output[j] = '\0';
}
