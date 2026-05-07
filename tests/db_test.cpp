#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statsistics.hpp"
#include <gtest/gtest.h>

namespace bookdb {
class BookDatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 100);
        db.EmplaceBack("1984", "George Orwell", 1949, Genre::Fiction, 4.2, 150);
        db.EmplaceBack("Sapiens", "Yuval Noah Harari", 2011, Genre::NonFiction, 4.7, 200);
        db.EmplaceBack("A Brief History of Time", "Stephen Hawking", 1988, Genre::NonFiction, 4.3, 120);
        db.EmplaceBack("Dune", "Frank Herbert", 1965, Genre::SciFi, 4.6, 180);
    }

    bookdb::BookDatabase<std::vector<Book>> db;
};

TEST_F(BookDatabaseTest, InitializerList) {
    bookdb::BookDatabase<std::vector<Book>> db_init = {
        Book("Test Book", "Test Author", 2020, Genre::Fiction, 4.0, 50),
    };
    EXPECT_EQ(db_init.size(), 1);
}

TEST_F(BookDatabaseTest, PushBack) {
    size_t initial_size = db.size();
    Book book("New Book", "New Author", 2023, Genre::Mystery, 3.9, 75);
    db.PushBack(book);

    EXPECT_EQ(db.size(), initial_size + 1);
    EXPECT_EQ(db.GetBooks().back().title, "New Book");
}

TEST_F(BookDatabaseTest, PushBackRValue) {
    size_t initial_size = db.size();
    db.PushBack(Book("Rvalue Book", "Rvalue Author", 2023, Genre::Biography, 4.1, 60));

    EXPECT_EQ(db.size(), initial_size + 1);
}

TEST_F(BookDatabaseTest, EmplaceBack) {
    size_t initial_size = db.size();
    db.EmplaceBack("Emplaced Book", "Emplaced Author", 2023, Genre::Mystery, 3.8, 45);

    EXPECT_EQ(db.size(), initial_size + 1);
    EXPECT_EQ(db.GetBooks().back().title, "Emplaced Book");
}

TEST_F(BookDatabaseTest, Clear) {
    EXPECT_GT(db.size(), 0);
    db.Clear();

    EXPECT_EQ(db.size(), 0);
    EXPECT_TRUE(db.empty());
    EXPECT_EQ(db.GetAuthors().size(), 0);
}

TEST_F(BookDatabaseTest, GetBooks) {
    auto books = db.GetBooks();
    EXPECT_EQ(books.size(), 5);
    EXPECT_EQ(books[0].title, "The Great Gatsby");
}

TEST_F(BookDatabaseTest, HeterogeneousLookup) {
    auto authors = db.GetAuthors();

    EXPECT_TRUE(authors.contains(std::string_view("Yuval Noah Harari")));

    EXPECT_FALSE(authors.contains(std::string("Unknown Author")));
}

TEST_F(BookDatabaseTest, Iterators) {
    int count = 0;
    for (auto it = db.begin(); it != db.end(); ++it) {
        count++;
    }
    EXPECT_EQ(count, db.size());
}

class FilterTest : public ::testing::Test {
protected:
    void SetUp() override {
        db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 100);
        db.EmplaceBack("1984", "George Orwell", 1949, Genre::Fiction, 4.2, 150);
        db.EmplaceBack("Sapiens", "Yuval Noah Harari", 2011, Genre::NonFiction, 4.7, 200);
        db.EmplaceBack("A Brief History of Time", "Stephen Hawking", 1988, Genre::NonFiction, 4.3, 120);
        db.EmplaceBack("Dune", "Frank Herbert", 1965, Genre::SciFi, 4.6, 180);
    }

    bookdb::BookDatabase<std::vector<Book>> db;
};

TEST_F(FilterTest, RatingAbove) {
    auto pred = RatingAbove(4.5);
    auto filtered = filterBooks(db.begin(), db.end(), pred);

    EXPECT_EQ(filtered.size(), 3);
    for (const auto &book_ref : filtered) {
        EXPECT_GE(book_ref.get().rating, 4.5);
    }
}

