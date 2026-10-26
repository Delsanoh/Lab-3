#pragma once
#include <vector>

bool try_mark_element(std::vector<int>& arr, unsigned int index, int marker_number);
unsigned int count_marked_elements(const std::vector<int>& arr, int marker_number);
void reset_marked_elements(std::vector<int>& arr, int marker_number);