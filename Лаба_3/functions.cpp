
#include "functions.h"

bool try_mark_element(std::vector<int>& arr, unsigned int index, int marker_number) {
    if (arr[index] == 0) {
        arr[index] = marker_number;
        return true;
    }
    return false;
}

unsigned int count_marked_elements(const std::vector<int>& arr, int marker_number) {
    unsigned int count = 0;
    for (int val : arr) {
        if (val == marker_number) {
            count++;
        }
    }
    return count;
}

void reset_marked_elements(std::vector<int>& arr, int marker_number) {
    for (int& val : arr) {
        if (val == marker_number) {
            val = 0;
        }
    }
}