#include <stdio.h>

// Define a union with 
// different data types
union Student {
    int rollNo;
    float height;
    char firstLetter;
};

#include <stdio.h>
#include <stdint.h>
#include <string.h>

void printBinary(uint32_t n) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i == 31 || i == 23) printf(" "); // separate sign, exponent, mantissa
    }
    printf("\n");
}

int main() {
    int teste = 3;

    printf("%b", teste);
}