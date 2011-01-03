#include <string>
#include <vector>

using std::string;
using std::vector;

namespace strutil {

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
