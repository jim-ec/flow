#pragma once

struct Identifier
{
    int id{};
    bool default_constructed{};
    bool argument_constructed{};
    bool copy_constructed{};
    bool move_constructed{};
    bool copy_assigned{};
    bool move_assigned{};
    bool moved_away{};

    Identifier():
        id(-1),
        default_constructed(true)
    {
    }

    explicit Identifier(int id):
            id(id),
            argument_constructed(true)
    {
    }

    Identifier(const Identifier &rhs):
            id(rhs.id),
            copy_constructed(true)
    {
    }

    Identifier(Identifier &&rhs) noexcept:
            id(rhs.id),
            move_constructed(true)
    {
        rhs.moved_away = true;
    }

    Identifier &operator=(Identifier const &rhs)
    {
        copy_assigned = true;
        id = rhs.id;
        return *this;
    }

    Identifier &operator=(Identifier &&rhs) noexcept
    {
        move_assigned = true;
        id = rhs.id;
        rhs.moved_away = true;
        return *this;
    }

    bool operator==(Identifier const &rhs) const
    {
        return id == rhs.id;
    }
};

template<class T>
struct ConstContainerIterator
{
    T const *ptr;

    ConstContainerIterator &operator++()
    {
        ++ptr;
        return *this;
    }

    bool operator!=(ConstContainerIterator const &rhs)
    {
        return ptr != rhs.ptr;
    }

    T const *operator->() const
    {
        return ptr;
    }

    T const &operator*() const
    {
        return *ptr;
    }
};

template<class T>
struct ContainerIterator
{
    T *ptr;

    ContainerIterator &operator++()
    {
        ++ptr;
        return *this;
    }

    bool operator!=(ContainerIterator const &rhs)
    {
        return ptr != rhs.ptr;
    }

    T *operator->()
    {
        return ptr;
    }

    T &operator*()
    {
        return *ptr;
    }
};

/// A container with two elements hold in heap.
/// Used to test correct lifetime behaviour.
template<class E>
class Container
{
public:
    using value_type = E;
    using iterator = ContainerIterator<E>;
    using const_iterator = ConstContainerIterator<E>;

    Container()
    {
        data = (E *) malloc(sizeof(E) * 2);
        new (data) E();
        new (data + 1) E();
    }

    ~Container()
    {
        if (data != nullptr)
        {
            data[0].~E();
            data[1].~E();
            free(data);
        }
    }

    Container(Container const &rhs)
    {
        data = (E *) malloc(sizeof(E) * 2);
        new (data) E(rhs.data[0]);
        new (data + 1) E(rhs.data[1]);
    }

    Container(Container &&rhs) noexcept
    {
        data = rhs.data;
        rhs.data = nullptr;
    }

    ConstContainerIterator<E> begin() const
    {
        return {data};
    }

    ConstContainerIterator<E> end() const
    {
        return {data + 1};
    }

    ContainerIterator<E> begin()
    {
        return {data};
    }

    ContainerIterator<E> end()
    {
        return {data + 2};
    }

    E &a()
    {
        return data[0];
    }

    E &b()
    {
        return data[1];
    }

    E const &a() const
    {
        return data[0];
    }

    E const &b() const
    {
        return data[1];
    }

private:
    E *data;
};
