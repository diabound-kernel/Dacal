#ifndef DACAL_QUICK_SORT_HPP
#define DACAL_QUICK_SORT_HPP

#include "iterator.hpp"
#include "utils.hpp"

namespace detail {
template<class Iterator, class Compare>
int partition(Iterator array, int low, int high, const Compare &compare)
{
    auto middle = (low + high) / 2;
    dacal::swap(array[middle], array[high]);
    auto pivot = array[high];

    auto greater_th_pivot = 0, current = 0;
    for (; current < high; current++) {
        if (compare.operator()(array[current], pivot)) {
            dacal::swap(array[greater_th_pivot], array[current]);
            greater_th_pivot++;
        }
    }

    dacal::swap(array[greater_th_pivot], array[high]);
    return greater_th_pivot;
}

template<class Iterator, class Compare>
void qsort_helper(Iterator array, int low, int high, const Compare &compare)
{
    if (low < high) {
        auto pivot_offset = partition(array, low, high, compare);

        qsort_helper(array, low, pivot_offset - 1, compare);
        qsort_helper(array, pivot_offset + 1, high, compare);
    }
}

}  // namespace detail

#endif  // DACAL_QUICK_SORT_HPP
