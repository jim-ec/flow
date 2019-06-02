//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter>
class ByPtr
{
public:
    using cache_value_type = iter_value_type_t<Iter>;
    using value_type = cache_value_type *;

private:
    Iter m_iter;
    mutable value_type m_cache{};

public:

    ByPtr() = default;

    ByPtr(const ByPtr &rhs) = default;

    ByPtr(ByPtr &&rhs) noexcept = default;

    ByPtr &operator=(const ByPtr &rhs) = default;

    ByPtr &operator=(ByPtr &&rhs) noexcept = default;

    explicit ByPtr(const Iter &iter) :
            m_iter{iter}
    {}

    value_type &operator*()
    {
        m_cache = &*m_iter;
        return m_cache;
    }

    const value_type &operator*() const
    {
        m_cache = &*m_iter;
        return m_cache;
    }

    ByPtr &operator++()
    {
        ++m_iter;
        return *this;
    }

    ByPtr operator+(const size_t offset) const
    {
        ByPtr result{m_iter};
        for (size_t i = 0; i < offset; i++)
        {
            ++result.m_iter;
        }
        return result;
    }

    bool operator==(const ByPtr &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const ByPtr &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter>
ByPtr<Iter> make_by_ptr(const Iter &iter)
{
    return ByPtr<Iter>{iter};
}

}
