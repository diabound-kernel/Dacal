#ifndef DACAL_QUEUE_HPP
#define DACAL_QUEUE_HPP

#include "vector.hpp"

namespace dacal
{
    template< class T , class Container = dacal::vector<T> >
    class [[maybe_unused]] queue
    {
    public:
        [[maybe_unused]] queue() = default;

        [[maybe_unused]] void push(const T& data)
        {
            _container.push_back(data);
            _size++;
        }

        [[maybe_unused]] [[nodiscard]] T pop()
        {
            _size--;
            return _container.pop_front();
        }

        [[maybe_unused]] [[nodiscard]] std::size_t size() const
        {
            return _size;
        }
    private:
        Container _container;
        std::size_t _size{};
    };

} // namespace dacal

#endif // DACAL_QUEUE_HPP
