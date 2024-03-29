#ifndef DACAL_ALGORITHM_HPP
#define DACAL_ALGORITHM_HPP

#include "iterator.hpp"
#include "quick_sort.hpp"
#include "utils.hpp"

#include <type_traits>

namespace dacal {
template<class Container>
[[maybe_unused]] auto begin(Container &_container)
    -> decltype(_container.begin())
{
    return _container.begin();
}

template<class Container>
[[maybe_unused]] auto end(Container &_container) -> decltype(_container.end())
{
    return _container.end();
}

template<class Container>
[[maybe_unused]] auto rbegin(Container &_container)
    -> decltype(_container.rbegin())
{
    return _container.rbegin();
}

template<class Container>
[[maybe_unused]] auto rend(Container &_container) -> decltype(_container.rend())
{
    return _container.rend();
}

template<InputIterator InIter, class UnaryFunction>
[[maybe_unused]] void
for_each(InIter _first, InIter _last, const UnaryFunction &_func)
{
    for (; _first != _last; ++_first) {
        _func(*_first);
    }
}

template<InputIterator InIter, class T>
[[maybe_unused]] [[nodiscard]] InIter
find(InIter _first, InIter _last, const T &_value)
{
    for (; _first != _last; ++_first) {
        if (*_first == _value) {
            return _first;
        }
    }
    return _last;
}

template<InputIterator InIter, class UnaryPredicate>
[[maybe_unused]] [[nodiscard]] InIter
find_if(InIter _first, InIter _last, const UnaryPredicate &_predicate)
{
    for (; _first != _last; ++_first) {
        if (_predicate(*_first)) {
            return _first;
        }
    }
    return _last;
}

template<InputIterator InIter, class T>
[[maybe_unused]] typename InIter::difference_type
count(InIter _first, InIter _last, const T &_value)
{
    typename InIter::difference_type _ret = 0;
    for (; _first != _last; ++_first) {
        if (*_first == _value)
            ++_ret;
    }
    return _ret;
}

template<InputIterator InIter, class UnaryPredicate>
[[maybe_unused]] typename InIter::difference_type
count_if(InIter _first, InIter _last, const UnaryPredicate &_predicate)
{
    typename InIter::difference_type _ret = 0;
    for (; _first != _last; ++_first) {
        if (_predicate(*_first))
            ++_ret;
    }
    return _ret;
}

template<InputIterator InIter, class UnaryPredicate>
[[maybe_unused]] bool
any_off(InIter _first, InIter _last, const UnaryPredicate &_predicate)
{
    return dacal::find_if(_first, _last, _predicate) != _last;
}

template<InputIterator InIter, class T, class BinaryOperation = dacal::plus<T>>
[[maybe_unused]] T accumulate(
    InIter _first,
    InIter _last,
    T _init,
    const BinaryOperation &_op = dacal::plus<T>())
{
    for (; _first != _last; ++_first) {
        _init = _op(dacal::move(_init), dacal::move(*_first));
    }
    return _init;
}

template<
    RandomAccessIterator RandIter,
    class Compare = dacal::less<typename RandIter::value_type>>
[[maybe_unused]] void qsort(
    RandIter _first,
    RandIter _last,
    const Compare &_compare = dacal::less<typename RandIter::value_type>{})
{
    detail::qsort_helper(_first, 0, (_last - _first) - 1, _compare);
}

template<InputIterator InIter>
[[maybe_unused]] typename InIter::difference_type
distance(InIter _first, InIter _last)
{
    typename InIter::difference_type _result = 0;
    if constexpr (std::is_same_v<
                      random_access_iterator_tag,
                      typename InIter::iterator_category>) {
        return _last - _first;
    }
    else {
        for (; _first != _last; ++_first, ++_result)
            ;
        return _result;
    }
}

template<InputIterator InIter, OutputIterator OutIter>
[[maybe_unused]] OutIter copy(InIter _first, InIter _last, OutIter _d_first)
{
    for (; _first != _last; ++_first, ++_d_first) {
        *_d_first = *_first;
    }
    return _d_first;
}

template<InputIterator InIter, OutputIterator OutIter, class UnaryFunction>
[[maybe_unused]] OutIter transform(
    InIter _first, InIter _last, OutIter _d_first, const UnaryFunction &_func)
{
    for (; _first != _last; ++_first, ++_d_first) {
        *_d_first = _func(*_first);
    }
    return _d_first;
}

template<InputIterator InIter1, InputIterator InIter2, OutputIterator OutIter>
[[maybe_unused]] OutIter merge(
    InIter1 _first1,
    InIter1 _last1,
    InIter2 _first2,
    InIter2 _last2,
    OutIter _d_first)
{
    for (; _first1 != _last1; ++_d_first) {
        if (!(_first2 != _last2)) {
            return dacal::copy(_first1, _last1, _d_first);
        }
        if (*_first2 < *_first1) {
            *_d_first = *_first2;
            ++_first2;
        }
        else {
            *_d_first = *_first1;
            ++_first1;
        }
    }
    return dacal::copy(_first2, _last2, _d_first);
}

}  // namespace dacal

#endif  // DACAL_ALGORITHM_HPP
