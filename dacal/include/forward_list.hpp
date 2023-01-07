#ifndef DACAL_FORWARD_LIST_HPP
#define DACAL_FORWARD_LIST_HPP

#include "iterator.hpp"
#include "utils.hpp"

#include <initializer_list>
#include <memory>

namespace detail {
template<class T>
struct forward_list_node
{
    forward_list_node(T data, forward_list_node *successor) :
        _data(data),
        _successor(successor)
    {}

    T _data{};
    forward_list_node *_successor{};
};

template<class T>
struct [[maybe_unused]] forward_list_iterator
    : dacal::
          base_iterator<dacal::forward_iterator_tag, T, std::size_t, T *, T &>
{
    using typename dacal::
        base_iterator<dacal::forward_iterator_tag, T, std::size_t, T *, T &>::
            iterator_category;

    using typename dacal::
        base_iterator<dacal::forward_iterator_tag, T, std::size_t, T *, T &>::
            value_type;

    using typename dacal::
        base_iterator<dacal::forward_iterator_tag, T, std::size_t, T *, T &>::
            difference_type;

    using typename dacal::
        base_iterator<dacal::forward_iterator_tag, T, std::size_t, T *, T &>::
            pointer;

    using typename dacal::
        base_iterator<dacal::forward_iterator_tag, T, std::size_t, T *, T &>::
            reference;

    [[maybe_unused]] explicit forward_list_iterator(forward_list_node<T> *ptr) :
        _ptr(ptr)
    {}
    [[maybe_unused]] forward_list_iterator() = default;

    [[maybe_unused]] forward_list_iterator &operator++()
    {
        this->_ptr = this->_ptr->_successor;
        return *this;
    }

    [[maybe_unused]] auto operator++(int) -> forward_list_iterator
    {
        auto _temp = *this;
        this->_ptr = this->_ptr->_successor;
        return _temp;
    }

    [[maybe_unused]] bool operator!=(const forward_list_iterator &i)
    {
        return this->_ptr != i._ptr;
    }

    [[maybe_unused]] reference operator*()
    {
        return _ptr->_data;
    }

    forward_list_node<T> *_ptr;
};

template<class T>
struct [[maybe_unused]] const_forward_list_iterator
    : dacal::
          base_iterator<dacal::forward_iterator_tag, T, std::size_t, T *, T &>
{
    using typename dacal::
        base_iterator<dacal::forward_iterator_tag, T, std::size_t, T *, T &>::
            iterator_category;
    using typename dacal::
        base_iterator<dacal::forward_iterator_tag, T, std::size_t, T *, T &>::
            value_type;
    using typename dacal::
        base_iterator<dacal::forward_iterator_tag, T, std::size_t, T *, T &>::
            difference_type;
    using typename dacal::
        base_iterator<dacal::forward_iterator_tag, T, std::size_t, T *, T &>::
            pointer;
    using typename dacal::
        base_iterator<dacal::forward_iterator_tag, T, std::size_t, T *, T &>::
            reference;

    [[maybe_unused]] explicit const_forward_list_iterator(
        forward_list_node<T> *ptr) :
        _ptr(ptr)
    {}
    [[maybe_unused]] const_forward_list_iterator() = default;

    [[maybe_unused]] const_forward_list_iterator &operator++()
    {
        this->_ptr = this->_ptr->_successor;
        return *this;
    }

    [[maybe_unused]] auto operator++(int) -> const_forward_list_iterator
    {
        auto _temp = *this;
        this->_ptr = this->_ptr->_successor;
        return _temp;
    }

    [[maybe_unused]] bool operator!=(const const_forward_list_iterator &i)
    {
        return this->_ptr != i._ptr;
    }

    [[maybe_unused]] value_type operator*() const
    {
        return _ptr->_data;
    }

    forward_list_node<T> *_ptr;
};

}  // namespace detail