TEST_F(FilterTest, YearBetween) {
    auto pred = YearBetween(1950, 2000);
    auto filtered = filterBooks(db.begin(), db.end(), pred);

    EXPECT_EQ(filtered.size(), 2);
    for (const auto &book_ref : filtered) {
        EXPECT_GE(book_ref.get().year, 1950);
        EXPECT_LE(book_ref.get().year, 2000);
    }
}

TEST_F(FilterTest, GenreIs) {
    auto pred = GenreIs(Genre::Fiction);
    auto filtered = filterBooks(db.begin(), db.end(), pred);

    EXPECT_EQ(filtered.size(), 2);
    for (const auto &book_ref : filtered) {
        EXPECT_EQ(book_ref.get().genre, Genre::Fiction);
    }
}

TEST_F(FilterTest, AllOf) {
    auto pred = all_of(RatingAbove(4.2), bookdb::GenreIs(Genre::Fiction));
    auto filtered = filterBooks(db.begin(), db.end(), pred);

    EXPECT_EQ(filtered.size(), 2);
}

TEST_F(FilterTest, AnyOf) {
    auto pred = any_of(GenreIs(Genre::SciFi), RatingAbove(4.6));
    auto filtered = filterBooks(db.begin(), db.end(), pred);

    EXPECT_GE(filtered.size(), 2);
}

TEST_F(FilterTest, EmptyFilter) {
    auto pred = RatingAbove(5.0);
    auto filtered = filterBooks(db.begin(), db.end(), pred);

    EXPECT_EQ(filtered.size(), 0);
}

// ============================================================================
// Statistics Tests
// ============================================================================

class StatisticsTest : public ::testing::Test {
protected:
    void SetUp() override {
        db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 100);
        db.EmplaceBack("1984", "George Orwell", 1949, Genre::Fiction, 4.2, 150);
        db.EmplaceBack("Sapiens", "Yuval Noah Harari", 2011, Genre::NonFiction, 4.7, 200);
        db.EmplaceBack("A Brief History of Time", "Stephen Hawking", 1988, Genre::NonFiction, 4.3, 120);
        db.EmplaceBack("Dune", "Frank Herbert", 1965, Genre::SciFi, 4.6, 180);
    }

    bookdb::BookDatabase<std::vector<Book>> db;
};

TEST_F(StatisticsTest, CalculateAverageRating) {
    double avg = calculateAverageRating(db);

    // (4.5 + 4.2 + 4.7 + 4.3 + 4.6) / 5 = 4.46
    EXPECT_NEAR(avg, 4.46, 0.01);
}

TEST_F(StatisticsTest, CalculateAverageRatingEmpty) {
    bookdb::BookDatabase<std::vector<Book>> empty_db;
    double avg = calculateAverageRating(empty_db);

    EXPECT_EQ(avg, 0.0);
}

TEST_F(StatisticsTest, BuildAuthorHistogramFlat) {
    auto histogram = buildAuthorHistogramFlat(db);

    EXPECT_EQ(histogram.size(), 5);
    EXPECT_EQ(histogram.at("F. Scott Fitzgerald"), 1);
    EXPECT_EQ(histogram.at("George Orwell"), 1);
}

TEST_F(StatisticsTest, BuildAuthorHistogramFlatHeterogeneousLookup) {
    auto histogram = buildAuthorHistogramFlat(db);

    EXPECT_EQ(histogram.count(std::string_view("Yuval Noah Harari")), 1);

    EXPECT_EQ(histogram.count("Stephen Hawking"), 1);
}

TEST_F(StatisticsTest, CalculateGenreRatings) {
    auto genre_ratings = calculateGenreRatings(db.begin(), db.end());

    EXPECT_EQ(genre_ratings.size(), 3);

    EXPECT_NEAR(genre_ratings.at(Genre::Fiction), 4.35, 0.01);

    EXPECT_NEAR(genre_ratings.at(Genre::NonFiction), 4.5, 0.01);

    EXPECT_NEAR(genre_ratings.at(Genre::SciFi), 4.6, 0.01);
}

TEST_F(StatisticsTest, SampleRandomBooks) {
    auto sample = sampleRandomBooks(db, 3);

    EXPECT_EQ(sample.size(), 3);

    for (const auto &book_ref : sample) {
        EXPECT_FALSE(book_ref.get().title.empty());
    }
}

