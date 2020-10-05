#pragma once

namespace flow
{
    struct None
    {
    };
    
    template<class T>
    class Maybe
    {
    public:
        using ValueType = T;
        
        Maybe(None): valid(false)
        {
        }
        
        Maybe(T const &value): valid(true), data(value)
        {
        }
        
        Maybe(T &&value): valid(true), data(std::move(value))
        {
        }
        
        Maybe(Maybe const &other): valid(other.valid)
        {
            if (valid)
            {
                new (&data) T(other.data);
            }
        }
        
        Maybe(Maybe &&other): valid(other.valid)
        {
            if (valid)
            {
                new (&data) T(std::move(other.data));
            }
        }
        
        ~Maybe()
        {
            if (valid)
            {
                data.~T();
            }
        }
        
        Maybe &operator=(Maybe const &other)
        {
            this->~Maybe();
            valid = other.valid;
            if (valid)
            {
                new (&data) T(other.data);
            }
            return *this;
        }
        
        Maybe &operator=(T &&other)
        {
            this->~Maybe();
            valid = other.valid;
            if (valid)
            {
                new (&data) T(std::move(other.data));
            }
            
            return *this;
        }
        
        bool hasValue() const
        {
            return valid;
        }
        
        T &value() &
        {
            return data;
        }
        
        T const &value() const &
        {
            return data;
        }
        
        T &&value() &&
        {
            return std::move(data);
        }
        
        bool operator==(Maybe const &other) const
        {
            return (!valid && !other.valid) || (valid && other.valid && data == other.data);
        }
        
        bool operator!=(Maybe const &other) const
        {
            return !(*this == other);
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
        using ValueType = T &;
        
        Maybe(None): pointer(nullptr)
        {
        }
        
        Maybe(T &reference): pointer(&reference)
        {
        }
        
        Maybe(Maybe const &other): pointer(other.pointer)
        {
        }
        
        Maybe &operator=(Maybe const &other)
        {
            pointer = other.pointer;
            return *this;
        }
        
        bool hasValue() const
        {
            return pointer != nullptr;
        }
        
        T &value()
        {
            return *pointer;
        }
        
        T const &value() const
        {
            return *pointer;
        }
        
        bool operator==(Maybe const &other) const
        {
            return (!pointer && !other.pointer) || (pointer && other.pointer && *pointer == *other.pointer);
        }
        
        bool operator!=(Maybe const &other) const
        {
            return !(*this == other);
        }
        
    private:
        T *pointer;
    };
    
    /// Maybes over rvalue references are still illegal.
    /// TODO: Does it make sense to implement them owning the value?
    template<class T>
    class Maybe<T &&>;
    
    /// Usually, when returning a maybe which is either a value or none, the type of the maybe has to be
    /// noted explicitly, because both branches has to return the same type, and it is not possible
    /// to reverse infer a type based on another branch or t infer the correct type using the none constructor.
    /// Therefore, this function can be used to enforce forward type inference.
    /// The maybe type is infered from the value which is returned if the condition evaluates to true.
    /// Otherwise, a none maybe is returned.
    template<class T>
    Maybe<T> maybeIf(bool condition, T &&value)
    {
        if (condition)
        {
            return Maybe(value);
        }
        else
        {
            return None();
        }
    }
    
    template<class T>
    struct IsMaybe
    {
        static const constexpr bool value = false;
    };
    
    template<class T>
    struct IsMaybe<Maybe<T>>
    {
        static const constexpr bool value = true;
    };
    
    template<class T>
    static const constexpr bool isMaybe = IsMaybe<T>::value;
}
