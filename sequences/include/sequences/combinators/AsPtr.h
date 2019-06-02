//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter>
class AsPtr
{
public:
    using cache_value_type = iter_value_type_t<Iter>;
    using value_type = cache_value_type *;

private:
    Iter m_iter;
    mutable value_type m_cache{};

public:

    AsPtr() = default;

    AsPtr(const AsPtr &rhs) = default;

    AsPtr(AsPtr &&rhs) noexcept = default;

    AsPtr &operator=(const AsPtr &rhs) = default;

    AsPtr &operator=(AsPtr &&rhs) noexcept = default;

    explicit AsPtr(const Iter &iter) :
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

    AsPtr &operator++()
    {
        ++m_iter;
        return *this;
    }

    AsPtr operator+(const size_t offset) const
    {
        AsPtr result{m_iter};
        for (size_t i = 0; i < offset; i++)
        {
            ++result.m_iter;
        }
        return result;
    }

    bool operator==(const AsPtr &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const AsPtr &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter>
AsPtr<Iter> make_as_ptr(const Iter &iter)
{
    return AsPtr<Iter>{iter};
}

}
