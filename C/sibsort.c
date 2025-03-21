#include <stdlib.h>
#include <string.h>
#include "sibsort.h"

#define SMALLEST_INCLUDED_GAP 3

int sibsort(void*, size_t, size_t, int (*)(void*, void*));
int insertion(void*, size_t, size_t, int (*)(void*, void*));

char* index_value;

/*
Arguments for both algorithms (compare with qsort()):
arr: array to sort
length: number of elements in the array
size: size of each element in bytes
cmp: user-defined compare function (*arg1 > *arg2 for ascending order, *arg1 < *arg2 for descending order)*/

int sibsort(void* arr, size_t length, size_t size, int (*cmp)(void*, void*)) {
	if (arr == NULL || !length || !size || (index_value = (char*)malloc(size)) == NULL) return -1;
	char* array = (char*)arr;
	size_t gap = length;
	while (gap > SMALLEST_INCLUDED_GAP) {
		gap *= .75;
		if (gap % 2 == 0) {
			gap--;
		}
		for (size_t index = 0; index < length - gap; index += 2) {
			char* index_low = array + index * size;
			char* index_high = index_low + gap * size;
			if (cmp(index_low, index_high)) {
				memcpy(index_value, index_low, size);
				memcpy(index_low, index_high, size);
				memcpy(index_high, index_value, size);
			}
		}
		for (size_t index = 1; index < length - gap; index += 2) {
			char* index_low = array + index * size;
			char* index_high = index_low + gap * size;
			if (cmp(index_low, index_high)) {
				memcpy(index_value, index_low, size);
				memcpy(index_low, index_high, size);
				memcpy(index_high, index_value, size);
			}
		}
	}
	//no need to free the buffer here, insertion() will later
	return insertion(array, length, size, cmp);
}

int insertion(void* arr, size_t length, size_t size, int (*cmp)(void*, void*)) {
	//if called by itself, give a chance to allocate the temporary buffer
	if (index_value == NULL) index_value = (char*) malloc(size);
	if (arr == NULL || !length || !size || index_value == NULL) return -1;
	char* array = (char*)arr;
	for (size_t index = 1; index < length; index++) {
		char* pointer = array + index * size;
		memcpy(index_value, pointer, size);
		while (pointer > array && cmp(pointer - size, index_value)) {
			memcpy(pointer, pointer - size, size);
			pointer -= size;
		}
		memcpy(pointer, index_value, size);
	}
	free(index_value);
	return 0;
}