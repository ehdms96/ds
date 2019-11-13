#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define _CRT_SECURE_NO_WARNINGS
#define SWAP(x, y, t) ((t) = (x), (x) = (y), (y) = (t))
void sort(int [], int);
int main(void){
    int i, j, n;
        
    FILE *fp1 = fopen("1_1test3.txt", "r");
    FILE *fp2 = fopen("1_1test3_out.txt", "w");
    fscanf(fp1, "%d", &n);
    int *list = malloc(sizeof(int)*n);

    for(i=0; i<n; i++){
        fscanf(fp1, "%d", &list[i]);
    } 
    
    sort(list,n);

    for(j=0; j<n; j++){
        fprintf(fp2, "%d ", list[j]);
    }

    for(i=0; i<n; i++)
        printf("%d ", list[i]);
    printf("\n");

    fclose(fp1);
    fclose(fp2);

    return 0;
    }

void sort(int list[], int n){
    int i, j, min, temp;
    for(i=0; i<n-1; i++) {
        min = i;
        for(j = i+1; j<n; j++)
            if (list[j] < list[min])
                min = j;
        SWAP(list[i], list[min], temp);
    }
}
