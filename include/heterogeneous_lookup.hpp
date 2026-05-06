#pragma once

#include "book.hpp"
#include <functional>
#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;

    auto operator()(const bookdb::Book &iBook, const std::string &iStr) { return iBook.title < iStr; }
    auto operator()(const std::string &iStr, const Book &iBook) { return iStr < iBook.title; }
    auto operator()(const Book &iBook, std::string_view iStr) { return iBook.author < iStr; }
    auto operator()(std::string_view iStr, const Book &iBook) { return iStr < iBook.author; }
};

struct TransparentStringEqual {
    using is_transparent = void;

    auto operator()(const Book &iBook, const std::string &iStr) { return iBook.title == iStr; }
    auto operator()(const std::string &iStr, const Book &iBook) { return iStr == iBook.title; }
    auto operator()(const Book &iBook, std::string_view iStr) { return iBook.author == iStr; }
    auto operator()(std::string_view iStr, const Book &iBook) { return iStr == iBook.author; }
};

struct TransparentStringHash {
    using is_transparent = void;

    auto operator()(const Book &iBook) { return std::hash<std::string>{}(iBook.title); }
};

}  // namespace bookdb
