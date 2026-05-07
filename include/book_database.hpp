#pragma once

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <print>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

#include <unordered_set>

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    using value_type = Book;
    using size_type = std::size_t;
    using reference = value_type &;
    using const_reference = const value_type &;

    using iterator = BookContainer::iterator;
    using const_iterator = BookContainer::const_iterator;
    using reverse_iterator = BookContainer::reverse_iterator;
    using const_reverse_iterator = BookContainer::const_reverse_iterator;

    using AuthorContainer = std::unordered_set<std::string, TransparentStringHash, TransparentStringEqual>;

    BookDatabase() = default;

    BookDatabase(std::initializer_list<Book> init) : books_(init) {}

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    iterator begin() { return books_.begin(); }
    const_iterator begin() const { return books_.begin(); }
    iterator end() { return books_.end(); }
    const_iterator end() const { return books_.end(); }

    constexpr size_type size() const { return books_.size(); }
    constexpr bool empty() const { return size() == 0; }

    void PushBack(const Book &book) {
        authors_.emplace(book.author);
        books_.push_back(book);
    }
    void PushBack(Book &&book) {
        authors_.emplace(book.author);
        books_.push_back(std::move(book));
    };

    template <typename... Args>
        requires std::constructible_from<Book, Args...>
    void EmplaceBack(Args &&...args) {
        books_.emplace_back(std::forward<Args>(args)...);
        authors_.emplace(books_.back().author);
    }

    std::span<const Book> GetBooks() const { return books_; };
    std::span<Book> GetBooks() { return books_; };
    const AuthorContainer &GetAuthors() { return authors_; };
    const AuthorContainer &GetAuthors() const { return authors_; };

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};
}  // namespace std