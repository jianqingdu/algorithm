//
//  heap_sort.c
//  algorithm
//
//  Created by jianqing.du on 15-12-11.
//  Copyright (c) 2015年. All rights reserved.
//

#include <stdio.h>

/*
 * implement heap sort in <<Introduction to Algorithm>> 3rd Edition, chapter 6
 */
 

// different with the book, cause c array is start from 0
int parent(int i)
{
    return (i - 1) / 2;
}

int left(int i)
{
    return 2 * i + 1;
}

int right(int i)
{
    return 2 * i + 2;
}

void max_heapify(int array[], int heap_size, int i)
{
    int largest = i;
    int l = left(i);
    int r = right(i);
    
    if ((l < heap_size) && (array[i] < array[l])) {
        largest = l;
    }
    
    if ((r < heap_size) && (array[largest] < array[r])) {
        largest = r;
    }
    
    if (largest != i) {
        int tmp = array[i];
        array[i] = array[largest];
        array[largest] = tmp;
        
        max_heapify(array, heap_size, largest);
    }
}

void build_max_heap(int array[], int length)
{
    int heap_size = length;
    for (int i = heap_size / 2; i >= 0; i--) {
        max_heapify(array, heap_size, i);
    }
}

void heap_sort(int array[], int length)
{
    build_max_heap(array, length);
    
    for (int i = length - 1; i > 0; i--) {
        int tmp = array[0];
        array[0] = array[i];
        array[i] = tmp;
        
        max_heapify(array, i, 0);
    }
}

#define MAX_ARRAY_SIZE 16

int main(int argc, char* argv[])
{
    int array[MAX_ARRAY_SIZE];
    
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        array[i] = MAX_ARRAY_SIZE - i;
    }
    
    heap_sort(array, MAX_ARRAY_SIZE);
    
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        printf("%d\n", array[i]);
    }
}