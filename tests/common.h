//
// Created by jim on 27.05.19.
//
#pragma once

#include <sstream>

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

    S() : id{-1}, default_constructed{true}
    {
//        printf("S()\n");
    }

    explicit S(int id) :
            id{id},
            argument_constructed{true}
    {
//        printf("S(int): %d\n", id);
    }

    S(const S &rhs) :
            id{rhs.id},
            copy_constructed{true}
    {
//        printf("S(const S&): %d\n", id);
//        printf(">  rhs: %s\n", rhs.display_flags().data());
    }

    S(S &&rhs) noexcept :
            id{rhs.id},
            move_constructed{true}
    {
//        printf("S(S&&): %d\n", id);
//        printf(">  rhs: %s\n", rhs.display_flags().data());
        rhs.moved_away = true;
    }

    S &operator=(const S &rhs)
    {
        copy_assigned = true;
        id = rhs.id;
//        printf("S::operator=(const S &): %d\n", id);
//        printf(">  this: %s\n", display_flags().data());
//        printf(">  rhs:  %s\n", rhs.display_flags().data());
        return *this;
    }

    S &operator=(S &&rhs) noexcept
    {
        move_assigned = true;
        id = rhs.id;
//        printf("S::operator=(S&&): %d\n", id);
//        printf(">  this: %s\n", display_flags().data());
//        printf(">  rhs:  %s\n", rhs.display_flags().data());
        rhs.moved_away = true;
        return *this;
    }
};
