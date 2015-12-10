//
//  merge_sort.c
//  algorithm
//
//  Created by jianqing.du on 15-12-10.
//  Copyright (c) 2015年 mgj. All rights reserved.
//

/*
 * implement merge sort in <<Introduction to Algorithm>> 3rd Edition, chapter 2.3
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void merge(int array[], int start, int middle, int end)
{
    int n1 = middle - start + 1;
    int n2 = end - middle;
    int i, j, k;
    
    int* left = malloc((n1 + 1) * sizeof(int));
    int* right = malloc((n2 + 1) * sizeof(int));
    
    // copy to 2 array
    for (i = 0; i < n1; i++) {
        left[i] = array[i + start];
    }
    left[n1] = INT32_MAX;
    
    for (i = 0; i < n2; i++) {
        right[i] = array[i + middle + 1];
    }
    right[n2] = INT32_MAX;
    
    // merge 2 array
    i = 0;
    j = 0;
    for (k = start; k <= end; k++) {
        if (left[i] < right[j]) {
            array[k] = left[i++];
        } else {
            array[k] = right[j++];
        }
    }
    
    free(left);
    free(right);
}

void merge_sort(int array[], int start, int end)
{
    if (start < end) {
        int middle = (start + end) / 2;
        merge_sort(array, start, middle);
        merge_sort(array, middle + 1, end);
        
        merge(array, start, middle, end);
    }
}

#define MAX_ARRAY_SIZE  100

int main(int argc, char* argv[])
{
    int array[MAX_ARRAY_SIZE];
    
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        array[i] = MAX_ARRAY_SIZE - i;
    }
    
    merge_sort(array, 0, MAX_ARRAY_SIZE - 1);
    
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        printf("%d\n", array[i]);
    }
    
    return 0;
}
