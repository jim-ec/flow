//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
// Can only work on iterators which's values are a pair-like type.
template<class Iter, class Fn>
class MapPair
{
public:
    using pair_type = typename Iter::value_type;
    using value_type = decltype(std::declval<Fn>()(
            std::declval<typename Iter::value_type_left>(),
            std::declval<typename Iter::value_type_right>()
    ));

private:
    Iter m_iter;
    Fn m_fn;
    mutable value_type m_cache{};

public:

    MapPair() = default;
    MapPair(const MapPair &rhs) = default;
    MapPair(MapPair &&rhs) noexcept = default;

    MapPair &operator=(const MapPair &rhs) = default;
    MapPair &operator=(MapPair &&rhs) noexcept = default;

    MapPair(
            const Iter &iter,
            Fn fn
    ) :
            m_iter{iter},
            m_fn{fn}
    {
    }

    value_type &operator*()
    {
        const pair_type &pair = *m_iter;
        m_cache = m_fn(pair.m_first, pair.m_second);
        return m_cache;
    }

    const value_type &operator*() const
    {
        pair_type &pair = *m_iter;
        m_cache = m_fn(pair.m_first, pair.m_second);
        return m_cache;
    }

    MapPair &operator++()
    {
        ++m_iter;
        return *this;
    }

    MapPair operator+(const size_t offset) const
    {
        MapPair result{m_iter, m_fn};
        for (size_t i = 0; i < offset; i++)
        {
            ++result.m_iter;
        }
        return result;
    }

    bool operator==(const MapPair &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const MapPair &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter, class Fn>
MapPair<Iter, Fn> make_map_pair(
        const Iter &iter,
        Fn fn
)
{
    return MapPair<Iter, Fn>{iter, fn};
}

}
