#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T &container) {
    { container.begin() } -> std::forward_iterator;
    { container.end() } -> std::forward_iterator;

    { *container.begin() } -> std::convertible_to<const Book &>;

    { container.size() } -> std::convertible_to<std::size_t>;
};

template <typename I>
concept BookIterator = requires(I it) {
    requires std::forward_iterator<I>;

    { *it } -> std::convertible_to<const Book &>;

    { ++it } -> std::same_as<I &>;
    { it++ } -> std::convertible_to<const I &>;

    { it == it } -> std::convertible_to<bool>;
    { it != it } -> std::convertible_to<bool>;
};

template <typename S, typename I>
concept BookSentinel = requires(const S &sentinel, const I &it) {
    { it == sentinel } -> std::convertible_to<bool>;
    { it != sentinel } -> std::convertible_to<bool>;
};

template <typename P>
concept BookPredicate = requires(P pred, const Book &book) {
    { pred(book) } -> std::convertible_to<bool>;
};

template <typename C>
concept BookComparator = requires(C comp, const Book &a, const Book &b) {
    { comp(a, b) } -> std::convertible_to<bool>;
    { comp(b, a) } -> std::convertible_to<bool>;
};

}  // namespace bookdb