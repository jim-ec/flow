#pragma once

namespace flow
{
    struct None
    {
    };
    
    /// An optional type, which is immutable by design.
    /// It differs from the STL's optional type in that it's immutable design avoids rebinding versus assignment forwarding debate,
    /// and therefore allowing optionals to hold references.
    template<class T>
    class Maybe
    {
    public:
        Maybe(None): valid(false)
        {
        }
        
        Maybe(T const &value): valid(true), data(value)
        {
        }
        
        Maybe(T &&value): valid(true), data(std::move(value))
        {
        }
        
        ~Maybe()
        {
            if (valid)
            {
                data.~T();
            }
        }
        
        bool holdsValue() const
        {
            return valid;
        }
        
        T const &value() const &
        {
            return data;
        }
        
        T &&value() &&
        {
            return std::move(data);
        }
        
    private:
        bool valid;
        union
        {
            T data;
        };
    };
    
    /// Implementation of the optional type where the held type is a reference.
    /// Internally, the reference is backed as a nullable pointer.
    /// Because references cannot be null in contrast to pointers, the information whether a value is held or not
    /// can be directly encoded into the pointer, by either being null or not.
    template<class T>
    class Maybe<T &>
    {
    public:
        Maybe(None): pointer(nullptr)
        {
        }
        
        Maybe(T &value): pointer(&value)
        {
        }
        
        bool holdsValue() const
        {
            return pointer != nullptr;
        }
        
        T const &value()
        {
            return *pointer;
        }
        
    private:
        T const* const pointer;
    };
    
    /// Maybes over rvalue references are still illegal.
    /// TODO: Does it make sense to implement them owning the value?
    template<class T>
    class Maybe<T &&>;
}
