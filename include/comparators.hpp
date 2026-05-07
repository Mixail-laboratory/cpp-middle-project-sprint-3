#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    auto operator()(const bookdb::Book &iBook, const bookdb::Book &iRBook) { return iBook.author < iRBook.author; }
};

struct LessByPopularity {
    auto operator()(const bookdb::Book &iBook, const bookdb::Book &iRBook) {
        return iBook.read_count < iRBook.read_count;
    }
};

struct LessByRating {
    auto operator()(const bookdb::Book &iBook, const bookdb::Book &iRBook) { return iBook.rating < iRBook.rating; }
};

struct GreaterByRating {
    auto operator()(const bookdb::Book &iBook, const bookdb::Book &iRBook) { return iBook.rating > iRBook.rating; }
};

struct GreaterByPopularity {
    auto operator()(const bookdb::Book &iBook, const bookdb::Book &iRBook) {
        return iBook.read_count > iRBook.read_count;
    }
};

}  // namespace bookdb::comp