//
// Created by jim on 27.05.19.
//
#pragma once

std::string concat(const std::string &a, const std::string &b);

std::string concat(const std::string &a, int n);

struct S
{
    int id{};
    bool default_constructed{};
    bool argument_constructed{};
    bool copy_constructed{};
    bool move_constructed{};
    bool copy_assigned{};
    bool move_assigned{};

    S() : id{-1}, default_constructed{true}
    {
        printf("S(): %d\n", id);
    }

    explicit S(int id) :
            id{id},
            argument_constructed{true}
    {
        printf("S(int): %d\n", id);
    }

    S(const S &rhs) :
            id{rhs.id},
            copy_constructed{true}
    {
        printf("S(const S&): %d\n", id);
    }

    S(S &&rhs) noexcept :
            id{rhs.id},
            move_constructed{true}
    {
        printf("S(S&&): %d\n", id);
    }

    S &operator=(const S &rhs)
    {
        copy_assigned = true;
        id = rhs.id;
        printf("S::operator=(const S &): %d\n", id);
        return *this;
    }

    S &operator=(S &&rhs) noexcept
    {
        move_assigned = true;
        id = rhs.id;
        printf("S::operator=(S&&): %d\n", id);
        return *this;
    }
};
