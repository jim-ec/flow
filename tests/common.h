//
// Created by jim on 27.05.19.
//
#pragma once

#include <sstream>

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

    std::string display_flags() const
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
//        printf("S()\n");
    }

    explicit Identifier(int id) :
            id{id},
            argument_constructed{true}
    {
//        printf("S(int): %d\n", id);
    }

    Identifier(const Identifier &rhs) :
            id{rhs.id},
            copy_constructed{true}
    {
//        printf("S(const S&): %d\n", id);
//        printf(">  rhs: %s\n", rhs.display_flags().data());
    }

    Identifier(Identifier &&rhs) noexcept :
            id{rhs.id},
            move_constructed{true}
    {
//        printf("S(S&&): %d\n", id);
//        printf(">  rhs: %s\n", rhs.display_flags().data());
        rhs.moved_away = true;
    }

    Identifier &operator=(Identifier const &rhs)
    {
        copy_assigned = true;
        id = rhs.id;
//        printf("S::operator=(const S &): %d\n", id);
//        printf(">  this: %s\n", display_flags().data());
//        printf(">  rhs:  %s\n", rhs.display_flags().data());
        return *this;
    }

    Identifier &operator=(Identifier &&rhs) noexcept
    {
        move_assigned = true;
        id = rhs.id;
//        printf("S::operator=(S&&): %d\n", id);
//        printf(">  this: %s\n", display_flags().data());
//        printf(">  rhs:  %s\n", rhs.display_flags().data());
        rhs.moved_away = true;
        return *this;
    }

    bool operator==(Identifier const &rhs) const {
        return id == rhs.id;
    }
};

template<class E>
struct ConstContainerIterator {
    E const *ptr;

    ConstContainerIterator &operator++() {
        ++ptr;
        return *this;
    }

    bool operator!=(ConstContainerIterator const &rhs) {
        return ptr != rhs.ptr;
    }

    E const *operator->() const {
        return ptr;
    }

    E const &operator*() const {
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
    using const_iterator = ConstContainerIterator<E>;

    Container() {
//        printf("Container()\n");
        data = (E *) malloc(sizeof(E) * 2);
        new (data) E();
        new (data + 1) E();
    }

    ~Container() {
//        printf("~Container()\n");
        data[0].~E();
        data[1].~E();
        free(data);
    }

    Container(Container const &rhs)
    {
//        printf("Container(Container const &)\n");
        data = (E *) malloc(sizeof(E) * 2);
        new (data) E(rhs.data[0]);
        new (data + 1) E(rhs.data[1]);
    }

    Container(Container &&rhs) noexcept
    {
//        printf("Container(Container &&)\n");
        data = rhs.data;
        rhs.data = nullptr;
    }

    ConstContainerIterator<E> begin() const {
        return {data};
    }

    ConstContainerIterator<E> end() const {
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