TEST_F(StatisticsTest, SampleRandomBooksExceedsSize) {
    auto sample = sampleRandomBooks(db, 10);

    EXPECT_EQ(sample.size(), db.size());
}

TEST_F(StatisticsTest, SampleRandomBooksThrowsOnZero) { EXPECT_THROW(sampleRandomBooks(db, 0), std::runtime_error); }

TEST_F(StatisticsTest, GetTopNByRating) {
    auto top = getTopNBy(db, 3, comp::GreaterByRating{});

    EXPECT_EQ(top.size(), 3);

    std::vector<double> ratings;
    for (const auto &book_ref : top) {
        ratings.push_back(book_ref.get().rating);
    }
    std::sort(ratings.rbegin(), ratings.rend());
    EXPECT_NEAR(ratings[0], 4.7, 0.01);
}

TEST_F(StatisticsTest, GetTopNByPopularity) {
    auto top = bookdb::getTopNBy(db, 2, bookdb::comp::GreaterByPopularity{});

    EXPECT_EQ(top.size(), 2);

    for (const auto &book_ref : top) {
        EXPECT_GE(book_ref.get().read_count, 150);
    }
}

TEST_F(StatisticsTest, GetTopNByAuthor) {
    auto top = bookdb::getTopNBy(db, 2, bookdb::comp::LessByAuthor{});

    EXPECT_EQ(top.size(), 2);
}

class BookTest : public ::testing::Test {};

TEST_F(BookTest, BookConstructorFromGenre) {
    Book book(Genre::Fiction);
    EXPECT_EQ(book.genre, Genre::Fiction);
}

TEST_F(BookTest, BookConstructorFromString) {
    Book book("Fiction");
    EXPECT_EQ(book.genre, Genre::Fiction);
}

TEST_F(BookTest, BookConstructorFull) {
    Book book("Test Title", "Test Author", 2020, Genre::SciFi, 4.5, 100);

    EXPECT_EQ(book.title, "Test Title");
    EXPECT_EQ(book.author, "Test Author");
    EXPECT_EQ(book.year, 2020);
    EXPECT_EQ(book.genre, Genre::SciFi);
    EXPECT_EQ(book.rating, 4.5);
    EXPECT_EQ(book.read_count, 100);
}

TEST_F(BookTest, GenreFromStringValid) {
    EXPECT_EQ(bookdb::GenreFromString("Fiction"), Genre::Fiction);
    EXPECT_EQ(bookdb::GenreFromString("NonFiction"), Genre::NonFiction);
    EXPECT_EQ(bookdb::GenreFromString("SciFi"), Genre::SciFi);
    EXPECT_EQ(bookdb::GenreFromString("Biography"), Genre::Biography);
    EXPECT_EQ(bookdb::GenreFromString("Mystery"), Genre::Mystery);
}

TEST_F(BookTest, GenreFromStringInvalid) { EXPECT_EQ(bookdb::GenreFromString("InvalidGenre"), Genre::Unknown); }

class ComparatorTest : public ::testing::Test {};

TEST_F(ComparatorTest, LessByRating) {
    Book book1("Book 1", "Author 1", 2020, Genre::Fiction, 4.5, 100);
    Book book2("Book 2", "Author 2", 2020, Genre::Fiction, 3.5, 100);

    bookdb::comp::LessByRating comp;
    EXPECT_TRUE(comp(book2, book1));
    EXPECT_FALSE(comp(book1, book2));
}

TEST_F(ComparatorTest, LessByPopularity) {
    Book book1("Book 1", "Author 1", 2020, Genre::Fiction, 4.5, 100);
    Book book2("Book 2", "Author 2", 2020, Genre::Fiction, 4.5, 200);

    bookdb::comp::LessByPopularity comp;
    EXPECT_TRUE(comp(book1, book2));
    EXPECT_FALSE(comp(book2, book1));
}

TEST_F(ComparatorTest, LessByAuthor) {
    Book book1("Book 1", "Author A", 2020, Genre::Fiction, 4.5, 100);
    Book book2("Book 2", "Author B", 2020, Genre::Fiction, 4.5, 100);

    bookdb::comp::LessByAuthor comp;
    EXPECT_TRUE(comp(book1, book2));
    EXPECT_FALSE(comp(book2, book1));
}
}  // namespace bookdb