#ifndef DACAL_LIST_HPP
#define DACAL_LIST_HPP

#include "iterator.hpp"
#include "utils.hpp"

#include <initializer_list>
#include <memory>

namespace detail {
template<class T>
struct list_node
{
    list_node(T data, list_node *predecessor, list_node *successor) :
        _data(data),
        _predecessor(predecessor),
        _successor(successor)
    {}

    T _data{};
    list_node<T> *_predecessor{};
    list_node<T> *_successor{};
};

template<class T>
struct [[maybe_unused]] list_iterator : dacal::base_iterator<
                                            dacal::bidirectional_iterator_tag,
                                            T,
                                            std::size_t,
                                            T *,
                                            T &>
{
    using typename dacal::base_iterator<
        dacal::bidirectional_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::iterator_category;

    using typename dacal::base_iterator<
        dacal::bidirectional_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::value_type;

    using typename dacal::base_iterator<
        dacal::bidirectional_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::difference_type;

    using typename dacal::base_iterator<
        dacal::bidirectional_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::pointer;

    using typename dacal::base_iterator<
        dacal::bidirectional_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::reference;

    [[maybe_unused]] explicit list_iterator(list_node<T> *ptr) : _ptr(ptr) {}
    [[maybe_unused]] list_iterator() = default;

    [[maybe_unused]] list_iterator &operator--()
    {
        this->_ptr = this->_ptr->_predecessor;
        return *this;
    }

    [[maybe_unused]] auto operator--(int) -> list_iterator
    {
        auto _temp = *this;
        this->_ptr = this->_ptr->_predecessor;
        return _temp;
    }

    [[maybe_unused]] list_iterator &operator++()
    {
        this->_ptr = this->_ptr->_successor;
        return *this;
    }

    [[maybe_unused]] auto operator++(int) -> list_iterator
    {
        auto _temp = *this;
        this->_ptr = this->_ptr->_successor;
        return _temp;
    }

    [[maybe_unused]] bool operator!=(const list_iterator &i)
    {
        return this->_ptr != i._ptr;
    }

    [[maybe_unused]] reference operator*()
    {
        return _ptr->_data;
    }

    list_node<T> *_ptr;
};

template<class T>
struct [[maybe_unused]] const_list_iterator
    : dacal::base_iterator<
          dacal::bidirectional_iterator_tag,
          T,
          std::size_t,
          T *,
          T &>
{
    using typename dacal::base_iterator<
        dacal::bidirectional_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::iterator_category;

    using typename dacal::base_iterator<
        dacal::bidirectional_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::value_type;

    using typename dacal::base_iterator<
        dacal::bidirectional_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::difference_type;

    using typename dacal::base_iterator<
        dacal::bidirectional_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::pointer;

    using typename dacal::base_iterator<
        dacal::bidirectional_iterator_tag,
        T,
        std::size_t,
        T *,
        T &>::reference;

    [[maybe_unused]] explicit const_list_iterator(list_node<T> *ptr) : _ptr(ptr)
    {}
    [[maybe_unused]] const_list_iterator() = default;

    [[maybe_unused]] const_list_iterator &operator--()
    {
        this->_ptr = this->_ptr->_predecessor;
        return *this;
    }

    [[maybe_unused]] auto operator--(int) -> const_list_iterator
    {
        auto _temp = *this;
        this->_ptr = this->_ptr->_predecessor;
        return _temp;
    }

    [[maybe_unused]] const_list_iterator &operator++()
    {
        this->_ptr = this->_ptr->_successor;
        return *this;
    }

    [[maybe_unused]] auto operator++(int) -> const_list_iterator
    {
        auto _temp = *this;
        this->_ptr = this->_ptr->_successor;
        return _temp;
    }

    [[maybe_unused]] bool operator!=(const const_list_iterator &i)
    {
        return this->_ptr != i._ptr;
    }

    [[maybe_unused]] value_type operator*() const
    {
        return _ptr->_data;
    }

    list_node<T> *_ptr;
};

}  // namespace detail

namespace dacal {
template<class T, class Allocator = std::allocator<T>>
class [[maybe_unused]] list
{
public:
    using value_type = T;
    using const_reference = const T &;
    using iterator = detail::list_iterator<T>;
    using const_iterator = detail::const_list_iterator<T>;
    using reverse_iterator = detail::container_reverse_iterator<iterator>;
    using const_reverse_iterator =
        detail::container_reverse_iterator<const_iterator>;
    using allocator = Allocator;
    using node_allocator = typename std::allocator_traits<
        allocator>::template rebind_alloc<detail::list_node<T>>;