namespace dacal {
template<class T, class Allocator = std::allocator<T>>
class [[maybe_unused]] forward_list
{
public:
    using value_type = T;
    using const_reference = const T &;
    using iterator = detail::forward_list_iterator<T>;
    using const_iterator = detail::const_forward_list_iterator<T>;
    using allocator = Allocator;
    using node_allocator = typename std::allocator_traits<
        allocator>::template rebind_alloc<detail::forward_list_node<T>>;

    [[maybe_unused]] forward_list() = default;
    [[maybe_unused]] forward_list(const forward_list<T, Allocator> &_other);
    [[maybe_unused]] forward_list(forward_list &&_other) noexcept;
    [[maybe_unused]] forward_list(const std::initializer_list<T> &initializer);
    [[maybe_unused]] ~forward_list();

    [[maybe_unused]] forward_list<T, Allocator> &
    operator=(const forward_list<T, Allocator> &_other);
    [[maybe_unused]] forward_list<T, Allocator> &
    operator=(forward_list<T, Allocator> &&_other) noexcept;

    [[maybe_unused]] iterator begin();
    [[maybe_unused]] iterator end();

    [[maybe_unused]] const_iterator cbegin();
    [[maybe_unused]] const_iterator cend();

    [[maybe_unused]] void push_back(const_reference _data);
    [[maybe_unused]] void push_front(const_reference _data);
    [[maybe_unused]] void remove(const_reference _data);
    [[maybe_unused]] [[nodiscard]] value_type pop_back();
    [[maybe_unused]] [[nodiscard]] value_type pop_front();

private:
    [[maybe_unused]] void _push(const T &data);
    [[maybe_unused]] void _destroy();

