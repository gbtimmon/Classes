#include "eString.h"

char *get_line(FILE* fp)
{
    int len = 0, got = 0, c;
    char *buf = 0;
    while ((c = fgetc(fp)) != EOF) {
        if (got + 1 >= len) {
            len *= 2;
            if(len <= 4) len = 4;
            buf = realloc(buf, len);
        }
        buf[got++] = c;
        if (c == '\n' ) break;
    }
    if (c == EOF && !got) return 0;
    buf[got++] = '\0';
    return buf;
}


char** split(const char* str, const char* delim, size_t* numtokens) {
    char *s = strdup(str);
    size_t tokens_alloc = 1;
    size_t tokens_used = 0;
    char **tokens = calloc(tokens_alloc, sizeof(char*));
    char *token, *rest = s;

    while ((token = strsep(&rest, delim)) != NULL) {
        if (tokens_used == tokens_alloc) {
            tokens_alloc *= 2;
            tokens = realloc(tokens, tokens_alloc * sizeof(char*));
        }
        tokens[tokens_used++] = strdup(token);
    }

    if (tokens_used == 0) {
        free(tokens);
        tokens = NULL;
    } else {
        tokens = realloc(tokens, tokens_used * sizeof(char*));
    }
    *numtokens = tokens_used;
    free(s);

    return tokens;
}

char* chomp(char* str){
    int i = strlen(str);
    if( str[i-1] == '\n' )
        str[i-1]  = '\0';
    return str;
}

char *strdup(const char *str)
{
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if(dup)
    {
        strcpy(dup, str);
    }
    return dup;
}