    [[maybe_unused]] list() = default;
    [[maybe_unused]] list(const list<T, Allocator> &_other);
    [[maybe_unused]] list(list<T, Allocator> &&_other) noexcept;
    [[maybe_unused]] list(const std::initializer_list<T> &initializer);
    [[maybe_unused]] ~list();

    [[maybe_unused]] list<T, Allocator> &
    operator=(const list<T, Allocator> &_other);
    [[maybe_unused]] list<T, Allocator> &
    operator=(list<T, Allocator> &&_other) noexcept;

    [[maybe_unused]] iterator begin() const;
    [[maybe_unused]] iterator end() const;

    [[maybe_unused]] const_iterator cbegin();
    [[maybe_unused]] const_iterator cend();

    [[maybe_unused]] reverse_iterator rbegin();
    [[maybe_unused]] reverse_iterator rend();

    [[maybe_unused]] const_reverse_iterator crbegin() const;
    [[maybe_unused]] const_reverse_iterator crend() const;

    [[maybe_unused]] void push_back(const_reference _data);
    [[maybe_unused]] void remove(const_reference &_data);
    [[maybe_unused]] [[nodiscard]] value_type pop_back();
    [[maybe_unused]] [[nodiscard]] value_type pop_front();

private:
    [[maybe_unused]] void _push_back(const T &data);
    [[maybe_unused]] void _destroy();

