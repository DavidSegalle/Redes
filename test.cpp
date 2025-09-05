#include <stdio.h>

// Define a union with 
// different data types
union Student {
    int rollNo;
    float height;
    char firstLetter;
};

int main() {
    
    char* teste[4];

    for(int i =0; i < 4; i++){
        teste[i] = 0;
    }

    printf("%d", teste[0]);

    return 0;
}