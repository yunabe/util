#include "os.h"

#include <limits.h>  // PATH_MAX
#include <unistd.h>  // getcwd

#include "strutil.h"

string os::getcwd() {
  char cwd[PATH_MAX];
  ::getcwd(cwd, PATH_MAX);
  return string(cwd);
}

bool os::path::isabs(const string& path) {
  return !path.empty() && path[0] == '/';
}

string os::path::join(const vector<string>& paths) {
  string result;
  for (int i = 0; i < paths.size(); ++i) {
    if (result.empty() || os::path::isabs(paths[i])) {
      result = paths[i];
      continue;
    }
    if (result[result.size() - 1] != '/') {
      result.append("/");
    }
    result.append(paths[i]);
  }
  return result;
}

string os::path::join(const string& a, const string& b) {
  vector<string> paths;
  paths.push_back(a);
  paths.push_back(b);
  return os::path::join(paths);
}

string os::path::normpath(const string& path) {
  // Ported from posixpath.py
  if (path.empty()) {
    return ".";
  }
  int initial_slashes = path[0] == '/' ? 1 : 0;
  if (initial_slashes > 0 && path.size() > 1 && path[1] == '/' &&
      (path.size() == 2 || path[2] != '/')) {
    // POSIX allows one or two initial slashes, but treats three or more
    // as single slash.
    initial_slashes = 2;
  }
  vector<string> comps;
  strutil::SplitString(path, '/', &comps);
  vector<string> new_comps;
  for (int i = 0; i < comps.size(); ++i) {
    const string& comp = comps[i];
    if (comp == "" || comp == ".") {
      continue;
    }
    if (comp != ".." ||
        initial_slashes == 0 && new_comps.size() == 0 ||
        new_comps.size() > 0 && new_comps[new_comps.size() - 1] == "..") {
      new_comps.push_back(comp);
    } else if (new_comps.size() > 0) {
      // vector::pop_back required size() > 0.
      new_comps.pop_back();
    }
  }
  string joined = strutil::JoinString("/", new_comps);
  switch (initial_slashes) {
  case 1:
    return "/" + joined;
  case 2:
    return "//" + joined;
  default:
    return joined;
  }
}

string os::path::abspath(const string& path) {
  string newpath;
  if (!os::path::isabs(path)) {
    newpath = os::path::join(os::getcwd(), path);
  } else {
    newpath = path;
  }
  return os::path::normpath(newpath);
}
