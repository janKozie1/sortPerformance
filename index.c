#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "algorytmy.h"

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printTab(int* tab, int length){
    for(int i = 0; i < length; i++) printf("%d ", tab[i]);
    printf("\n");
}

void getRandomizedData(int* tab, int length){
    for(int i = 0; i < length; i++) tab[i] = rand()%201 - 100;
}

void getOrderedData(int* tab, int length){
   for(int i = 0; i < length; i++) tab[i] = i;
}

void getReversedData(int* tab, int length){
    for(int i = 0; i < length; i++) tab[length - 1 - i] = i;
}

int isSorted(int* tab, int length){
    for(int i = 0; i < length - 1; i++) if( tab[i] >  tab[i+1]) return 0;
    return 1;
}

void printProgress(int current, int max){
    printf("\r\tSorting: ");
    int done = (double)current / max * 10;
    for(int i = 0; i < done; i++){
        printf("#");
    }
    for(int i = done; i < 10; i++){
        printf("_");
    }
}

void printToFileAndConsole(FILE* f, char* header){
    fprintf(f,"%s", header);
    printf("%s", header);
}

void measure(int baseLength, int max_checks, char* name, FILE* formatted, FILE* raw, void (*sort)(int* tab, int length), void (*data)(int* tab, int length)){
    int i;
    int* tab;
    struct timeval start, end;

    printToFileAndConsole(formatted, "\t");
    printToFileAndConsole(formatted, name);
    printToFileAndConsole(formatted, ":\n");

    for(i = 1; i < max_checks; i++){
        int length = baseLength * i;
        tab = (int*)malloc(sizeof(int) * length);

        fprintf(formatted, "\t%15d elements - ", length);
        fprintf(raw, "%d,", length);

        (*data)(tab, length);
        gettimeofday(&start, NULL);
        (*sort)(tab, length);
        gettimeofday(&end, NULL);
        free(tab);

        printProgress(i, max_checks);
        
        long timeElapsed =  (end.tv_usec - start.tv_usec);
        fprintf(formatted, "%15ld us\n", timeElapsed);
        fprintf(raw, "%ld\n", timeElapsed);
    }
    printProgress(max_checks, max_checks);
}

void perf(int baseLength, int max_checks, FILE* formatted, FILE* raw, char* name, void (*sort)(int* tab, int length)){
    printToFileAndConsole(formatted, "\n---");
    printToFileAndConsole(formatted, name);
    printToFileAndConsole(formatted, "---\n");
   
    measure(baseLength, max_checks, "Randomized data", formatted, raw, sort, getRandomizedData);
    printToFileAndConsole(raw, "\n");
    measure(baseLength, max_checks, "Reversed data", formatted, raw, sort, getReversedData);
    printToFileAndConsole(raw, "\n");
    measure(baseLength, max_checks, "Ordered data", formatted, raw, sort, getOrderedData);
    printToFileAndConsole(raw, "\n");
    fprintf(raw, "\n");
}

int main(){
    srand(time(NULL));

    int baseLength;
    int max;
    int* tab;
    
    FILE *formatted = fopen("formatted_output.txt", "w+");
    FILE *raw = fopen("data.txt", "w+");

    printf("Input base length: ");
    scanf("%d", &baseLength);

    printf("Input how many sorts to perform: ");
    scanf("%d", &max);
    
    
    fprintf(formatted, "Testing for starting length = %d\n", baseLength);
    perf(baseLength, max, formatted, raw, "Insertion sort", insertion_sort);
    perf(baseLength, max, formatted, raw, "Selection sort", selection_sort);
    perf(baseLength, max, formatted, raw, "Bubble sort", bubble_sort);
    perf(baseLength, max, formatted, raw, "Quick sort", quick_sort_wrapper);
    perf(baseLength, max, formatted, raw, "Shell sort", shell_sort_wrapper);
    perf(baseLength, max, formatted, raw, "Heap sort", heap_sort);
}
