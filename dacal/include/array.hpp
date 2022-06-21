#ifndef DACAL_ARRAY_HPP
#define DACAL_ARRAY_HPP

#include <cstdint>
#include <initializer_list>
#include "iterator.hpp"

namespace detail
{
    template<class T>
    struct [[maybe_unused]] array_iterator : dacal::base_iterator<dacal::random_access_iterator_tag  , T , std::size_t , T*, T&>
    {
        using typename dacal::base_iterator<dacal::random_access_iterator_tag  , T , std::size_t , T*, T&>::iterator_category;
        using typename dacal::base_iterator<dacal::random_access_iterator_tag  , T , std::size_t , T*, T&>::value_type;
        using typename dacal::base_iterator<dacal::random_access_iterator_tag  , T , std::size_t , T*, T&>::difference_type;
        using typename dacal::base_iterator<dacal::random_access_iterator_tag  , T , std::size_t , T*, T&>::pointer;
        using typename dacal::base_iterator<dacal::random_access_iterator_tag  , T , std::size_t , T*, T&>::reference;

        [[maybe_unused]] explicit array_iterator(T *ptr) : _ptr(ptr) { }
        [[maybe_unused]] array_iterator() = default;

        [[maybe_unused]] array_iterator& operator--()
        {
            --_ptr;
            return *this;
        }

        [[maybe_unused]] auto operator--(int) -> array_iterator
        {
            _ptr--;
            return *this;
        }

        [[maybe_unused]] array_iterator& operator++()
        {
            ++_ptr;
            return *this;
        }

        [[maybe_unused]] auto operator++(int) -> array_iterator
        {
            _ptr++;
            return *this;
        }

        [[maybe_unused]] bool operator!=(const array_iterator &i)
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

        [[maybe_unused]] array_iterator operator+(int n)
        {
            return array_iterator{_ptr + n};
        }

        [[maybe_unused]] array_iterator operator-(int n)
        {
            return array_iterator{_ptr - n};
        }

        [[maybe_unused]] array_iterator::difference_type operator-(const array_iterator &rhs)
        {
            return this->_ptr - rhs._ptr;
        }

        [[maybe_unused]] bool operator>(const array_iterator &rhs)
        {
            return _ptr > rhs._ptr;
        }

        [[maybe_unused]] bool operator<(const array_iterator &rhs)
        {
            return _ptr < rhs._ptr;
        }

        [[maybe_unused]] bool operator>=(const array_iterator &rhs)
        {
            return _ptr >= rhs._ptr;
        }

        [[maybe_unused]] bool operator<=(const array_iterator &rhs)
        {
            return _ptr <= rhs._ptr;
        }

        T* _ptr;
    };

    template<class T>
    struct [[maybe_unused]] const_array_iterator : dacal::base_iterator<dacal::random_access_iterator_tag  , T , std::size_t , T*, T&>
    {
        using typename dacal::base_iterator<dacal::random_access_iterator_tag  , T , std::size_t , T*, T&>::iterator_category;
        using typename dacal::base_iterator<dacal::random_access_iterator_tag  , T , std::size_t , T*, T&>::value_type;
        using typename dacal::base_iterator<dacal::random_access_iterator_tag  , T , std::size_t , T*, T&>::difference_type;
        using typename dacal::base_iterator<dacal::random_access_iterator_tag  , T , std::size_t , T*, T&>::pointer;
        using typename dacal::base_iterator<dacal::random_access_iterator_tag  , T , std::size_t , T*, T&>::reference;

        [[maybe_unused]] explicit const_array_iterator(T* ptr) : _ptr(ptr) { }
        [[maybe_unused]] const_array_iterator() = default;

        [[maybe_unused]] const_array_iterator& operator--()
        {
            --_ptr;
            return *this;
        }

        [[maybe_unused]] auto operator--(int) -> const_array_iterator
        {
            _ptr--;
            return *this;
        }

        [[maybe_unused]] const_array_iterator& operator++()
        {
            ++_ptr;
            return *this;
        }

        [[maybe_unused]] auto operator++(int) -> const_array_iterator
        {
            _ptr++;
            return *this;
        }

        [[maybe_unused]] bool operator!=(const const_array_iterator &i)
        {
            return this->_ptr != i._ptr;
        }

        [[maybe_unused]] value_type operator*() const
        {
            return *_ptr;
        }

        [[maybe_unused]] const_array_iterator operator+(int n)
        {
            return const_array_iterator{_ptr + n};
        }

        [[maybe_unused]] const_array_iterator operator-(int n)
        {
            return const_array_iterator{_ptr - n};
        }

        [[maybe_unused]] const_array_iterator::difference_type operator-(const const_array_iterator &rhs)
        {
            return this->_ptr - rhs._ptr;
        }

        [[maybe_unused]] bool operator>(const const_array_iterator &rhs)
        {
            return _ptr > rhs._ptr;
        }

        [[maybe_unused]] bool operator<(const const_array_iterator &rhs)
        {
            return _ptr < rhs._ptr;
        }

        [[maybe_unused]] bool operator>=(const const_array_iterator &rhs)
        {
            return _ptr >= rhs._ptr;
        }

