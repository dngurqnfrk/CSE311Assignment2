#pragma once
#include "input.h"

template <typename T>
class heapSort {
    private:
        int heap_size;
        bool (*comparator) (T i, T j);
    public:
        heapSort(bool (*_comparator) (T i, T j), int size = 0);
        ~heapSort() {}
    
        void sort(std::vector<T> &arr);
        void build_max_heap(std::vector<T> &arr, int start, int end);
        void max_heapify(std::vector<T> &arr, int index, int start, int end);
        void min_heapify(std::vector<T> &arr, int index, int start);
        void heap_sort(std::vector<T> &arr, int start, int end);
        void heap_index_sort(std::vector<T> &arr, int start, int end);
        void heap_decrease_key(std::vector<T> &arr, int index, double key, void (*set)(T, double));
        T pop(std::vector<T> &arr);
};

template <typename T>
void swap(std::vector<T> &arr, int i, int j) {
	T temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

template <typename T>
heapSort<T>::heapSort(bool (*_comparator) (T i, T j), int size) 
 : comparator(_comparator), heap_size(size) {}

template <typename T>
void heapSort<T>::sort(std::vector<T> &arr) {
    heap_size = arr.size();
    if (heap_size < 2)
        return;

    heap_index_sort(arr, 0, arr.size() - 1);
}

template <typename T>
void heapSort<T>::build_max_heap(std::vector<T> &arr, int start, int end) {
    for (int i = start + heap_size / 2; i >= start; i--)
        max_heapify(arr, i, start, end);
}

template <typename T>
void heapSort<T>::max_heapify(std::vector<T> &arr, int index, int start, int end) {
    int left = (index - start) * 2 + start + 1;
    if (arr.size() <= left)
        return;

    int right = (index - start) * 2 + start + 2;
    
    int bigger_child = index;
    if ((left < heap_size + start) && comparator(arr[left], arr[right]))
        bigger_child = left;
    
    if (right < heap_size + start && comparator(arr[right], arr[bigger_child]))
        bigger_child = right;

    if (bigger_child != index) {
        swap(arr, index, bigger_child);
        max_heapify(arr, bigger_child, start, end);
    }
}

template <typename T>
void heapSort<T>::heap_sort(std::vector<T> &arr, int start, int end) {
    for(int i = end; i > start; i--) {
        heap_size--;
        swap(arr, start, i);
        max_heapify(arr, start, start, end);
    }
}

template <typename T>
void heapSort<T>::heap_index_sort(std::vector<T> &arr, int start, int end) {
    heap_size = end - start + 1;

    build_max_heap(arr, start, end);
    heap_sort(arr, start, end);
}

template <typename T>
void heapSort<T>::heap_decrease_key(std::vector<T> &arr, int index, double key, void (*set)(T, double)) {    
    set(arr[index], key);

    min_heapify(arr, index, 0);
}

template <typename T>
void heapSort<T>::min_heapify(std::vector<T> &arr, int index, int start) {
    while(index > start && comparator(arr[(index - start - 1) % 2 + start], arr[index])) {
        swap(arr, index, (index - start - 1) % 2 + start);
        index = (index - start - 1) % 2 + start;
    }
}

template <typename T>
T heapSort<T>::pop(std::vector<T> &arr) {
    T temp = arr[0];
    swap(arr, 0, arr.size() - 1);
    arr.pop_back();
    heap_size = arr.size();
    min_heapify(arr, 0, 0);

    return temp;
}