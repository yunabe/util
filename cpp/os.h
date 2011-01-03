#ifndef YUNABE_UTIL_CPP_OS_H_
#define YUNABE_UTIL_CPP_OS_H_

// Backport of Python os module

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace os {
  string getcwd();

  namespace path {
    string abspath(const string& path);
    
    bool isabs(const string& path);

    string join(const vector<string>& paths);

    string join(const string& a, const string& b);
    
    string normpath(const string& path);
  }
}

#endif  // YUNABE_UTIL_CPP_OS_H_