    allocator _allocator;
    node_allocator _node_allocator;
    detail::list_node<T> *_list_head{}, *_list_tail{};
};

template<class T, class Allocator>
[[maybe_unused]] void list<T, Allocator>::_push_back(const T &data)
{
    if (_list_head == nullptr) {
        _list_head =
            std::allocator_traits<node_allocator>::allocate(_node_allocator, 1);
        std::allocator_traits<allocator>::construct(
            _allocator, _list_head, data, nullptr, nullptr);
    }
    else {
        detail::list_node<T> *i;
        for (i = _list_head; i->_successor != nullptr; i = i->_successor)
            ;
        i->_successor =
            std::allocator_traits<node_allocator>::allocate(_node_allocator, 1);
        std::allocator_traits<allocator>::construct(
            _allocator, i->_successor, data, i, nullptr);
        _list_tail = i->_successor;
    }
}

template<class T, class Allocator>
[[maybe_unused]] void list<T, Allocator>::_destroy()
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
[[maybe_unused]] list<T, Allocator>::list(const list<T, Allocator> &_other)
{
    for (auto i = _other._list_head; i != nullptr; i = i->_successor) {
        _push_back(i->_data);
    }
}

template<class T, class Allocator>
[[maybe_unused]] list<T, Allocator>::list(list<T, Allocator> &&_other) noexcept
{
    this->_list_head = exchange(_other._list_head, nullptr);
    this->_list_tail = exchange(_other._list_tail, nullptr);
}

template<class T, class Allocator>
[[maybe_unused]] list<T, Allocator>::list(
    const std::initializer_list<T> &initializer)
{
    for (auto i = initializer.begin(); i != initializer.end(); ++i) {
        _push_back(*i);
    }
}

template<class T, class Allocator>
[[maybe_unused]] list<T, Allocator>::~list()
{
    if (_list_head)
        _destroy();
}

template<class T, class Allocator>
[[maybe_unused]] list<T, Allocator> &
list<T, Allocator>::operator=(const list<T, Allocator> &_other)
{
    if (this != &_other) {  // avoid self assigment
        for (auto i = _other._list_head; i != nullptr; i = i->_successor) {
            _push_back(i->_data);
        }
    }
    return *this;
}

template<class T, class Allocator>
[[maybe_unused]] list<T, Allocator> &
list<T, Allocator>::operator=(list<T, Allocator> &&_other) noexcept
{
    this->_list_head = exchange(_other._list_head, nullptr);
    this->_list_tail = exchange(_other._list_tail, nullptr);
    return *this;
}

template<class T, class Allocator>
[[maybe_unused]] typename list<T, Allocator>::iterator
list<T, Allocator>::begin() const
{
    return list::iterator(_list_head);
}

template<class T, class Allocator>
[[maybe_unused]] typename list<T, Allocator>::iterator
list<T, Allocator>::end() const
{
    return list::iterator(nullptr);
}

template<class T, class Allocator>
[[maybe_unused]] typename list<T, Allocator>::const_iterator
list<T, Allocator>::cbegin()
{
    return list::const_iterator(_list_head);
}

template<class T, class Allocator>
[[maybe_unused]] typename list<T, Allocator>::const_iterator
list<T, Allocator>::cend()
{
    return list::const_iterator(nullptr);
}

template<class T, class Allocator>
[[maybe_unused]] typename list<T, Allocator>::reverse_iterator
list<T, Allocator>::rbegin()
{
    return reverse_iterator(list<T, Allocator>::iterator(_list_tail));
}

template<class T, class Allocator>
[[maybe_unused]] typename list<T, Allocator>::reverse_iterator
list<T, Allocator>::rend()
{
    return reverse_iterator(list<T, Allocator>::iterator(nullptr));
}

template<class T, class Allocator>
[[maybe_unused]] typename list<T, Allocator>::const_reverse_iterator
list<T, Allocator>::crbegin() const
{
    return const_reverse_iterator(
        list<T, Allocator>::const_iterator(_list_tail));
}

template<class T, class Allocator>
[[maybe_unused]] typename list<T, Allocator>::const_reverse_iterator
list<T, Allocator>::crend() const
{
    return const_reverse_iterator(list<T, Allocator>::const_iterator(nullptr));
}

template<class T, class Allocator>
[[maybe_unused]] void list<T, Allocator>::push_back(const_reference _data)
{
    _push_back(_data);
}

template<class T, class Allocator>
[[maybe_unused]] void
list<T, Allocator>::remove(typename list<T, Allocator>::const_reference _data)
{
    detail::list_node<T> *_node_to_delete = nullptr;
    for (auto i = _list_head; i != nullptr; i = i->_successor) {
        if (i->_data == _data) {
            _node_to_delete = i;
        }
    }

    if (_node_to_delete) {
        if (_node_to_delete->_predecessor != nullptr &&
            _node_to_delete->_successor != nullptr) {
            auto predecessor = _node_to_delete->_predecessor;
            auto successor = _node_to_delete->_successor;
            predecessor->_successor = successor;
            successor->_predecessor = predecessor;
            std::allocator_traits<allocator>::destroy(
                _allocator, _node_to_delete);
            std::allocator_traits<node_allocator>::deallocate(
                _node_allocator, _node_to_delete, 1);
        }
        else if (
            _node_to_delete->_predecessor == nullptr &&
            _node_to_delete->_successor != nullptr) {
            auto successor = _node_to_delete->_successor;
            std::allocator_traits<allocator>::destroy(
                _allocator, _node_to_delete);
            std::allocator_traits<node_allocator>::deallocate(
                _node_allocator, _node_to_delete, 1);
            successor->_predecessor = nullptr;
            _list_head = successor;
        }
        else if (
            _node_to_delete->_predecessor != nullptr &&
            _node_to_delete->_successor == nullptr) {
            auto predecessor = _node_to_delete->_predecessor;
            std::allocator_traits<allocator>::destroy(
                _allocator, _node_to_delete);
            std::allocator_traits<node_allocator>::deallocate(
                _node_allocator, _node_to_delete, 1);
            predecessor->_successor = nullptr;
        }
        else {
            std::allocator_traits<allocator>::destroy(_allocator, _list_head);
            std::allocator_traits<node_allocator>::deallocate(
                _node_allocator, _list_head, 1);
            _list_head = nullptr;
        }
    }
}

template<class T, class Allocator>
[[maybe_unused]] [[nodiscard]] typename list<T, Allocator>::value_type
list<T, Allocator>::pop_back()
{
    auto ret_val_ = _list_tail->_data;
    auto temp = _list_tail;
    _list_tail = _list_tail->_predecessor;

    if (_list_tail)
        _list_tail->_successor = nullptr;
    else
        _list_head = nullptr;

    std::allocator_traits<allocator>::destroy(_allocator, temp);
    std::allocator_traits<node_allocator>::deallocate(_node_allocator, temp, 1);
    return ret_val_;
}

template<class T, class Allocator>
[[maybe_unused]] [[nodiscard]] typename list<T, Allocator>::value_type
list<T, Allocator>::pop_front()
{
    if (_list_head->_successor == nullptr) {
        auto ret_val = _list_head->_data;
        std::allocator_traits<allocator>::destroy(_allocator, _list_head);
        std::allocator_traits<node_allocator>::deallocate(
            _node_allocator, _list_head, 1);
        _list_head = nullptr;
        return ret_val;
    }
    else {
        auto ret_val = _list_head->_data;
        auto t = _list_head->_successor;

        std::allocator_traits<allocator>::destroy(_allocator, t->_predecessor);
        std::allocator_traits<node_allocator>::deallocate(
            _node_allocator, t->_predecessor, 1);
        t->_predecessor = nullptr;

        _list_head = t;
        return ret_val;
    }
}

}  // namespace dacal

#endif  // DACAL_LIST_HPP
