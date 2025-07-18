#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


int read_codepoint(const char *str, int *bytes_used) {
    unsigned char byte = str[0];
    int codepoint = 0;
    if ((byte & 0x80) == 0) {
        codepoint = byte;
        *bytes_used = 1;
    } else if ((byte & 0xE0) == 0xC0) {
        codepoint = ((str[0] & 0x1F) << 6) | (str[1] & 0x3F);
        *bytes_used = 2;
    } else if ((byte & 0xF0) == 0xE0) {
        codepoint = ((str[0] & 0x0F) << 12) | ((str[1] & 0x3F) << 6) | (str[2] & 0x3F);
        *bytes_used = 3;
    } else if ((byte & 0xF8) == 0xF0) {
        codepoint = ((str[0] & 0x07) << 18) | ((str[1] & 0x3F) << 12) |
                    ((str[2] & 0x3F) << 6) | (str[3] & 0x3F);
        *bytes_used = 4;
    } else {
        *bytes_used = 1;
        return -1;
    }
    return codepoint;
}

bool is_animal_emoji(int codepoint) {
    return (codepoint >= 0x1F400 && codepoint <= 0x1F43F) ||
           (codepoint >= 0x1F980 && codepoint <= 0x1F9AE);
}

void print_codepoints_and_lengths(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
	int bytes = 0;
        int codepoint = read_codepoint(&str[i], &bytes);
        printf("%d ", codepoint);
        i += bytes;
    }
    printf("\n");
}

void print_bytes_per_codepoint(const char *str) {
    printf("Bytes per code point:\n");
    int i = 0;
    while (str[i] != '\0') {
        int bytes = 0;
        read_codepoint(&str[i], &bytes);
        printf("%d ", bytes);
        i += bytes;
    }
    printf("\n");
}

void print_animal_emojis(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        int bytes = 0;
        int cp = read_codepoint(&str[i], &bytes);
        if (is_animal_emoji(cp)) {
            for (int j = 0; j < bytes; j++) {
                printf("%c", str[i + j]);
            }
        }
        i += bytes;
    }
    printf("\n");
}

void print_codepoint_plus_one_at_index3(const char *str) {
    int i = 0, index = 0;
    while (str[i] != '\0') {
        int bytes = 0;
        int cp = read_codepoint(&str[i], &bytes);
        if (index == 3) {
            cp += 1;
            if (cp <= 0x7F) {
                printf("%c\n", cp);
            } else if (cp <= 0x7FF) {
                printf("%c%c\n", 0xC0 | (cp >> 6), 0x80 | (cp & 0x3F));
            } else if (cp <= 0xFFFF) {
                printf("%c%c%c\n", 0xE0 | (cp >> 12), 0x80 | ((cp >> 6) & 0x3F), 0x80 | (cp & 0x3F));
            } else {
                printf("%c%c%c%c\n", 0xF0 | (cp >> 18), 0x80 | ((cp >> 12) & 0x3F),
                                     0x80 | ((cp >> 6) & 0x3F), 0x80 | (cp & 0x3F));
            }
            return;
        }
        i += bytes;
        index++;
    }
    printf("(Index 3 not found)\n");
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

void print_first_six_codepoints(const char *str) {
    int i = 0, count = 0;
    while (str[i] != '\0' && count < 6) {
        int bytes = 0;
        int cp = read_codepoint(&str[i], &bytes);
        for (int j = 0; j < bytes; j++) {
            printf("%c", str[i + j]);
        }
        i += bytes;
        count++;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }

    const char *input = argv[1];

    printf("Valid ASCII: %s\n", is_valid_ascii(input) ? "true" : "false");

    printf("Uppercased ASCII: ");
    print_uppercase(input);

    printf("Length in bytes: %d\n", count_bytes(input));

    printf("Number of code points: %d\n", count_codepoints(input));

    printf("Code points as decimal numbers: ");
    print_codepoints_and_lengths(input);

    printf("Bytes per code point: ");
    print_bytes_per_codepoint(input);

    printf("Substring of the first 6 code points: ");
    print_first_six_codepoints(input);

    printf("Animal emojis: ");
    print_animal_emojis(input);

    printf("Codepoint+1 at index 3: ");
    print_codepoint_plus_one_at_index3(input);

    return 0;
}
