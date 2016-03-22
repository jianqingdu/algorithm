//
//  shell_sort.c
//  algorithm
//
//  Created by jianqing.du on 16-1-25.
//  Copyright (c) 2016年. All rights reserved.
//

/*
 * implement shell sort in https://en.wikipedia.org/wiki/Shellsort
 */
#include <stdio.h>

#define MAX_ARRAY_SIZE 64

void shell_sort(int array[], int n)
{
    //Start with the largest gap and work down to a gap of 1
    for (int gap = n / 2; gap > 0; gap /= 2) {
        // Do a gapped insertion sort for this gap size.
        // The first gap elements a[0..gap-1] are already in gapped order
        for (int i = gap; i < n; i++) {
            int j = i;
            int tmp = array[i];
            for (; (j >= gap) && (array[i] < array[j - gap]); j -= gap) {
                array[j] = array[j - gap];
            }
            
            array[j] = tmp;
        }
    }
}

int main(int argc, char* argv[])
{
    int array[MAX_ARRAY_SIZE];
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        array[i] = MAX_ARRAY_SIZE - i;
    }
    
    shell_sort(array, MAX_ARRAY_SIZE);
    
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        printf("%d\n", array[i]);
    }
    
    return 0;
}