#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

constexpr Genre GenreFromString(std::string_view s) {
    if (s == "Fiction") {
        return Genre::Fiction;
    }
    if (s == "NonFiction") {
        return Genre::NonFiction;
    }
    if (s == "SciFi") {
        return Genre::SciFi;
    }
    if (s == "Biography") {
        return Genre::Biography;
    }
    if (s == "Mystery") {
        return Genre::Mystery;
    }

    return Genre::Unknown;
}

struct Book final {
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    constexpr Book(std::string_view iGenre) : genre(GenreFromString(iGenre)) {};
    constexpr Book(Genre iGenre) : genre(iGenre) {};
    Book(std::string_view iTitle, std::string_view iAuthor, int iYear, Genre iGenre, double iRating, int iReadCount)
        : author(iAuthor), title(iTitle), year(iYear), genre(iGenre), rating(iRating), read_count(iReadCount) {}
};
}  // namespace bookdb

namespace std {

template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book &book, FormatContext &fc) const {
        return std::format_to(fc.out(),
                              "Book(: author: {}, title: {}, year: {}, genre: {}, rating: {}, read_count: {})",
                              book.author, book.title, book.year, book.genre, book.rating, book.read_count);
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};

}  // namespace std
