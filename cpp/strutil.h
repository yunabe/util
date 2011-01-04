#ifndef YUNABE_UTIL_CPP_STRUTIL_H_
#define YUNABE_UTIL_CPP_STRUTIL_H_

#include <string>
#include <vector>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

using std::string;
using std::vector;

namespace strutil {

static string Format(const char* fmt, ...) {
  va_list args;
  char* buffer = NULL;
  va_start(args, fmt);
  int rc = vasprintf(&buffer, fmt, args);
  va_end(args);
  string result;
  if (rc >= 0) {
    result = buffer;
  }
  if (buffer != NULL) {
    free(buffer);
  }
  return result;
}

static void SplitString(const string& s, char sep, vector<string>* result) {
  size_t seek_pos = 0;
  while (seek_pos < s.size() + 1) {
    size_t sep_pos = s.find_first_of(sep, seek_pos);
    if (sep_pos == string::npos) {
      sep_pos = s.size();
    }
    result->push_back(s.substr(seek_pos, sep_pos - seek_pos));
    seek_pos = sep_pos + 1;
  }
}

static string JoinString(const string& combinator, const vector<string>& list) {
  if (list.size() == 0) {
    return "";
  }
  size_t total_size = 0;
  for (int i = 0; i < list.size(); ++i) {
    total_size += list[i].length();
  }
  total_size += combinator.length() * (list.size() - 1);
  string result;
  result.reserve(total_size);
  for (int i = 0; i < list.size(); ++i) {
    if(i != 0) {
      result.append(combinator);
    }
    result.append(list[i]);
  }
  return result;
}
}

#endif  // YUNABE_UTIL_CPP_STRUTIL_H_
