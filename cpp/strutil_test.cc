#include "strutil.h"

#include <gtest/gtest.h>

class SplitStringTest : public testing::Test {
};

TEST_F(SplitStringTest, SplitString) {
  const char* s = "a/b/c";
  char sep = '/';
  vector<string> results;
  strutil::SplitString(s, sep, &results);
  ASSERT_EQ(3, results.size());
  EXPECT_EQ("a", results[0]);
  EXPECT_EQ("b", results[1]);
  EXPECT_EQ("c", results[2]);
}

TEST_F(SplitStringTest, TrailingSeparator) {
  const char* s = "a/b/c/";
  char sep = '/';
  vector<string> results;
  strutil::SplitString(s, sep, &results);
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
  strutil::SplitString(s, sep, &results);
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
  strutil::SplitString(s, sep, &results);
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
  EXPECT_EQ("a/b/c", strutil::JoinString("/", list));
}

class FormatTest : public testing::Test {
};

TEST_F(FormatTest, Format) {
  EXPECT_EQ("2011/01/04",
            strutil::Format("%d/%02d/%02d", 2011, 1, 4));
  EXPECT_EQ("Hello world",
            strutil::Format("Hello %s", "world"));
}
