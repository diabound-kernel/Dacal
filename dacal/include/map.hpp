#ifndef DACAL_MAP_HPP
#define DACAL_MAP_HPP

#include <initializer_list>
#include <memory>
#include "utils.hpp"
#include "iterator.hpp"
#include "pair.hpp"

namespace dacal
{
    template< class Key , class T , class Compare = dacal::less<Key> , class Allocator = std::allocator<dacal::pair<Key , T>> >
    class [[maybe_unused]] map
    {
    public:
        using key_type         = Key;
        using mapped_type      =  T;
        using key_compare      = Compare;
        using value_type       = dacal::pair<key_type , mapped_type>;
        using iterator         = detail::rb_tree_iterator<value_type>;
        using reverse_iterator = detail::container_reverse_iterator<iterator>;
        using allocator        = Allocator;
        using node_allocator   = typename std::allocator_traits<allocator>::template rebind_alloc<detail::rb_tree_node<value_type>>;

        [[maybe_unused]] map() = default;
        [[maybe_unused]] map(const std::initializer_list<value_type>& _initializer);
        [[maybe_unused]] map(const map& _other);
        [[maybe_unused]] map(map&& _other) noexcept;
        [[maybe_unused]] ~map();

        [[maybe_unused]] map& operator=(const map& _other);
        [[maybe_unused]] map& operator=(map&& _other) noexcept;
        [[maybe_unused]] T& operator[](const key_type& key);

        [[maybe_unused]] iterator begin() const;
        [[maybe_unused]] iterator end() const;

        [[maybe_unused]] reverse_iterator rbegin() const ;
        [[maybe_unused]] reverse_iterator rend() const ;

        [[maybe_unused]] value_type& insert(value_type _data);
        [[maybe_unused]] iterator find(const key_type& key);
    private:
        [[maybe_unused]] void _insert(detail::rb_tree_node<value_type> *&_node,
                                      detail::rb_tree_node<value_type> *_parent,
                                      detail::rb_tree_node<value_type> *new_node);

        void _fix_insert(detail::rb_tree_node<value_type> *node);
        [[maybe_unused]] void _rotateRight(detail::rb_tree_node<value_type> *node);
        [[maybe_unused]] void _rotateLeft(detail::rb_tree_node<value_type> *node);

