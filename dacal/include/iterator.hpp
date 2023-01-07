#ifndef DACAL_ITERATOR_HPP
#define DACAL_ITERATOR_HPP

#include "utils.hpp"

#include <concepts>

namespace dacal {
struct input_iterator_tag
{};
struct output_iterator_tag
{};
struct forward_iterator_tag : input_iterator_tag
{};
struct bidirectional_iterator_tag : forward_iterator_tag
{};
struct random_access_iterator_tag : bidirectional_iterator_tag
{};

template<
    class Category,
    class T,
    class Distance,
    class Pointer,
    class Reference>
struct base_iterator
{
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distance;
    using pointer = Pointer;
    using reference = Reference;
};

template<class T>
concept InputIterator =
    std::derived_from<typename T::iterator_category, input_iterator_tag> &&
    requires(T ptr) {
        {
            *ptr
        };
        {
            ++ptr
        };
        {
            ptr++
        };
    } && requires(T ptr1, T ptr2) {
             {
                 ptr1 != ptr2
             };
         };

template<class T>
concept OutputIterator =
    std::derived_from<typename T::iterator_category, output_iterator_tag> &&
    requires(T ptr) {
        {
            *ptr
        };
        {
            ++ptr
        };
        {
            ptr++
        };
    };

template<class T>
concept ForwardIterator =
    std::derived_from<typename T::iterator_category, forward_iterator_tag> &&
    InputIterator<T> && std::is_default_constructible_v<T> &&
    std::is_copy_constructible_v<T>;

template<class T>
concept BidirectionalIterator = ForwardIterator<T> &&
    std::is_default_constructible_v<T> && std::is_copy_constructible_v<T> &&
    requires(T ptr) {
        {
            --ptr
        };
        {
            ptr--
        };
    } &&
    std::derived_from<typename T::iterator_category,
                      bidirectional_iterator_tag>;

template<class T>
concept RandomAccessIterator = BidirectionalIterator<T> &&
    std::is_default_constructible_v<T> && std::is_copy_constructible_v<T> &&
    requires(T ptr_1, T ptr_2, int n) {
        {
            ptr_1 - ptr_2
        };
        {
            ptr_1 > ptr_2
        };
        {
            ptr_1 < ptr_2
        };
        {
            ptr_1 <= ptr_2
        };
        {
            ptr_1 >= ptr_2
        };
        {
            ptr_1 + n
        };
        {
            ptr_1 - n
        };
    } &&
    std::derived_from<typename T::iterator_category,
                      random_access_iterator_tag>;

template<class Container>
struct [[maybe_unused]] insert_iterator
    : base_iterator<output_iterator_tag, void, std::size_t, void, void>
{
    [[maybe_unused]] explicit insert_iterator() : _container(nullptr) {}
    [[maybe_unused]] explicit insert_iterator(Container &container) :
        _container(&container)
    {}

    [[maybe_unused]] insert_iterator &
    operator=(const typename Container::value_type &data)
    {
        _container->insert(data);
        return *this;
    }

    [[maybe_unused]] insert_iterator &operator++()
    {
        return *this;
    }

    [[maybe_unused]] auto operator++([[maybe_unused]] int i) -> insert_iterator
    {}

    [[maybe_unused]] insert_iterator &operator*()
    {
        return *this;
    }

    Container *_container;
};

template<class Container>
struct [[maybe_unused]] back_insert_iterator
    : base_iterator<output_iterator_tag, void, std::size_t, void, void>
{
    [[maybe_unused]] explicit back_insert_iterator() : _container(nullptr) {}
    [[maybe_unused]] explicit back_insert_iterator(Container &container) :
        _container(&container)
    {}

    [[maybe_unused]] back_insert_iterator &
    operator=(const typename Container::value_type &data)
    {
        _container->push_back(data);
        return *this;
    }

    [[maybe_unused]] back_insert_iterator &operator++()
    {
        return *this;
    }

    [[maybe_unused]] auto operator++([[maybe_unused]] int i)
        -> back_insert_iterator
    {}

    [[maybe_unused]] back_insert_iterator &operator*()
    {
        return *this;
    }

    Container *_container;
};

template<class Container>
[[maybe_unused]] inline insert_iterator<Container>
inserter(Container &_container)
{
    return insert_iterator(_container);
}

template<class Container>
[[maybe_unused]] inline back_insert_iterator<Container>
back_inserter(Container &_container)
{
    return back_insert_iterator(_container);
}

template<class Base>
class [[maybe_unused]] reverse_view
{
public:
    using iterator = typename Base::reverse_iterator;

