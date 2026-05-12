#pragma once

#include <concepts>
#include <iterator>
#include <utility>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept IsBook = std::same_as<T, Book>;

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
    requires std::sentinel_for<S, I>;
    requires BookIterator<I>;

    { it == sentinel } -> std::convertible_to<bool>;
    { sentinel == it } -> std::convertible_to<bool>;
};

template <typename T>
concept BookContainerLike = requires(T &container) {
    { container.begin() } -> BookIterator;
    { container.end() } -> BookSentinel<decltype(container.begin())>;

    { *container.begin() } -> std::convertible_to<const Book &>;

    { container.size() } -> std::convertible_to<std::size_t>;

    { container.push_back(std::declval<const Book &>()) } -> std::same_as<void>;

    { container.clear() } -> std::same_as<void>;
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