        allocator _allocator;
        node_allocator _node_allocator;
        key_compare _compare;
        detail::rb_tree_node<value_type> *_root_of_tree{};
    };

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] void map<Key, T, Compare , Allocator>::_insert(detail::rb_tree_node<value_type> *&_node,
                                                        detail::rb_tree_node<value_type> *_parent,
                                                        detail::rb_tree_node<value_type> *new_node)
    {

        if (_node == nullptr) {
            _node = new_node;
            _node->_parent = _parent;
        } else {
            if (_compare(new_node->_data._first , _node->_data._first)) {        // less(_data , _node->_data) or greater(_data , _node->_data)
                _insert(_node->_left_child, _node, new_node);
            } else if (_compare(_node->_data._first , new_node->_data._first)) { // less(_node->_data , _data) or greater(_node->_data , _data)
                _insert(_node->_right_child, _node, new_node);
            } else {
                // do nothing!
                return ;
            }
        }
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] void map<Key, T, Compare , Allocator>::_fix_insert(detail::rb_tree_node<value_type> *node)
    {
        if (node->_parent != nullptr)
        {
            auto parent = node->_parent;
            if (node != _root_of_tree && parent->_color == detail::color::red)
            {
                auto grandparent = node->_parent->_parent;
                if (grandparent)
                {
                    auto uncle = grandparent->_right_child == parent ? grandparent->_left_child : grandparent->_right_child;
                    if (uncle != nullptr && uncle->_color == detail::color::red)
                    {
                        parent->_color      = parent->_color      == detail::color::red ? detail::color::black : detail::color::red;
                        uncle->_color       = uncle->_color       == detail::color::red ? detail::color::black : detail::color::red;
                        grandparent->_color = grandparent->_color == detail::color::red ? detail::color::black : detail::color::red;
                        _fix_insert(grandparent);
                    } else if (node == parent->_left_child) {
                        if (!parent->_left_child)
                        {
                            _rotateLeft(parent);
                        }
                        parent->_color      = parent->_color      == detail::color::red ? detail::color::black : detail::color::red;
                        grandparent->_color = grandparent->_color == detail::color::red ? detail::color::black : detail::color::red;
                        _rotateRight(grandparent);
                        _fix_insert(parent->_left_child ? parent : grandparent);
                    } else if (node == parent->_right_child) {
                        if (!parent->_left_child)
                        {
                            _rotateRight(parent);
                        }
                        parent->_color      = parent->_color      == detail::color::red ? detail::color::black : detail::color::red;
                        grandparent->_color = grandparent->_color == detail::color::red ? detail::color::black : detail::color::red;
                        _rotateLeft(grandparent);
                        _fix_insert(parent->_left_child ? parent : grandparent);
                    }
                }
            }
        }
        _root_of_tree->_color = detail::color::black;
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] void map<Key, T, Compare , Allocator>::_rotateRight(detail::rb_tree_node<value_type> *node)
    {
        auto left_node = node->_left_child;
        if (left_node != nullptr)
        {
            node->_left_child = left_node->_right_child;

            if (node->_left_child != nullptr) {
                node->_left_child->_parent = node;
            }

            left_node->_right_child = node;
            left_node->_parent = node->_parent;

            if (node->_parent == nullptr) {
                _root_of_tree = left_node;
            } else if (node == node->_parent->_left_child) {
                node->_parent->_left_child = left_node;
            } else {
                node->_parent->_right_child = left_node;
            }

            node->_parent = left_node;
        }
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] void map<Key, T, Compare , Allocator>::_rotateLeft(detail::rb_tree_node<value_type> *node)
    {
        auto right_node = node->_right_child;

        if (right_node != nullptr)
        {
            node->_right_child = right_node->_left_child;
            if (node->_right_child != nullptr) {
                node->_right_child->_parent = node;
            }

            right_node->_left_child = node;
            right_node->_parent = node->_parent;

            if (node->_parent == nullptr) {
                _root_of_tree = right_node;
            } else if (node == node->_parent->_left_child) {
                node->_parent->_left_child = right_node;
            } else {
                node->_parent->_right_child = right_node;
            }

            node->_parent = right_node;
        }
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] map<Key, T, Compare , Allocator>::map(const std::initializer_list<value_type> &_initializer)
    {
        for (auto i = _initializer.begin(); i != _initializer.end(); i++) {
            this->insert(*i);
        }
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] map<Key, T, Compare , Allocator>::map(const map &_other)
    {
        for (map<Key , T , Compare  , Allocator>::iterator i = _other.begin(); i != _other.end(); ++i) {
            this->insert(*i);
        }
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] map<Key, T, Compare , Allocator>::map(map &&_other) noexcept
    {
        this->_root_of_tree = dacal::exchange(_other._root_of_tree, nullptr);
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] map<Key, T, Compare , Allocator>::~map()
    {
        std::allocator_traits<node_allocator>::deallocate(_node_allocator , _root_of_tree , 1);

    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] map<Key , T , Compare , Allocator> &map<Key, T, Compare , Allocator>::operator=(const map &_other)
    {
        if (this != &_other)
        {
            for (map<Key, T , Compare , Allocator>::iterator i = _other.begin(); i != _other.end(); ++i) {
                this->insert(*i);
            }
        }
        return *this;
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] map<Key , T , Compare , Allocator>& map<Key, T, Compare , Allocator>::operator=(map &&_other) noexcept
    {
        this->_root_of_tree = dacal::exchange(_other._root_of_tree, nullptr);
        return *this;
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] typename map<Key , T , Compare , Allocator>::iterator map<Key, T, Compare , Allocator>::begin() const
    {
        return dacal::map<Key , T , Compare , Allocator>::iterator(detail::rb_tree_node<value_type>::_leftmost_node(_root_of_tree));
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] typename map<Key , T , Compare , Allocator>::iterator map<Key, T, Compare , Allocator>::end() const
    {
        return dacal::map<Key , T , Compare , Allocator>::iterator(nullptr);
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] typename map<Key , T , Compare , Allocator>::reverse_iterator map<Key, T, Compare , Allocator>::rbegin() const
    {
        return reverse_iterator(iterator(detail::rb_tree_node<value_type>::_rightmost_node(_root_of_tree)));
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] typename map<Key , T , Compare , Allocator>::reverse_iterator map<Key, T, Compare , Allocator>::rend() const
    {
        return reverse_iterator(iterator(nullptr));
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] typename dacal::map<Key , T , Compare , Allocator>::value_type& dacal::map<Key, T, Compare , Allocator>::insert(value_type _data)
    {
        auto new_node = std::allocator_traits<node_allocator>::allocate(_node_allocator , 1);
        std::allocator_traits<allocator>::construct(_allocator , new_node , _data ,detail::color::red, nullptr, nullptr, nullptr);
        _insert(_root_of_tree, nullptr, new_node);
        _fix_insert(new_node);
        return new_node->_data;
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] T& map<Key, T, Compare , Allocator>::operator[](const key_type &key)
    {
        auto iter = find(key);
        if (iter != end()) return (*iter)._second;
        return (insert(dacal::pair<Key , T>{key , T{}}))._second;
    }

    template<class Key, class T, class Compare , class Allocator>
    [[maybe_unused]] typename map<Key, T , Compare , Allocator>::iterator map<Key, T, Compare , Allocator>::find(const key_type & key)
    {
        auto i = begin();
        for (; i != end(); ++i) {
            if ((*i)._first == key) return i;
        }
        return end();
    }

} // namespace dacal

#endif // DACAL_MAP_HPP
