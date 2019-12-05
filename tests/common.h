//
// Created by jim on 27.05.19.
//
#pragma once

#include <sstream>

#include <flow/core/Log.h>

std::string concat(const std::string &a, const std::string &b);

std::string concat(const std::string &a, int n);

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

    [[nodiscard]] std::string
    display_flags() const
    {
        std::stringstream ss;
        if (default_constructed) {
            ss << "default constructed, ";
        }
        if (argument_constructed) {
            ss << "argument constructed, ";
        }
        if (copy_constructed) {
            ss << "copy constructed, ";
        }
        if (move_constructed) {
            ss << "move constructed, ";
        }
        if (copy_assigned) {
            ss << "copy assigned, ";
        }
        if (move_assigned) {
            ss << "move assigned, ";
        }

        std::string s = ss.str();
        if (!s.empty()) {
            s.pop_back();
            s.pop_back();
        }
        return s;
    }

    Identifier() : id{-1}, default_constructed{true}
    {
        log("S() = %p", this);
    }

    explicit Identifier(int id) :
            id{id},
            argument_constructed{true}
    {
        log("S(int): %d = %p", id, this);
    }

    Identifier(const Identifier &rhs) :
            id{rhs.id},
            copy_constructed{true}
    {
        log("S(const S&): %d = %p, rhs: %s = %p",
            id, this, rhs.display_flags().data(), &rhs);
    }

    Identifier(Identifier &&rhs) noexcept :
            id{rhs.id},
            move_constructed{true}
    {
        log("S(S&&): %d = %p, rhs: %s = %p", id, this, rhs.display_flags().data(), &rhs);
        rhs.moved_away = true;
    }

    Identifier &operator=(Identifier const &rhs)
    {
        copy_assigned = true;
        id = rhs.id;
        log("S::operator=(const S &): %d = %p, rhs:  %s = %p",
            id, display_flags().data(), this, rhs.display_flags().data(), &rhs);
        return *this;
    }

    Identifier &operator=(Identifier &&rhs) noexcept
    {
        move_assigned = true;
        id = rhs.id;
        log("S::operator=(S&&): %d, %s = %p, rhs:  %s = %p",
            id, display_flags().data(), this, rhs.display_flags().data(), &rhs);
        rhs.moved_away = true;
        return *this;
    }

    bool operator==(Identifier const &rhs) const {
        return id == rhs.id;
    }
};

template<class T>
struct ConstContainerIterator {
    T const *ptr;

    ConstContainerIterator &operator++() {
        ++ptr;
        return *this;
    }

    bool operator!=(ConstContainerIterator const &rhs) {
        return ptr != rhs.ptr;
    }

    T const *operator->() const {
        return ptr;
    }

    T const &operator*() const {
        return *ptr;
    }
};

template<class T>
struct ContainerIterator {
    T *ptr;

    ContainerIterator &operator++() {
        ++ptr;
        return *this;
    }

    bool operator!=(ContainerIterator const &rhs) {
        return ptr != rhs.ptr;
    }

    T *operator->() {
        return ptr;
    }

    T &operator*() {
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

    Container() {
        data = (E *) malloc(sizeof(E) * 2);
        log("Container() = %p, data = %p", this, data);
        new (data) E();
        new (data + 1) E();
    }

    ~Container() {
        log("~Container() = %p,data = %p", this, data);
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
        log("Container(Container const &) = %p, data = %p, rhs = %p, rhs.data = %p",
            this, data, &rhs, rhs.data);
    }

    Container(Container &&rhs) noexcept
    {
        data = rhs.data;
        rhs.data = nullptr;
        log("Container(Container &&) = %p, data = %p, rhs= %p",
            this, data, &rhs);
    }

    ConstContainerIterator<E> begin() const {
        return {data};
    }

    ConstContainerIterator<E> end() const {
        return {data + 1};
    }

    ContainerIterator<E> begin() {
        return {data};
    }

    ContainerIterator<E> end() {
        return {data + 2};
    }

    E &a() {
        return data[0];
    }

    E &b() {
        return data[1];
    }

    E const &a() const {
        return data[0];
    }

    E const &b() const {
        return data[1];
    }

private:
    E *data;
};
