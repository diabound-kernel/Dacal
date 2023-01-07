#ifndef DACAL_VECTOR_HPP
#define DACAL_VECTOR_HPP

#include "iterator.hpp"
#include "utils.hpp"

#include <initializer_list>
#include <memory>

namespace detail {
template<class T>
struct [[maybe_unused]] vector_iterator : dacal::base_iterator<
                                              dacal::random_access_iterator_tag,
                                              T,
                                              std::size_t,
                                              T *,
                                              T &>
{
    using typename dacal::base_iterator<
        dacal::random_access_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::iterator_category;

    using typename dacal::base_iterator<
        dacal::random_access_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::value_type;

    using typename dacal::base_iterator<
        dacal::random_access_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::difference_type;

    using typename dacal::base_iterator<
        dacal::random_access_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::pointer;

    using typename dacal::base_iterator<
        dacal::random_access_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::reference;

    [[maybe_unused]] explicit vector_iterator(T *ptr) : _ptr(ptr) {}
    [[maybe_unused]] vector_iterator() = default;

    [[maybe_unused]] vector_iterator &operator--()
    {
        --_ptr;
        return *this;
    }

    [[maybe_unused]] auto operator--(int) -> vector_iterator
    {
        _ptr--;
        return *this;
    }

    [[maybe_unused]] vector_iterator &operator++()
    {
        ++_ptr;
        return *this;
    }

    [[maybe_unused]] auto operator++(int) -> vector_iterator
    {
        _ptr++;
        return *this;
    }

    [[maybe_unused]] bool operator!=(const vector_iterator &i)
    {
        return this->_ptr != i._ptr;
    }

    [[maybe_unused]] reference operator*()
    {
        return *_ptr;
    }

    [[maybe_unused]] reference operator[](std::size_t _offset)
    {
        return _ptr[_offset];
    }

    [[maybe_unused]] vector_iterator operator+(int n)
    {
        return vector_iterator{_ptr + n};
    }

    [[maybe_unused]] vector_iterator operator-(int n)
    {
        return vector_iterator{_ptr - n};
    }

    [[maybe_unused]] vector_iterator::difference_type
    operator-(const vector_iterator &rhs)
    {
        return this->_ptr - rhs._ptr;
    }

    [[maybe_unused]] bool operator>(const vector_iterator &rhs)
    {
        return _ptr > rhs._ptr;
    }

    [[maybe_unused]] bool operator<(const vector_iterator &rhs)
    {
        return _ptr < rhs._ptr;
    }

    [[maybe_unused]] bool operator>=(const vector_iterator &rhs)
    {
        return _ptr >= rhs._ptr;
    }

    [[maybe_unused]] bool operator<=(const vector_iterator &rhs)
    {
        return _ptr <= rhs._ptr;
    }

    T *_ptr;
};

template<class T>
struct [[maybe_unused]] const_vector_iterator
    : dacal::base_iterator<
          dacal::random_access_iterator_tag,
          T,
          std::size_t,
          T *,
          T &>
{
    using typename dacal::base_iterator<
        dacal::random_access_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::iterator_category;

    using typename dacal::base_iterator<
        dacal::random_access_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::value_type;

    using typename dacal::base_iterator<
        dacal::random_access_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::difference_type;

    using typename dacal::base_iterator<
        dacal::random_access_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::pointer;

    using typename dacal::base_iterator<
        dacal::random_access_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::reference;

    [[maybe_unused]] explicit const_vector_iterator(T *ptr) : _ptr(ptr) {}
    [[maybe_unused]] const_vector_iterator() = default;

    [[maybe_unused]] const_vector_iterator &operator--()
    {
        --_ptr;
        return *this;
    }

    [[maybe_unused]] auto operator--(int) -> const_vector_iterator
    {
        _ptr--;
        return *this;
    }

    [[maybe_unused]] const_vector_iterator &operator++()
    {
        ++_ptr;
        return *this;
    }

    [[maybe_unused]] auto operator++(int) -> const_vector_iterator
    {
        _ptr++;
        return *this;
    }

    [[maybe_unused]] bool operator!=(const const_vector_iterator &i)
    {
        return this->_ptr != i._ptr;
    }

    [[maybe_unused]] value_type operator*() const
    {
        return *_ptr;
    }

    [[maybe_unused]] const_vector_iterator operator+(int n)
    {
        return const_vector_iterator{_ptr + n};
    }

    [[maybe_unused]] const_vector_iterator operator-(int n)
    {
        return const_vector_iterator{_ptr - n};
    }

    [[maybe_unused]] const_vector_iterator::difference_type
    operator-(const const_vector_iterator &rhs)
    {
        return this->_ptr - rhs._ptr;
    }

    [[maybe_unused]] bool operator>(const const_vector_iterator &rhs)
    {
        return _ptr > rhs._ptr;
    }

    [[maybe_unused]] bool operator<(const_vector_iterator &rhs)
    {
        return _ptr < rhs._ptr;
    }

    [[maybe_unused]] bool operator>=(const const_vector_iterator &rhs)
    {
        return _ptr >= rhs._ptr;
    }

    [[maybe_unused]] bool operator<=(const const_vector_iterator &rhs)
    {
        return _ptr <= rhs._ptr;
    }

    T *_ptr;
};

}  // namespace detail

