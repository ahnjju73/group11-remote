#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }

    // implement the UTF-8 analyzer here

}
bool is_valid_ascii(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if ((unsigned char)str[i] > 127) return false;
    }
    return true;
}


void print_uppercase(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if ('a' <= c && c <= 'z') {
            printf("%c", c - 32);
        } else {
            printf("%c", c);
        }
    }
    printf("\n");
}


int count_bytes(const char *str) {
    return strlen(str);
}


int count_codepoints(const char *str) {
    int count = 0;
    for (int i = 0; str[i] != '\0';) {
        unsigned char byte = str[i];
        if ((byte & 0x80) == 0) i += 1;
        else if ((byte & 0xE0) == 0xC0) i += 2;
        else if ((byte & 0xF0) == 0xE0) i += 3;
        else if ((byte & 0xF8) == 0xF0) i += 4;
        else return -1; // invalid UTF-8
        count++;
    }
    return count;
}