    [[maybe_unused]] explicit reverse_view(Base &base) : _base(&base) {}

    [[maybe_unused]] iterator begin()
    {
        return _base->rbegin();
    }
    [[maybe_unused]] iterator end()
    {
        return _base->rend();
    }

private:
    Base *_base;
};

}  // namespace dacal

namespace detail {
template<class Iter>
struct container_reverse_iterator : dacal::base_iterator<
                                        typename Iter::iterator_category,
                                        typename Iter::value_type,
                                        typename Iter::difference_type,
                                        typename Iter::pointer,
                                        typename Iter::reference>
{
    [[maybe_unused]] explicit container_reverse_iterator(Iter iter) :
        _iter(iter)
    {}

    [[maybe_unused]] auto operator*()
    {
        return *_iter;
    }

    [[maybe_unused]] container_reverse_iterator &operator++()
    {
        --_iter;
        return *this;
    }

    [[maybe_unused]] auto operator++(int) -> container_reverse_iterator
    {
        _iter--;
        return *this;
    }

    [[maybe_unused]] bool operator!=(const container_reverse_iterator &rhs)
    {
        return this->_iter != rhs._iter;
    }

    Iter _iter;
};

template<class T>
struct [[maybe_unused]] rb_tree_iterator
    : dacal::base_iterator<
          dacal::bidirectional_iterator_tag,
          T,
          std::size_t,
          T *,
          T &>
{
    /*
     *                      *
     *          (22)        *
     *         /   \        *
     *      (11)  (44)      *
     *            /         *
     *          (33)        *
     *                      *
     **/

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

    [[maybe_unused]] explicit rb_tree_iterator(detail::rb_tree_node<T> *ptr) :
        _ptr(ptr)
    {}

    [[maybe_unused]] rb_tree_iterator &operator++()
    {
        if (_ptr == nullptr)
            return *this;

        if (_ptr->_right_child != nullptr) {
            _ptr = _ptr->_right_child;
            _ptr = rb_tree_node<T>::_leftmost_node(_ptr);
        }
        else {
            auto parent = _ptr->_parent;

            while (parent != nullptr && _ptr == parent->_right_child) {
                _ptr = parent;
                parent = parent->_parent;
            }

            _ptr = parent;
        }

        return *this;
    }

    [[maybe_unused]] auto operator++(int) -> rb_tree_iterator
    {
        ++(*this);
        return *this;
    }

    [[maybe_unused]] rb_tree_iterator &operator--()
    {
        if (_ptr == nullptr)
            return *this;

        if (_ptr->_left_child != nullptr) {
            _ptr = _ptr->_left_child;
            _ptr = rb_tree_node<T>::_rightmost_node(_ptr);
        }
        else {
            auto parent = _ptr->_parent;

            while (parent != nullptr && _ptr == parent->_left_child) {
                _ptr = parent;
                parent = parent->_parent;
            }

            _ptr = parent;
        }

        return *this;
    }

    [[maybe_unused]] auto operator--(int) -> rb_tree_iterator
    {
        --(*this);
        return *this;
    }

    [[maybe_unused]] bool operator!=(const rb_tree_iterator &rhs) const
    {
        return _ptr != rhs._ptr;
    }

    [[maybe_unused]] reference operator*() const
    {
        return _ptr->_data;
    }

    rb_tree_node<T> *_ptr{};
};

}  // namespace detail

#endif  // DACAL_ITERATOR_HPP
