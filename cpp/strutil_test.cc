#include "strutil.h"

#include <gtest/gtest.h>

class SplitStringTest : public testing::Test {
};

TEST_F(SplitStringTest, SplitString) {
  const char* s = "a/b/c";
  char sep = '/';
  vector<string> results;
  strutil::split(s, sep, &results);
  ASSERT_EQ(3, results.size());
  EXPECT_EQ("a", results[0]);
  EXPECT_EQ("b", results[1]);
  EXPECT_EQ("c", results[2]);
}

TEST_F(SplitStringTest, TrailingSeparator) {
  const char* s = "a/b/c/";
  char sep = '/';
  vector<string> results;
  strutil::split(s, sep, &results);
  ASSERT_EQ(4, results.size());
  EXPECT_EQ("a", results[0]);
  EXPECT_EQ("b", results[1]);
  EXPECT_EQ("c", results[2]);
  EXPECT_EQ("", results[3]);
}

TEST_F(SplitStringTest, PrecedingSeparator) {
  const char* s = "/a/b/c";
  char sep = '/';
  vector<string> results;
  strutil::split(s, sep, &results);
  ASSERT_EQ(4, results.size());
  EXPECT_EQ("", results[0]);
  EXPECT_EQ("a", results[1]);
  EXPECT_EQ("b", results[2]);
  EXPECT_EQ("c", results[3]);
}

TEST_F(SplitStringTest, RepeatedSeparator) {
  const char* s = "a//b";
  char sep = '/';
  vector<string> results;
  strutil::split(s, sep, &results);
  ASSERT_EQ(3, results.size());
  EXPECT_EQ("a", results[0]);
  EXPECT_EQ("", results[1]);
  EXPECT_EQ("b", results[2]);
}

class JoinStringTest : public testing::Test {
};

TEST_F(JoinStringTest, JoinString) {
  vector<string> list;
  list.push_back("a");
  list.push_back("b");
  list.push_back("c");
  EXPECT_EQ("a/b/c", strutil::join("/", list));
}

class FormatTest : public testing::Test {
};

TEST_F(FormatTest, Format) {
  EXPECT_EQ("2011/01/04",
            strutil::format("%d/%02d/%02d", 2011, 1, 4));
  EXPECT_EQ("Hello world",
            strutil::format("Hello %s", "world"));
}

TEST_F(FormatTest, strip) {
  EXPECT_EQ("a b c", strutil::strip(" a b c "));
  EXPECT_EQ("a b c", strutil::strip("a b c "));
  EXPECT_EQ("a b c", strutil::strip(" a b c"));
  EXPECT_EQ("", strutil::strip(""));
  EXPECT_EQ("", strutil::strip(" "));
  EXPECT_EQ("", strutil::strip("  "));

  EXPECT_EQ("hello", strutil::strip("\thello\n"));
}

TEST_F(FormatTest, rstrip) {
  EXPECT_EQ(" a b c", strutil::rstrip(" a b c "));
  EXPECT_EQ("a b c", strutil::rstrip("a b c "));
  EXPECT_EQ(" a b c", strutil::rstrip(" a b c"));
  EXPECT_EQ("", strutil::rstrip(""));
  EXPECT_EQ("", strutil::rstrip(" "));
  EXPECT_EQ("", strutil::rstrip("  "));
}

TEST_F(FormatTest, lstrip) {
  EXPECT_EQ("a b c ", strutil::lstrip(" a b c "));
  EXPECT_EQ("a b c ", strutil::lstrip("a b c "));
  EXPECT_EQ("a b c", strutil::lstrip(" a b c"));
  EXPECT_EQ("", strutil::lstrip(""));
  EXPECT_EQ("", strutil::lstrip(" "));
  EXPECT_EQ("", strutil::lstrip("  "));
}