namespace dacal {
template<class T, class Allocator = std::allocator<T> >
class [[maybe_unused]] vector
{
public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using iterator = detail::vector_iterator<T>;
    using const_iterator = detail::const_vector_iterator<T>;
    using reverse_iterator = detail::container_reverse_iterator<iterator>;
    using allocator = Allocator;

    [[maybe_unused]] vector();
    [[maybe_unused]] vector(const std::initializer_list<T> &_initializer);
    [[maybe_unused]] vector(const vector &_other);
    [[maybe_unused]] vector(vector &&_other) noexcept;
    [[maybe_unused]] ~vector();

    [[maybe_unused]] vector &operator=(const vector &_other);
    [[maybe_unused]] vector &operator=(vector &&_other) noexcept;
    [[maybe_unused]] reference operator[](std::size_t _offset);
    [[maybe_unused]] const_reference operator[](std::size_t _offset) const;

    [[maybe_unused]] iterator begin();
    [[maybe_unused]] iterator end();

    [[maybe_unused]] reverse_iterator rbegin();
    [[maybe_unused]] reverse_iterator rend();

    [[maybe_unused]] const_iterator cbegin();
    [[maybe_unused]] const_iterator cend();

    [[maybe_unused]] void push_back(const_reference data);
    [[maybe_unused]] [[nodiscard]] value_type pop_back();
    [[maybe_unused]] [[nodiscard]] value_type pop_front();
    [[maybe_unused]] [[nodiscard]] std::size_t size() const;

private:
    [[maybe_unused]] void _realloc(std::size_t _new_capacity);

