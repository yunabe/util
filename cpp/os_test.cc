#include "os.h"

#include <gtest/gtest.h>

class OsTest : public testing::Test {
};

TEST_F(OsTest, GetCwd) {
  string cwd = os::getcwd();
  chdir("/tmp");
  EXPECT_EQ("/tmp", os::getcwd());
  chdir(cwd.c_str());
}

class OsPathTest : public testing::Test {
public:
  OsPathTest() : original_cwd_(os::getcwd()) {
    chdir("/tmp");    
  }

  ~OsPathTest() {
    chdir(original_cwd_.c_str());
  }

private:
  string original_cwd_;
};

TEST_F(OsPathTest, IsAbs) {
  EXPECT_EQ(true, os::path::isabs("/tmp"));
  EXPECT_EQ(false, os::path::isabs("~/.emacs"));
  EXPECT_EQ(false, os::path::isabs("../"));
  EXPECT_EQ(false, os::path::isabs("./test"));
}

TEST_F(OsPathTest, PathJoin) {
  EXPECT_EQ("/home/yunabe", os::path::join("/home", "yunabe"));
  EXPECT_EQ("/tmp", os::path::join("/home", "/tmp"));
  EXPECT_EQ("/home/./", os::path::join("/home", "./"));
  EXPECT_EQ("/home/../", os::path::join("/home", "../"));
}

TEST_F(OsPathTest, NormPath) {
  EXPECT_EQ("b/a", os::path::normpath("b/c/../a"));
  EXPECT_EQ("a/d", os::path::normpath("a/b/c/../../d"));
  EXPECT_EQ("d", os::path::normpath("a/b/c/../../../d"));

  EXPECT_EQ("a/b", os::path::normpath("./a/b"));
  EXPECT_EQ("a/b", os::path::normpath("a/b/"));
  EXPECT_EQ("/a/b", os::path::normpath("/a/b"));
  EXPECT_EQ("//a/b", os::path::normpath("//a/b"));
  EXPECT_EQ("../a/b", os::path::normpath("../a/b"));

  EXPECT_EQ("//", os::path::normpath("//"));
  EXPECT_EQ("/", os::path::normpath("/"));
  EXPECT_EQ("/", os::path::normpath("///"));

  EXPECT_EQ("/a", os::path::normpath("/../a"));
}

TEST_F(OsPathTest, AbsPath) {
  EXPECT_EQ("/tmp/a", os::path::abspath("a"));
  EXPECT_EQ("/home", os::path::abspath("../home"));
  EXPECT_EQ("/tmp/a", os::path::abspath("./a"));
  EXPECT_EQ("/tmp/~/a", os::path::abspath("~/a"));
  EXPECT_EQ("/home", os::path::abspath("/home"));
}
