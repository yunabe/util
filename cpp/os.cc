#include "os.h"

#include <dirent.h>  // opendir
#include <limits.h>  // PATH_MAX
#include <unistd.h>  // getcwd
#include <stdlib.h> // exit
#include <string.h>  // strcmp
#include <sys/types.h>  // opendir, wait
#include <sys/wait.h>  // wait

#include <map>

#include "strutil.h"

using std::map;

static const char* kCurrentDirectory = ".";
static const char* kParentDirectory = "..";

string os::getcwd() {
  char cwd[PATH_MAX];
  ::getcwd(cwd, PATH_MAX);
  return string(cwd);
}

pid_t os::fork() {
  return ::fork();
}

pid_t os::wait(int* status) {
  return ::wait(status);
}

void os::exit(int status) {
  ::exit(status);
}

void os::_exit(int status) {
  ::_exit(status);
}

int os::rmdir(const string& path) {
  return ::rmdir(path.c_str());
}

bool os::listdir(const string& path, vector<string>* children) {
  DIR* d = opendir(path.c_str());
  if (d == NULL) {
    return false;
  }
  struct dirent* child;
  while (child = readdir(d)) {
    // Ignores . and ..
    if (strcmp(kCurrentDirectory, child->d_name) == 0 ||
        strcmp(kParentDirectory, child->d_name) == 0) {
      continue;
    }
    children->push_back(string(child->d_name));
  }
  closedir(d);
  return true;
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

namespace {
  class EnvironManager {
  public:
    EnvironManager() {
      char** env = environ;
      while (*env != NULL) {
        string name_value = *env;
        ++env;
        size_t pos = name_value.find('=');
        if (pos == string::npos) {
          // Error, just ignore.
          continue;
        }
        string name = name_value.substr(0, pos);
        string value = name_value.substr(pos + 1);
        env_[name] = value;
      }
    }

    map<string, string> env_;
  };

  EnvironManager environ_manager;
}

const string& os::environ(const string& name,
                          const string& default_value) {
  map<string, string>::const_iterator iter = environ_manager.env_.find(name);
  if (iter != environ_manager.env_.end()) {
    return iter->second;
  } else {
    return default_value;
  }
}

const string& os::environ(const string& name) {
  static const string empty;
  return os::environ(name, empty);
}

void os::set_environ(const string& name, const string& value) {
  environ_manager.env_[name] = value;
  setenv(name.c_str(), value.c_str(), 1);
}

void os::clear_environ(const string& name) {
  environ_manager.env_.erase(name);
  unsetenv(name.c_str());
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
  strutil::split(path, '/', &comps);
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
  string joined = strutil::join("/", new_comps);
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

void os::path::split(const string& path, string* head, string* tail) {
  size_t i = path.rfind('/');
  if (i == string::npos) {
    head->clear();
    *tail = path;
  } else {
    *head = os::path::normpath(path.substr(0, i + 1));
    *tail = os::path::normpath(path.substr(i + 1, path.size() - i - 1));
  }
}

string os::path::basename(const string& path) {
  string head, tail;
  os::path::split(path, &head, &tail);
  return tail;
}

string os::path::dirname(const string& path) {
  string head, tail;
  os::path::split(path, &head, &tail);
  return head;
}
