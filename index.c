#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
    fprintf(f, header);
    printf(header);
}

void measure(int baseLength, int max_checks, char* name, FILE* formatted, FILE* raw, void (*sort)(int* tab, int length), void (*data)(int* tab, int length)){
    int i;
    int* tab;
    long double start, end;

    printToFileAndConsole(formatted, "\t");
    printToFileAndConsole(formatted, name);
    printToFileAndConsole(formatted, ":\n");

    for(i = 1; i < max_checks; i++){
        int length = baseLength * i;
        tab = (int*)malloc(sizeof(int) * length);

        fprintf(formatted, "\t%15d elements - ", length);
        fprintf(raw, "%d,", length);
    gettimeof
        (*data)(tab, length);
        start = time(NULL);
        (*sort)(tab, length);
        end = time(NULL);
        free(tab);

        printProgress(i, max_checks);
        
        long double timeElapsed =  (end-start) * 1000;
        fprintf(formatted, "%15f ms\n", timeElapsed);
        fprintf(raw, "%f\n", timeElapsed);
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

void insertion_sort(int* tab, int length){
    int i = 0;
    while(i < length){
        int j = i;
        while(j > 0 && tab[j - 1] > tab[j]){
            swap(&tab[j], &tab[j - 1]);
            j--;
        }
        i++;
    }
}

void selection_sort(int* tab, int length){
    int i = 0;
    while(i < length){
        int j = i;
        int* currentMinimum = &tab[i];
        while(j < length){
            if(*currentMinimum > tab[j]) currentMinimum = &tab[j];
            j++;
        }
        swap(currentMinimum, &tab[i]);
        i++;
    }
}

void bubble_sort(int* tab, int length){
    int i = 0;
    while(i < length){
        int j = 0;
        while(j < length - i - 1){
            if(tab[j] > tab[j+1]) swap(&tab[j], &tab[j+1]);
            j++;
        }
        i++;
    }
}

int get_pivot_with_partition(int* tab, int s, int f){
    int pivot = tab[f];
    int swapIndex = s;

    for(int i = s; i < f; i++){
        if(tab[i] <= pivot){
            swap(&tab[i], &tab[swapIndex]);
            swapIndex++;
        }
    }
    swap(&tab[swapIndex], &tab[f]);
    return swapIndex;
}

void quick_sort(int* tab, int s, int f){
    if(s < f){
        int pivot = get_pivot_with_partition(tab, s, f);
        quick_sort(tab, s, pivot -1);
        quick_sort(tab, pivot + 1, f);
    }
}

void quick_sort_wrapper(int* tab, int length){
    quick_sort(tab, 0, length - 1);
}

void shell_sort(int* tab, int length, int gap){
    if(!gap) return;
    for(int i = 0; i < length; i += gap){
        int j = i;
        while(j - gap >=  0 && tab[j] < tab[j - gap]){
            swap(&tab[j], &tab[j - gap]);
            j -= gap;
        }
    }
    shell_sort(tab, length, gap/2);
}

void shell_sort_wrapper(int* tab, int length){
    shell_sort(tab, length, length / 2);
}

void heapify(int* arr, int n, int i) { 
    int currMax = i; 
    int left = 2 * i + 1;
    int right = 2 * i + 2; 


    if(left < n) {
        if(arr[left] > arr[currMax]) currMax = left;
        if(right < n && arr[right] > arr[currMax]) currMax = right;
    }
    
    if (currMax != i) { 
        swap(&arr[i], &arr[currMax]); 
        heapify(arr, n, currMax); 
    } 
} 
  
void heap_sort(int* arr, int length) { 
    for (int i = length / 2 - 1; i >= 0; i--) {
        heapify(arr, length, i); 
    }
    for (int i = length - 1; i > 0; i--) { 
        swap(&arr[0], &arr[i]); 
        heapify(arr, i, 0); 
    } 
} 

int main(){
    system("CLS");
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