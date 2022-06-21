#ifndef DACAL_PAIR_HPP
#define DACAL_PAIR_HPP

namespace dacal
{
    template<class T1 , class T2> struct [[maybe_unused]] pair
    {
        [[maybe_unused]] pair(const T1& first , const  T2& second)
            : _first(first) , _second(second) {}

        T1 _first {};
        T2 _second{};
    };

    template<class T1 , class T2>
    [[maybe_unused]] dacal::pair<T1 , T2> make_pair(const T1& first , const T2& second)
    {
        return dacal::pair<T1 , T2>(first , second);
    }

    template<int I , class T1 , class T2>
    [[maybe_unused]] auto get(pair<T1 , T2>& p)
    {
       if constexpr      (I == 0) { return p._first;  }
       else if constexpr (I == 1) { return p._second; }
       else                       { return "none";    }
    }
}

#endif // DACAL_PAIR_HPP
