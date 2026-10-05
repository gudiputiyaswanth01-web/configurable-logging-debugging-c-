#ifndef MACRO_HANDLER_H
#define MACRO_HANDLER_H
#define MAX_MACROS 100
#define NAME_SIZE 80
#define VALUE_SIZE 300
typedef struct {
    char name[NAME_SIZE];
    char value[VALUE_SIZE];
    char argument[NAME_SIZE];
    int has_argument;
} Macro;
void clear_macros(void);
int store_macro(const char *line);
void substitute_macros(const char *line, char *output, int output_size);
#endif
