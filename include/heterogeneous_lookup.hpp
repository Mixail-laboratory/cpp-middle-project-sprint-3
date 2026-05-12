#pragma once

#include "book.hpp"
#include <functional>
#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;

    bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs < rhs; }

    bool operator()(const std::string &lhs, std::string_view rhs) const { return lhs < rhs; }

    bool operator()(std::string_view lhs, const std::string &rhs) const { return lhs < rhs; }

    bool operator()(const std::string &lhs, const std::string &rhs) const { return lhs < rhs; }

    bool operator()(const char *lhs, std::string_view rhs) const { return std::string_view(lhs) < rhs; }

    bool operator()(std::string_view lhs, const char *rhs) const { return lhs < std::string_view(rhs); }

    bool operator()(const char *lhs, const std::string &rhs) const { return std::string_view(lhs) < rhs; }

    bool operator()(const std::string &lhs, const char *rhs) const { return lhs < std::string_view(rhs); }
};

struct TransparentStringHash {
    using is_transparent = void;

    size_t operator()(std::string_view str) const { return std::hash<std::string_view>{}(str); }

    size_t operator()(const std::string &str) const { return std::hash<std::string>{}(str); }
};

struct TransparentStringEqual {
    using is_transparent = void;

    bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs == rhs; }

    bool operator()(const std::string &lhs, std::string_view rhs) const { return lhs == rhs; }

    bool operator()(std::string_view lhs, const std::string &rhs) const { return lhs == rhs; }

    bool operator()(const std::string &lhs, const std::string &rhs) const { return lhs == rhs; }
};

}  // namespace bookdb
