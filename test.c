#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <sys/time.h>
#include "ca.h"

int main() {
    int size = 50;
    int data[size * size];
    srand(time(0) + getpid());
    for (int i = 0; i < size * size; i++)
        data[i] = rand() % 2;
    
    ca_state * ca = ca_create(size, size, data, 8, 12); // GoL rule

    for (int it = 0; it < 20; it++) {
        ca_iterate(ca, 1);
        ca_copy_data(ca, data);
        for (int j = 0; j < size; j++) {
            for (int i = 0; i < size; i++)
                printf("%c", data[j * size + i] ? '#' : ' ');
            printf("\n");
        }
        for (int i = 0; i < size; i++)
            printf("-");
        printf("\n");
    }
    
    ca_free(ca);
    return 0;
}
