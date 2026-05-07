#include "book_database.hpp"
#include <gtest/gtest.h>

TEST(TestComponentName, SimpleCheck) {
    bookdb::BookDatabase bookDB = {};
    bookDB.EmplaceBack("Fiction");
    ASSERT_EQ(bookDB.size(), 1);
}