    allocator _allocator;
    T *_data{};
    std::size_t _capacity{2};
    std::size_t _size{};
};

template<class T, class Allocator>
[[maybe_unused]] void vector<T, Allocator>::_realloc(std::size_t _new_capacity)
{
    auto _tmp_buffer =
        std::allocator_traits<allocator>::allocate(_allocator, _new_capacity);

    for (auto i = 0; i < _size; ++i) {
        _tmp_buffer[i] = dacal::move(_data[i]);
    }

    std::allocator_traits<allocator>::destroy(_allocator, _data);
    std::allocator_traits<allocator>::deallocate(_allocator, _data, _size);
    _data = _tmp_buffer;
    _capacity = _new_capacity;
}

template<class T, class Allocator>
[[maybe_unused]] vector<T, Allocator>::vector()
{
    _realloc(2);
}

template<class T, class Allocator>
[[maybe_unused]] vector<T, Allocator>::vector(
    const std::initializer_list<T> &_initializer)
{
    for (auto i = _initializer.begin(); i != _initializer.end(); ++i) {
        push_back(*i);
    }
}

template<class T, class Allocator>
[[maybe_unused]] vector<T, Allocator>::vector(const vector &_other)
{
    this->_data = std::allocator_traits<allocator>::allocate(
        _allocator, _other._capacity);

    for (auto i = 0; i < _other._size; ++i) {
        _data[_size++] = _other._data[i];
    }
}

template<class T, class Allocator>
[[maybe_unused]] vector<T, Allocator>::vector(vector &&_other) noexcept
{
    _capacity = dacal::exchange(_other._capacity, 0);
    _size = dacal::exchange(_other._size, 0);
    _data = dacal::exchange(_other._data, nullptr);
}

template<class T, class Allocator>
[[maybe_unused]] vector<T, Allocator>::~vector()
{
    std::allocator_traits<allocator>::destroy(_allocator, _data);
    std::allocator_traits<allocator>::deallocate(_allocator, _data, _capacity);
}

template<class T, class Allocator>
[[maybe_unused]] vector<T, Allocator> &
vector<T, Allocator>::operator=(const vector &_other)
{
    if (this != &_other) {
        this->_data = std::allocator_traits<allocator>::allocate(
            _allocator, _other._capacity);

        for (auto i = 0; i < _other._size; ++i) {
            _data[_size++] = _other._data[i];
        }
    }
    return *this;
}

template<class T, class Allocator>
[[maybe_unused]] vector<T, Allocator> &
vector<T, Allocator>::operator=(vector &&_other) noexcept
{
    _capacity = dacal::exchange(_other._capacity, 0);
    _size = dacal::exchange(_other._size, 0);
    _data = dacal::exchange(_other._data, nullptr);
    return *this;
}

template<class T, class Allocator>
[[maybe_unused]] typename vector<T, Allocator>::reference
vector<T, Allocator>::operator[](std::size_t _offset)
{
    return _data[_offset];
}

template<class T, class Allocator>
[[maybe_unused]] typename vector<T, Allocator>::const_reference
vector<T, Allocator>::operator[](std::size_t _offset) const
{
    return _data[_offset];
}

template<class T, class Allocator>
[[maybe_unused]] typename vector<T, Allocator>::iterator
vector<T, Allocator>::begin()
{
    return dacal::vector<T, Allocator>::iterator(_data);
}

template<class T, class Allocator>
[[maybe_unused]] typename vector<T, Allocator>::iterator
vector<T, Allocator>::end()
{
    return dacal::vector<T, Allocator>::iterator(_data + _size);
}

template<class T, class Allocator>
[[maybe_unused]] typename vector<T, Allocator>::reverse_iterator
vector<T, Allocator>::rbegin()
{
    return dacal::vector<T, Allocator>::reverse_iterator(
        iterator(_data + (_size - 1)));
}

template<class T, class Allocator>
[[maybe_unused]] typename vector<T, Allocator>::reverse_iterator
vector<T, Allocator>::rend()
{
    return dacal::vector<T, Allocator>::reverse_iterator(iterator(_data - 1));
}

template<class T, class Allocator>
[[maybe_unused]] typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::cbegin()
{
    return dacal::vector<T, Allocator>::const_iterator(_data);
}

template<class T, class Allocator>
[[maybe_unused]] typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::cend()
{
    return dacal::vector<T, Allocator>::const_iterator(_data + _size);
}

template<class T, class Allocator>
[[maybe_unused]] void vector<T, Allocator>::push_back(
    typename vector<T, Allocator>::const_reference data)
{
    if (_size >= _capacity) {
        _realloc(_capacity + (_capacity / 2));
    }
    else if (_size == 0) {
        _realloc(2);
    }
    _data[_size++] = data;
}

template<class T, class Allocator>
[[maybe_unused]] [[nodiscard]] typename vector<T, Allocator>::value_type
vector<T, Allocator>::pop_back()
{
    auto ret_val = _data[_size - 1];
    _data[_size--].~T();
    return ret_val;
}

template<class T, class Allocator>
[[maybe_unused]] [[nodiscard]] typename vector<T, Allocator>::value_type
vector<T, Allocator>::pop_front()
{
    auto ret_val = _data[0];
    _data[0].~T();
    for (auto i = 1; i < _size; i++) {
        _data[i - 1] = _data[i];
    }
    _size--;
    return ret_val;
}

template<class T, class Allocator>
[[maybe_unused]] [[nodiscard]] std::size_t vector<T, Allocator>::size() const
{
    return _size;
}

}  // namespace dacal

#endif  // DACAL_VECTOR_HPP
