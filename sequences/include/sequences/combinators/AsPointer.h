//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter>
class AsPointer
{
public:
    using cache_value_type = iter_value_type_t<Iter>;
    using value_type = cache_value_type *;

private:
    Iter m_iter;
    mutable value_type m_cache{};

public:

    AsPointer() = default;

    AsPointer(const AsPointer &rhs) = default;

    AsPointer(AsPointer &&rhs) noexcept = default;

    AsPointer &operator=(const AsPointer &rhs) = default;

    AsPointer &operator=(AsPointer &&rhs) noexcept = default;

    explicit AsPointer(const Iter &iter) :
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

    AsPointer &operator++()
    {
        ++m_iter;
        return *this;
    }

    AsPointer operator+(const size_t offset) const
    {
        AsPointer result{m_iter};
        for (size_t i = 0; i < offset; i++)
        {
            ++result.m_iter;
        }
        return result;
    }

    bool operator==(const AsPointer &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const AsPointer &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter>
AsPointer<Iter> make_as_pointer(const Iter &iter)
{
    return AsPointer<Iter>{iter};
}

}
