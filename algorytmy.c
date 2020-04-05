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

void heapify(int* arr, int n, int i){ 
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
  
void heap_sort(int* arr, int length){ 
    for (int i = length / 2 - 1; i >= 0; i--) {
        heapify(arr, length, i); 
    }
    for (int i = length - 1; i > 0; i--) { 
        swap(&arr[0], &arr[i]); 
        heapify(arr, i, 0); 
    } 
} 
