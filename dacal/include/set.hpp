#ifndef DACAL_SET_HPP
#define DACAL_SET_HPP

#include "iterator.hpp"
#include "utils.hpp"

#include <initializer_list>
#include <memory>

namespace dacal {
template<
    class T,
    class Compare = dacal::less<T>,
    class Allocator = std::allocator<T>>
class [[maybe_unused]] set
{
public:
    using value_type = T;
    using const_reference = const T &;
    using value_compare = Compare;
    using iterator = detail::rb_tree_iterator<T>;
    using reverse_iterator = detail::container_reverse_iterator<iterator>;
    using allocator = Allocator;
    using node_allocator = typename std::allocator_traits<
        allocator>::template rebind_alloc<detail::rb_tree_node<value_type>>;

    [[maybe_unused]] set() = default;
    [[maybe_unused]] set(const std::initializer_list<T> &_initializer);
    [[maybe_unused]] set(const set &_other);
    [[maybe_unused]] set(set &&_other) noexcept;
    [[maybe_unused]] ~set();

    [[maybe_unused]] set &operator=(const set &_other);
    [[maybe_unused]] set &operator=(set &&_other) noexcept;

    [[maybe_unused]] iterator begin() const;
    [[maybe_unused]] iterator end() const;

    [[maybe_unused]] reverse_iterator rbegin() const;
    [[maybe_unused]] reverse_iterator rend() const;

    [[maybe_unused]] void insert(const_reference _data);

private:
    [[maybe_unused]] void _insert(
        detail::rb_tree_node<T> *&_node,
        detail::rb_tree_node<T> *_parent,
        detail::rb_tree_node<T> *new_node);
    [[maybe_unused]] void _fix_insert(detail::rb_tree_node<T> *node);
    [[maybe_unused]] void _rotateRight(detail::rb_tree_node<T> *node);
    [[maybe_unused]] void _rotateLeft(detail::rb_tree_node<T> *node);
    [[maybe_unused]] detail::rb_tree_node<value_type> *
    _find(detail::rb_tree_node<value_type> *node, const_reference data);

