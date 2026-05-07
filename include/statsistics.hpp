#pragma once

#include <algorithm>
#include <flat_map>
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book.hpp"
#include "book_database.hpp"

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    const auto books = cont.GetBooks();
    std::flat_map<std::string_view, size_t> histogram;
    std::for_each(books.begin(), books.end(), [&](const Book &book) {
        auto [it, inserted] = histogram.try_emplace(book.author, 0);
        ++it->second;
    });
    return histogram;
}

template <BookIterator It, BookSentinel<It> Sent>
auto calculateGenreRatings(It first, Sent last) {
    std::flat_map<Genre, std::pair<double, size_t>> genreStats;
    std::for_each(first, last, [&](const Book &iBook) {
        auto [it, _] = genreStats.try_emplace(iBook.genre, 0, 0);
        it->second.first += iBook.rating;
        it->second.second += 1;
    });

    std::flat_map<Genre, double> result;

    std::transform(genreStats.begin(), genreStats.end(), std::inserter(result, result.begin()), [](const auto &pair) {
        const auto &[genre, stats] = pair;
        const auto [sum, count] = stats;

        return std::make_pair(genre, sum / count);
    });
    return result;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &books) {
    if (books.empty()) {
        return 0;
    }
    auto sum = std::accumulate(books.begin(), books.end(), 0.0,
                               [](double acc, const Book &iBook) { return acc + iBook.rating; });
    return sum / books.size();
};

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t num) {
    const auto &books = cont.GetBooks();

    if (num > books.size()) {
        num = books.size();
    }

    if (num == 0) {
        throw std::runtime_error("invalid size");
    }

    std::vector<size_t> indices(books.size());
    std::iota(indices.begin(), indices.end(), 0);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(indices.begin(), indices.end(), gen);

    std::vector<std::reference_wrapper<const Book>> result;
    result.reserve(num);

    std::transform(indices.begin(), indices.begin() + num, std::back_inserter(result),
                   [&books](size_t idx) { return std::reference_wrapper<const Book>(books[idx]); });

    return result;
}

template <BookContainerLike T, typename Comparator>
auto getTopNBy(BookDatabase<T> &cont, size_t n, Comparator comp) {
    std::partial_sort(cont.rbegin(), cont.rbegin() + n, cont.rend(), comp);

    std::vector<std::reference_wrapper<const Book>> result;
    result.reserve(n);
    std::copy(cont.rbegin(), cont.rbegin() + n, std::back_inserter(result));

    return result;
}

}  // namespace bookdb
