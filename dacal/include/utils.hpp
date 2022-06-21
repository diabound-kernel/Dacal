#ifndef DACAL_UTILITY_HPP
#define DACAL_UTILITY_HPP

#include <cstdint>

namespace dacal
{
    template<class T> struct remove_reference      { using type = T; };  //  T = int   -> type = int
    template<class T> struct remove_reference<T&>  { using type = T; };  //  T = int&  -> type = int
    template<class T> struct remove_reference<T&&> { using type = T; };  //  T = int&& -> type = int

    template<class T>
    [[maybe_unused]] constexpr typename remove_reference<T>::type&& move(T&& _arg) noexcept
    {
         /*
             * In this case after argument passing T = T& and if we substitute T with T& we have T&&& which
	     * is actually T& , so we don't want that, so we need to catch only T regardless of the type of
	     * T reference at compile-time. This is possible via remove_reference struct.
         * */
	return
        static_cast<typename remove_reference<T>::type&&>(_arg);
    }

    template<class T>
    [[maybe_unused]] void swap(T& _object_A , T& _object_B) noexcept
    {
        auto _temp = move(_object_A);
        _object_A  = move(_object_B);
        _object_B  = move(_temp);
    }	    

    template<class T , class U = T>
    [[maybe_unused]] [[nodiscard]] T exchange(T& _object , U&& _new_value)
    {
        auto _old_value_ = dacal::move(_object);
        _object          = _new_value;
        return _old_value_;
    }

    template<class T> struct [[maybe_unused]] less
    {
         [[maybe_unused]] bool operator()(const T& _lhs , const T& _rhs) const
         {
             return _lhs < _rhs;
         }
    };

    template<class T> struct [[maybe_unused]] greater
    {
        [[maybe_unused]] bool operator()(const T& _lhs , const T& _rhs) const
        {
            return _lhs > _rhs;
        }
    };

    template<class T> struct [[maybe_unused]] plus
    {
        [[maybe_unused]] T operator()(const T& _lhs , const T& _rhs) const
        {
            return _lhs + _rhs;
        }
    };

    template<class T> struct [[maybe_unused]] minus
    {
        [[maybe_unused]] T operator()(const T& _lhs , const T& _rhs) const
        {
            return _lhs - _rhs;
        }
    };

    template<class T> struct [[maybe_unused]] multiplies
    {
        [[maybe_unused]] T operator()(const T& _lhs , const T& _rhs) const
        {
            return _lhs * _rhs;
        }
    };

    template<class T> struct [[maybe_unused]] divides
    {
        [[maybe_unused]] T operator()(const T& _lhs , const T& _rhs) const
        {
            return _lhs / _rhs;
        }
    };

    template<class T> struct [[maybe_unused]] modulus
    {
        [[maybe_unused]] T operator()(const T& _lhs , const T& _rhs) const
        {
            return _lhs % _rhs;
        }
    };

    template<class T> struct [[maybe_unused]] negate
    {
        [[maybe_unused]] T operator()(const T& _value) const
        {
            return -_value;
        }
    };

    template<class T> struct hash {};

    template<> struct [[maybe_unused]] hash<int>
    {
        uint64_t operator()(const int& _val) const noexcept
        {
            return ((_val | 0b111001010011010) << 12) % 10;
        }
    };

    template<> struct [[maybe_unused]] hash<const char*>
    {
        uint64_t operator()(const char* _val) const noexcept
        {
            uint64_t hash_val = 5381;

            for (; *_val != '\0'; _val++)
            {
                hash_val = ((hash_val << 5) + hash_val) + *_val;
            }
            return hash_val;
        }
    };

    template<class T> struct [[maybe_unused]] hash<T*>
    {
        uint64_t operator()(const T& _ptr) const noexcept
        {
            return reinterpret_cast<uint64_t>(&_ptr);
        }
    };

} // namespace dacal

// red-black tree utils
namespace detail
{
    enum class [[maybe_unused]] color { black = 0, red };

    template<class T>
    struct rb_tree_node
    {
        [[maybe_unused]] rb_tree_node(const T& data, color col, rb_tree_node *left, rb_tree_node *right, rb_tree_node *parent)
                : _data(data), _color(col), _left_child(left), _right_child(right), _parent(parent) {}

        [[maybe_unused]] ~rb_tree_node()
        {
            delete _left_child;
            delete _right_child;
        }

        [[maybe_unused]] static rb_tree_node<T> *_leftmost_node(rb_tree_node<T> *_node)
        {
            if (_node == nullptr) return nullptr;
            auto _temp = _node;
            while (_temp->_left_child != nullptr) {
                _temp = _temp->_left_child;
            }
            return _temp;
        }

        [[maybe_unused]] static rb_tree_node<T> *_rightmost_node(rb_tree_node<T> *_node)
        {
            if (_node == nullptr) return nullptr;
            auto _temp = _node;
            while (_temp->_right_child != nullptr) {
                _temp = _temp->_right_child;
            }
            return _temp;
        }

        T _data;
        color _color{};
        rb_tree_node<T> *_parent{};
        rb_tree_node<T> *_left_child{};
        rb_tree_node<T> *_right_child{};
    };

} // namespace detail

#endif // DACAL_UTILITY_HPP