    allocator _allocator;
    node_allocator _node_allocator;
    detail::forward_list_node<T> *_list_head{};
};

template<class T, class Allocator>
[[maybe_unused]] void forward_list<T, Allocator>::_push(const T &data)
{
    if (_list_head == nullptr) {
        _list_head =
            std::allocator_traits<node_allocator>::allocate(_node_allocator, 1);
        std::allocator_traits<allocator>::construct(
            _allocator, _list_head, data, nullptr);
    }
    else {
        detail::forward_list_node<T> *i;
        for (i = _list_head; i->_successor != nullptr; i = i->_successor)
            ;
        i->_successor =
            std::allocator_traits<node_allocator>::allocate(_node_allocator, 1);
        std::allocator_traits<allocator>::construct(
            _allocator, i->_successor, data, nullptr);
    }
}

template<class T, class Allocator>
[[maybe_unused]] void forward_list<T, Allocator>::_destroy()
{
    for (auto i = _list_head; i != nullptr;) {
        auto temp = i;
        i = i->_successor;
        std::allocator_traits<allocator>::destroy(_allocator, temp);
        std::allocator_traits<node_allocator>::deallocate(
            _node_allocator, temp, 1);
    }
}

template<class T, class Allocator>
[[maybe_unused]] forward_list<T, Allocator>::forward_list(
    const forward_list<T, Allocator> &_other)
{
    for (auto i = _other._list_head; i != nullptr; i = i->_successor) {
        _push(i->_data);
    }
}

template<class T, class Allocator>
[[maybe_unused]] forward_list<T, Allocator>::forward_list(
    forward_list &&_other) noexcept
{
    this->_list_head = exchange(_other._list_head, nullptr);
}

template<class T, class Allocator>
[[maybe_unused]] forward_list<T, Allocator>::forward_list(
    const std::initializer_list<T> &initializer)
{
    for (auto i = initializer.begin(); i != initializer.end(); i++) {
        _push(*i);
    }
}

template<class T, class Allocator>
[[maybe_unused]] forward_list<T, Allocator>::~forward_list()
{
    _destroy();
}

template<class T, class Allocator>
[[maybe_unused]] forward_list<T, Allocator> &
forward_list<T, Allocator>::operator=(const forward_list<T, Allocator> &_other)
{
    if (this != &_other) {  // avoid self assigment
        for (auto i = _other._list_head; i != nullptr; i = i->_successor) {
            _push(i->_data);
        }
    }
    return *this;
}

template<class T, class Allocator>
[[maybe_unused]] forward_list<T, Allocator> &
forward_list<T, Allocator>::operator=(
    forward_list<T, Allocator> &&_other) noexcept
{
    this->_list_head = exchange(_other._list_head, nullptr);
    return *this;
}

template<class T, class Allocator>
[[maybe_unused]] typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::begin()
{
    return iterator(_list_head);
}

template<class T, class Allocator>
[[maybe_unused]] typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::end()
{
    return iterator(nullptr);
}

template<class T, class Allocator>
[[maybe_unused]] typename forward_list<T, Allocator>::const_iterator
forward_list<T, Allocator>::cbegin()
{
    return const_iterator(_list_head);
}

template<class T, class Allocator>
[[maybe_unused]] typename forward_list<T, Allocator>::const_iterator
forward_list<T, Allocator>::cend()
{
    return const_iterator(nullptr);
}

template<class T, class Allocator>
[[maybe_unused]] void forward_list<T, Allocator>::push_back(
    typename forward_list<T, Allocator>::const_reference _data)
{
    _push(_data);
}

template<class T, class Allocator>
[[maybe_unused]] void forward_list<T, Allocator>::push_front(
    typename forward_list<T, Allocator>::const_reference _data)
{
    // TODO implement push_front() method for forward_list<>
}

template<class T, class Allocator>
[[maybe_unused]] void forward_list<T, Allocator>::remove(const_reference _data)
{
    detail::forward_list_node<T> *node_to_delete = nullptr;
    for (auto i = _list_head; i != nullptr; i = i->_successor) {
        if (i->_data == _data) {
            node_to_delete = i;
        }
    }

    if (node_to_delete) {
        if (node_to_delete->_data == _list_head->_data) {
            if (node_to_delete->_successor) {
                _list_head = node_to_delete->_successor;
                std::allocator_traits<allocator>::destroy(
                    _allocator, node_to_delete);
                std::allocator_traits<node_allocator>::deallocate(
                    _node_allocator, node_to_delete, 1);
            }
            else {
                delete _list_head;
                _list_head = nullptr;
            }
        }
        else {
            detail::forward_list_node<T> *tmp_node = _list_head;
            for (; tmp_node->_successor != node_to_delete;
                 tmp_node = tmp_node->_successor)
                ;
            tmp_node->_successor = node_to_delete->_successor;
            std::allocator_traits<allocator>::destroy(
                _allocator, node_to_delete);
            std::allocator_traits<node_allocator>::deallocate(
                _node_allocator, node_to_delete, 1);
        }
    }
}

template<class T, class Allocator>
[[maybe_unused]] [[nodiscard]] T forward_list<T, Allocator>::pop_back()
{
    detail::forward_list_node<T> *i;
    if (_list_head->_successor == nullptr) {
        auto ret_val = _list_head->_data;
        std::allocator_traits<allocator>::destroy(_allocator, _list_head);
        std::allocator_traits<node_allocator>::deallocate(
            _node_allocator, _list_head, 1);
        _list_head = nullptr;
        return ret_val;
    }
    else {
        for (i = _list_head; i->_successor->_successor != nullptr;
             i = i->_successor)
            ;

        auto ret_val = i->_successor->_data;
        std::allocator_traits<allocator>::destroy(_allocator, i->_successor);
        std::allocator_traits<node_allocator>::deallocate(
            _node_allocator, i->_successor, 1);
        i->_successor = nullptr;

        return ret_val;
    }
}

template<class T, class Allocator>
[[maybe_unused]] [[nodiscard]] T forward_list<T, Allocator>::pop_front()
{
    auto temp = _list_head;
    auto ret_value = _list_head->_data;

    _list_head = _list_head->_successor;
    std::allocator_traits<allocator>::destroy(_allocator, temp);
    std::allocator_traits<node_allocator>::deallocate(_node_allocator, temp, 1);
    return ret_value;
}

}  // namespace dacal

#endif  // DACAL_FORWARD_LIST_HPP
