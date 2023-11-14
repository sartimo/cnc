#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TAB 8  /* assumption about the terminal */

void cycle_color(int d, int low, int high, int *r, int *g, int *b, int *s) {
    switch (*s) {
        case 0: if ((*g += d) >= high) { *g = high; (*s)++; }; break;
        case 1: if ((*r -= d) <= low ) { *r = low;  (*s)++; }; break;
        case 2: if ((*b += d) >= high) { *b = high; (*s)++; }; break;
        case 3: if ((*g -= d) <= low ) { *g = low;  (*s)++; }; break;
        case 4: if ((*r += d) >= high) { *r = high; (*s)++; }; break;
        case 5: if ((*b -= d) <= low ) { *b = low;  *s = 0; }; break;
    }
}

void write_color(int r, int g, int b) {
    printf("\x1b[38;2;%d;%d;%dm", r, g, b);
}

int main() {
    const char *text = "Hello, Rainbow Text!";
    int lo = 80, hi = 255, dx = 10, dy = 30, off = -1;

    if (off < 0) srand(time(0)), off = rand() % 101;

    int R = hi, G = lo, B = lo, S = 0;
    while (off--) cycle_color(16, lo, hi, &R, &G, &B, &S);

    int x = 0, r = R, g = G, b = B, s = S;

    for (size_t i = 0; i < strlen(text); ++i) {
        char c = text[i];

        if (c == '\n') {
            putchar(c);
            cycle_color(dy, lo, hi, &R, &G, &B, &S);
            x = 0; r = R; g = G; b = B; s = S;
        } else if (c == '\t') {
            cycle_color(dx, lo, hi, &r, &g, &b, &s);
            write_color(r, g, b);
            putchar(c);
            while (++x % TAB)
                cycle_color(dx, lo, hi, &r, &g, &b, &s);
        } else if (c < 128 || (c & 64)) {  /* ascii7 or utf8 leading byte */
            cycle_color(dx, lo, hi, &r, &g, &b, &s);
            write_color(r, g, b);
            putchar(c);
            x++;
        } else {  /* utf8 continuation byte */
            putchar(c);
        }
    }

    printf("\x1b[0m");  // Reset colors
    return 0;
}
