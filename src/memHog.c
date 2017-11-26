#include <stdio.h>
#include <stdlib.h>


int main() {
    double *hog0, *hog1;
    hog0 = (int *)malloc(sizeof(double) * 1000000);
    hog1 = (int *)malloc(sizeof(double) * 1000000);
    double hog2[1000000] = {0.0};
    


    if (hog0 == NULL || hog1 == NULL || hog2 == NULL) {
        printf("Malloc failed!\n");
        exit(1);
    }
    int i = 0;
    while (1) {
        if (i >= 1000000) { i = 0; }
        hog0[i] = hog1[i] + hog2[i];
        hog1[i]++;
        hog2[i]++;
    }
}