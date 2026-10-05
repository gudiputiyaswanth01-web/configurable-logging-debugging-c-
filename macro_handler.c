#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "macro_handler.h"

static Macro macros[MAX_MACROS];
static int macro_count = 0;

static int name_char(char c)
{
    return isalnum((unsigned char)c) || c == '_';
}

void clear_macros(void)
{
    macro_count = 0;
}

int store_macro(const char *line)
{
    char name[NAME_SIZE] = "", value[VALUE_SIZE] = "", arg[NAME_SIZE] = "";
    const char *p = line;
    int n = 0, has_arg = 0;

    while (isspace((unsigned char)*p)) p++;
    if (strncmp(p, "#define", 7) != 0) return 0;
    p += 7;
    while (isspace((unsigned char)*p)) p++;
    while (name_char(*p) && n < NAME_SIZE - 1) name[n++] = *p++;
    name[n] = '\0';
    if (!name[0]) return -1;

    if (*p == '(') {
        has_arg = 1;
        p++;
        n = 0;
        while (name_char(*p) && n < NAME_SIZE - 1) arg[n++] = *p++;
        arg[n] = '\0';
        if (*p != ')') return -1;
        p++;
    }
    while (isspace((unsigned char)*p)) p++;
    snprintf(value, sizeof(value), "%s", p);
    value[strcspn(value, "\r\n")] = '\0';

    for (int i = 0; i < macro_count; i++) {
        if (strcmp(macros[i].name, name) == 0) {
            snprintf(macros[i].value, sizeof(macros[i].value), "%s", value);
            snprintf(macros[i].argument, sizeof(macros[i].argument), "%s", arg);
            macros[i].has_argument = has_arg;
            return 1;
        }
    }
    if (macro_count >= MAX_MACROS) return -1;
    snprintf(macros[macro_count].name, NAME_SIZE, "%s", name);
    snprintf(macros[macro_count].value, VALUE_SIZE, "%s", value);
    snprintf(macros[macro_count].argument, NAME_SIZE, "%s", arg);
    macros[macro_count].has_argument = has_arg;
    macro_count++;
    return 1;
}

void substitute_macros(const char *line, char *output, int output_size)
{
    int i = 0, used = 0;
    output[0] = '\0';

    while (line[i] != '\0' && used < output_size - 1) {
        int found = 0;
        if (isalpha((unsigned char)line[i]) || line[i] == '_') {
            for (int m = 0; m < macro_count; m++) {
                int len = (int)strlen(macros[m].name);
                if (strncmp(line + i, macros[m].name, len) == 0 &&
                    (i == 0 || !name_char(line[i - 1])) &&
                    !name_char(line[i + len])) {
                    if (macros[m].has_argument) {
                        int k = i + len, start, depth = 0;
                        char actual[VALUE_SIZE], replacement[VALUE_SIZE * 2] = "";
                        while (isspace((unsigned char)line[k])) k++;
                        if (line[k] != '(') continue;
                        start = ++k;
                        while (line[k] != '\0') {
                            if (line[k] == '(') depth++;
                            if (line[k] == ')' && depth == 0) break;
                            if (line[k] == ')') depth--;
                            k++;
                        }
                        if (line[k] != ')') continue;
                        int alen = k - start;
                        if (alen >= VALUE_SIZE) alen = VALUE_SIZE - 1;
                        memcpy(actual, line + start, alen);
                        actual[alen] = '\0';

                        const char *v = macros[m].value;
                        while (*v && strlen(replacement) < sizeof(replacement) - 2) {
                            int al = (int)strlen(macros[m].argument);
                            if (al > 0 && strncmp(v, macros[m].argument, al) == 0 &&
                                (v == macros[m].value || !name_char(v[-1])) &&
                                !name_char(v[al])) {
                                strncat(replacement, actual, sizeof(replacement) - strlen(replacement) - 1);
                                v += al;
                            } else {
                                int rlen = (int)strlen(replacement);
                                replacement[rlen] = *v++;
                                replacement[rlen + 1] = '\0';
                            }
                        }
                        int rlen = (int)strlen(replacement);
                        if (used + rlen < output_size) {
                            memcpy(output + used, replacement, rlen);
                            used += rlen;
                        }
                        i = k + 1;
                    } else {
                        int vlen = (int)strlen(macros[m].value);
                        if (used + vlen < output_size) {
                            memcpy(output + used, macros[m].value, vlen);
                            used += vlen;
                        }
                        i += len;
                    }
                    output[used] = '\0';
                    found = 1;
                    break;
                }
            }
        }
        if (!found) output[used++] = line[i++];
        output[used] = '\0';
    }
}