    allocator _allocator;
    node_allocator _node_allocator;
    value_compare _compare;
    detail::rb_tree_node<T> *_root_of_tree{};
};

template<class T, class Compare, class Allocator>
[[maybe_unused]] void set<T, Compare, Allocator>::_insert(
    detail::rb_tree_node<T> *&_node,
    detail::rb_tree_node<T> *_parent,
    detail::rb_tree_node<T> *new_node)
{
    if (_node == nullptr) {
        _node = new_node;
        _node->_parent = _parent;
    }
    else {
        if (_compare(
                new_node->_data,
                _node->_data)) {  // less(_data , _node->_data) or greater(_data
                                  // , _node->_data)
            _insert(_node->_left_child, _node, new_node);
        }
        else if (_compare(
                     _node->_data,
                     new_node->_data)) {  // less(_node->_data , _data) or
                                          // greater(_node->_data , _data)
            _insert(_node->_right_child, _node, new_node);
        }
        else {
            // do nothing!
            return;
        }
    }
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] void
set<T, Compare, Allocator>::_fix_insert(detail::rb_tree_node<T> *node)
{
    if (node->_parent != nullptr) {
        auto parent = node->_parent;
        if (node != _root_of_tree && parent->_color == detail::color::red) {
            auto grandparent = node->_parent->_parent;
            if (grandparent) {
                auto uncle = grandparent->_right_child == parent
                    ? grandparent->_left_child
                    : grandparent->_right_child;
                if (uncle != nullptr && uncle->_color == detail::color::red) {
                    parent->_color = parent->_color == detail::color::red
                        ? detail::color::black
                        : detail::color::red;
                    uncle->_color = uncle->_color == detail::color::red
                        ? detail::color::black
                        : detail::color::red;
                    grandparent->_color =
                        grandparent->_color == detail::color::red
                        ? detail::color::black
                        : detail::color::red;
                    _fix_insert(grandparent);
                }
                else if (node == parent->_left_child) {
                    if (!parent->_left_child) {
                        _rotateLeft(parent);
                    }
                    parent->_color = parent->_color == detail::color::red
                        ? detail::color::black
                        : detail::color::red;
                    grandparent->_color =
                        grandparent->_color == detail::color::red
                        ? detail::color::black
                        : detail::color::red;
                    _rotateRight(grandparent);
                    _fix_insert(parent->_left_child ? parent : grandparent);
                }
                else if (node == parent->_right_child) {
                    if (!parent->_left_child) {
                        _rotateRight(parent);
                    }
                    parent->_color = parent->_color == detail::color::red
                        ? detail::color::black
                        : detail::color::red;
                    grandparent->_color =
                        grandparent->_color == detail::color::red
                        ? detail::color::black
                        : detail::color::red;
                    _rotateLeft(grandparent);
                    _fix_insert(parent->_left_child ? parent : grandparent);
                }
            }
        }
    }
    _root_of_tree->_color = detail::color::black;
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] void
set<T, Compare, Allocator>::_rotateRight(detail::rb_tree_node<T> *node)
{
    auto left_node = node->_left_child;
    if (left_node != nullptr) {
        node->_left_child = left_node->_right_child;

        if (node->_left_child != nullptr) {
            node->_left_child->_parent = node;
        }

        left_node->_right_child = node;
        left_node->_parent = node->_parent;

        if (node->_parent == nullptr) {
            _root_of_tree = left_node;
        }
        else if (node == node->_parent->_left_child) {
            node->_parent->_left_child = left_node;
        }
        else {
            node->_parent->_right_child = left_node;
        }

        node->_parent = left_node;
    }
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] void
set<T, Compare, Allocator>::_rotateLeft(detail::rb_tree_node<T> *node)
{
    auto right_node = node->_right_child;

    if (right_node != nullptr) {
        node->_right_child = right_node->_left_child;
        if (node->_right_child != nullptr) {
            node->_right_child->_parent = node;
        }

        right_node->_left_child = node;
        right_node->_parent = node->_parent;

        if (node->_parent == nullptr) {
            _root_of_tree = right_node;
        }
        else if (node == node->_parent->_left_child) {
            node->_parent->_left_child = right_node;
        }
        else {
            node->_parent->_right_child = right_node;
        }

        node->_parent = right_node;
    }
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] detail::rb_tree_node<
    typename set<T, Compare, Allocator>::value_type> *
set<T, Compare, Allocator>::_find(
    detail::rb_tree_node<value_type> *node, const_reference data)
{
    if (node == nullptr) {
        return nullptr;
    }

    if (node->_data == data) {
        return node;
    }
    else if (node->_data > data) {
        return _find(node->_left_child, data);
    }
    else {
        return _find(node->_right_child, data);
    }
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] set<T, Compare, Allocator>::set(
    const std::initializer_list<T> &_initializer)
{
    for (auto i = _initializer.begin(); i != _initializer.end(); ++i) {
        this->insert(*i);
    }
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] set<T, Compare, Allocator>::set(const set &_other)
{
    for (set<T, Compare, Allocator>::iterator i = _other.begin();
         i != _other.end();
         ++i) {
        this->insert(*i);
    }
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] set<T, Compare, Allocator>::set(set &&_other) noexcept
{
    this->_root_of_tree = dacal::exchange(_other._root_of_tree, nullptr);
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] set<T, Compare, Allocator>::~set()
{
    std::allocator_traits<node_allocator>::deallocate(
        _node_allocator, _root_of_tree, 1);
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] set<T, Compare, Allocator> &
set<T, Compare, Allocator>::operator=(const set &_other)
{
    if (this != &_other) {
        for (set<T, Compare, Allocator>::iterator i = _other.begin();
             i != _other.end();
             ++i) {
            this->insert(*i);
        }
    }
    return *this;
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] set<T, Compare, Allocator> &
set<T, Compare, Allocator>::operator=(set &&_other) noexcept
{
    this->_root_of_tree = dacal::exchange(_other._root_of_tree, nullptr);
    return *this;
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] typename set<T, Compare, Allocator>::iterator
set<T, Compare, Allocator>::begin() const
{
    return set::iterator(
        detail::rb_tree_node<T>::_leftmost_node(_root_of_tree));
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] typename set<T, Compare, Allocator>::iterator
set<T, Compare, Allocator>::end() const
{
    return set::iterator(nullptr);
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] typename set<T, Compare, Allocator>::reverse_iterator
set<T, Compare, Allocator>::rbegin() const
{
    return set::reverse_iterator(
        set::iterator(detail::rb_tree_node<T>::_rightmost_node(_root_of_tree)));
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] typename set<T, Compare, Allocator>::reverse_iterator
set<T, Compare, Allocator>::rend() const
{
    return set::reverse_iterator(set::iterator(nullptr));
}

template<class T, class Compare, class Allocator>
[[maybe_unused]] void
set<T, Compare, Allocator>::insert(set::const_reference _data)
{
    auto new_node =
        std::allocator_traits<node_allocator>::allocate(_node_allocator, 1);
    std::allocator_traits<allocator>::construct(
        _allocator,
        new_node,
        _data,
        detail::color::red,
        nullptr,
        nullptr,
        nullptr);
    _insert(_root_of_tree, nullptr, new_node);
    _fix_insert(new_node);
}

}  // namespace dacal

#endif  // DACAL_SET_HPP
