#include "test/tests.h"
#include <string.h>
#include <util/string.h>
using util::string;

TEST_CASE(string_strlen) {
  const char *str = "12345678";
  ASSERT_EQ(strlen(str), 8);
}

TEST_CASE(string_eq) {
  string a = "asdf";
  string b = "fdsa";
  string c = "asdfg";
  string d = "asdf";
  ASSERT(a == d);
  ASSERT(a != b);
  ASSERT(a != c);
}

TEST_CASE(string_concat) {
  string a = "abcd";
  string b = "efgh";
  string c = a + b;
  ASSERT(c == "abcdefgh");
  string d = "1234";
  d += a;
  ASSERT(d == "1234abcd");
}

TEST_CASE(string_slice) {
  string a = "123456789";
  ASSERT(a.slice(0, 5) == "12345");
  ASSERT(a.slice(2, 5) == "345");
}

TEST_CASE(string_split) {
  string a = "home/dir/path";

  auto a_split = a.split('/');
  ASSERT(a_split.at(0) == "home");
  ASSERT(a_split.at(1) == "dir");
  ASSERT(a_split.at(2) == "path");

  string b = "/home/dir/path";

  auto b_split = b.split('/');
  ASSERT(b_split.at(0) == "");
  ASSERT(b_split.at(1) == "home");
  ASSERT(b_split.at(2) == "dir");
  ASSERT(b_split.at(3) == "path");

  string c = "home/dir/path/";
  auto c_split = c.split('/');
  ASSERT(c_split.at(0) == "home");
  ASSERT(c_split.at(1) == "dir");
  ASSERT(c_split.at(2) == "path");
  ASSERT(c_split.at(3) == "");
  // TODO: memory leaks like hell here
}
