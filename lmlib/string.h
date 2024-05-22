#include <string>

namespace lm {
namespace string {

// split string by character delimiter
template <typename Container>
Container &split(const std::string &str, char delim, Container &res) {

  size_t i, j;

  res.clear();
  i = j = 0;
  for (std::string::const_iterator it = str.begin(); it != str.end();
       ++it, ++i) {
    if (*it == delim) {
      res.push_back(str.substr(j, i - j));
      j = i + 1;
    }
  }
  res.push_back(str.substr(j));

  return res;
};

// Split str by delim only if delim is outside double quotes
template <typename Container>
Container &smartSplit(const std::string &str, char delim, Container &res) {

  size_t i, p = 0;
  bool insideQuotes = false;

  res.clear();

  for (i = 0; i < str.size(); i++) {

    // if character is delim
    if (str[i] == delim) {
      // and not inside quotes
      if (insideQuotes == false) {
        // extract substring from p to i-1
        res.push_back(str.substr(p, i - p));
        p = i + 1;
      }
    }
    // if character is a quote
    else if (str[i] == '\"') {
      // make sure it's a real quote
      if (i != 0 && str[i - 1] != '\\') {
        insideQuotes = !insideQuotes;
      }
    }
  }

  res.push_back(str.substr(p));

  return res;
};

template <typename Container>
Container &splitInTwo(const std::string &str, char delim, Container &res) {

  size_t i, p = 0;

  res.clear();

  for (i = 0; i < str.size(); i++) {

    // if character is delim
    if (str[i] == delim) {
      // extract substring from p to i-1
      res.push_back(str.substr(p, i - p));
      p = i + 1;
      break;
    }
  }

  res.push_back(str.substr(p));

  return res;
};

// convert string to uint16_t and set ok to true;
// if the conversion fails, ok is set to false and the return value is 0.
uint16_t toUShort(const std::string &str, bool *ok = nullptr, int base = 10);

std::string &trimLeft(std::string &str);
std::string &trimRight(std::string &str);
std::string &trim(std::string &str);
std::string &toLower(std::string &str);
std::string &toUpper(std::string &str);
bool toBool(std::string &str);

char *c_str(const std::string &str);

std::string &replace(std::string &str, const std::string &before,
                     const std::string &after);

} // namespace string
} // namespace lm