        [[maybe_unused]] bool operator<=(const const_array_iterator &rhs)
        {
            return _ptr <= rhs._ptr;
        }

        T* _ptr;
    };

} // namespace detail

namespace dacal
{
    template<typename T , std::size_t N>
    class array
    {
    public :
        using value_type             = T;
        using reference              = T&;
        using const_reference        = const T&;
        using iterator               = detail::array_iterator<T>;
        using const_iterator         = detail::const_array_iterator<T>;
        using reverse_iterator       = detail::container_reverse_iterator<iterator>;

        [[maybe_unused]] array() = default;
        [[maybe_unused]] array(const array<T , N> &cp);
        [[maybe_unused]] array(const std::initializer_list<T> &initializer);

        [[maybe_unused]] array<T , N>& operator=(const array<T , N> &cp);
        [[maybe_unused]] reference operator[](std::size_t offset);

        [[maybe_unused]] iterator begin();
        [[maybe_unused]] iterator end();

        [[maybe_unused]] const_iterator cbegin();
        [[maybe_unused]] const_iterator cend();

        [[maybe_unused]] reverse_iterator rbegin();
        [[maybe_unused]] reverse_iterator rend();

        [[maybe_unused]] void push_back(const_reference data);
        [[maybe_unused]] [[nodiscard]] value_type pop_back();
        [[maybe_unused]] [[nodiscard]] value_type pop_front();

        [[maybe_unused]] [[nodiscard]] std::size_t capacity() const;
        [[maybe_unused]] [[nodiscard]] std::size_t length()   const;
    private:
        std::size_t  _size  = N;
        [[maybe_unused]] static std::size_t _len;
        T _buffer[N];
    };

    template<typename T , std::size_t N>
    std::size_t array<T , N>::_len = 0;

    template<typename T, std::size_t N>
    [[maybe_unused]] array<T, N>::array(const array<T, N> &cp)
    {
        _size = N;
        _len  = cp._len;
        for (auto i = 0; i < _len; i++) {
            _buffer[i] = cp._buffer[i];
        }
    }

    template<typename T, std::size_t N>
    [[maybe_unused]] array<T, N>::array(const std::initializer_list<T> &initializer)
    {
        for (auto i  = initializer.begin() ; i != initializer.end(); i++) {
            _buffer[_len++] = *i;
        }
    }

    template<typename T, std::size_t N>
    [[maybe_unused]] array<T, N> &array<T, N>::operator=(const array<T, N> &cp)
    {
        if (this == &cp) {
            _size = N;
            _len = cp.len;
            for (auto i = 0; i < _len; i++) {
                _buffer[i] = cp._buffer[i];
            }
        }
        return *this;
    }

    template<typename T, std::size_t N>
    [[maybe_unused]] typename array<T , N>::reference array<T, N>::operator[](std::size_t offset)
    {
        return _buffer[offset];
    }

    template<typename T, std::size_t N>
    [[maybe_unused]] typename array<T , N>::iterator array<T, N>::begin()
    {
        return iterator(_buffer);
    }

    template<typename T, std::size_t N>
    [[maybe_unused]] typename array<T , N>::iterator array<T, N>::end()
    {
        return iterator(_buffer + _len);
    }

    template<typename T, std::size_t N>
    [[maybe_unused]] typename array<T , N>::const_iterator array<T, N>::cbegin()
    {
        return const_iterator(_buffer);
    }

    template<typename T, std::size_t N>
    [[maybe_unused]] typename array<T , N>::const_iterator array<T, N>::cend()
    {
        return const_iterator(_buffer + _len);
    }

    template<typename T, std::size_t N>
    [[maybe_unused]] typename array<T , N>::reverse_iterator array<T, N>::rbegin()
    {
        return reverse_iterator(iterator(_buffer + (_len - 1)));
    }

    template<typename T, std::size_t N>
    [[maybe_unused]] typename array<T , N>::reverse_iterator array<T, N>::rend()
    {
        return reverse_iterator(iterator(_buffer - 1));
    }

    template<typename T, std::size_t N>
    [[maybe_unused]] void array<T, N>::push_back(typename array<T , N>::const_reference data)
    {
        if (_len < _size) {
            _buffer[_len++]  = data;
        }
    }

    template<typename T, std::size_t N>
    [[maybe_unused]] [[nodiscard]] typename array<T , N>::value_type array<T, N>::pop_back()
    {
        auto ret_val = _buffer[_len - 1];
        _buffer[(_len--) - 1].~T();
        return ret_val;
    }

    template<typename T, std::size_t N>
    [[maybe_unused]] [[nodiscard]] typename array<T , N>::value_type array<T, N>::pop_front()
    {
        auto ret_val = _buffer[0];
        _buffer[0].~T();
        for (auto i = 1; i < _len; i++) {
            _buffer[i - 1] = _buffer[i];
        }
        _len--;
        return ret_val;
    }

    template<typename T, std::size_t N>
    [[maybe_unused]] [[nodiscard]] std::size_t array<T, N>::capacity() const
    {
        return _size;
    }

    template<typename T, std::size_t N>
    [[maybe_unused]] [[nodiscard]] std::size_t array<T, N>::length() const
    {
        return _len;
    }

} // namespace dacal

#endif // DACAL_ARRAY_HPP
