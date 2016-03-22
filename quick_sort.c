//
//  quick_sort.c
//  algorithm
//
//  Created by jianqing.du on 15-12-10.
//  Copyright (c) 2015年. All rights reserved.
//

/*
 * implement quick sort in <<Introduction to Algorithm>> 3rd Edition, chapter 7
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

uint64_t get_tick_count()
{
    struct timeval tval;
	uint64_t ret_tick;
    
	gettimeofday(&tval, NULL);
    
	ret_tick = tval.tv_sec * 1000L + tval.tv_usec / 1000L;
	return ret_tick;
}

int partition(int array[], int start, int end)
{
    int pivot = array[end];
    int i = start - 1;
    
    for (int j = start; j < end; j++) {
        if (array[j] <= pivot) {
            int tmp = array[j];
            array[j] = array[i + 1];
            array[i + 1] = tmp;
            i++;
        }
    }
    
    int tmp = array[end];
    array[end] = array[i + 1];
    array[i + 1] = tmp;
    
    return i + 1;
}

void quick_sort(int array[], int start, int end)
{
    if (start < end) {
        int pos = partition(array, start, end);
        
        quick_sort(array, start, pos - 1);
        quick_sort(array, pos + 1, end);
    }
}

int randomize_partition(int array[], int start, int end)
{
    int i = rand() % (end - start + 1) + start;
    if (i != end) {
        int tmp = array[end];
        array[end] = array[i];
        array[i] = tmp;
    }
    
    return partition(array, start, end);
}

void randomize_quick_sort(int array[], int start, int end)
{
    if (start < end) {
        int pos = randomize_partition(array, start, end);
        
        randomize_quick_sort(array, start, pos - 1);
        randomize_quick_sort(array, pos + 1, end);
    }
}

#define MAX_ARRAY_SIZE  1000

int main(int argc, char* argv[])
{
    int array[MAX_ARRAY_SIZE];
    uint64_t start_tick, end_tick;
    
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        array[i] = MAX_ARRAY_SIZE - i;
    }
    
    start_tick = get_tick_count();
    quick_sort(array, 0, MAX_ARRAY_SIZE - 1);
    end_tick = get_tick_count();
    printf("quicksort cost=%llu\n", end_tick - start_tick);
    
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        array[i] = MAX_ARRAY_SIZE - i;
    }
    
    start_tick = get_tick_count();
    randomize_quick_sort(array, 0, MAX_ARRAY_SIZE - 1);
    end_tick = get_tick_count();
    printf("random quicksort cost=%llu\n", end_tick - start_tick);
    
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        printf("%d\n", array[i]);
    }
    
    return 0;
}
