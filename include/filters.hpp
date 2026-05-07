#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {
constexpr auto YearBetween(int start, int end) {
    return [start, end](const Book &iBook) { return iBook.year >= start && iBook.year <= end; };
};
constexpr auto RatingAbove(double min_rating) {
    return [min_rating](const Book &iBook) { return iBook.rating >= min_rating; };
};
constexpr auto GenreIs(Genre genre) {
    return [genre](const Book &iBook) { return iBook.genre == genre; };
};

template <BookIterator It, BookSentinel<It> Sent, BookPredicate Pred>
auto filterBooks(It first, Sent last, Pred pred) {
    std::vector<std::reference_wrapper<const Book>> result;
    std::copy_if(first, last, std::back_inserter(result), [pred](const Book &iBook) { return pred(iBook); });
    return result;
};

template <BookPredicate... Preds>
constexpr auto all_of(Preds... preds) {
    return [preds...](const Book &iBook) { return (... && preds(iBook)); };
}

template <BookPredicate... Preds>
constexpr auto any_of(Preds... preds) {
    return [preds...](const Book &iBook) { return (... || preds(iBook)); };
}

}  // namespace bookdb