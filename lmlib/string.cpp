#include "string.h"

#include <locale>
#include <sstream>
#include <algorithm>

uint16_t lm::string::toUShort(const std::string &str, bool *ok, int base) {
  uint16_t res;
  try {
    res = static_cast<uint16_t>(std::stoul(str, nullptr, 16));
    if (ok != nullptr)
      *ok = true;
    return res;
  } catch (...) {
    if (ok != nullptr)
      *ok = false;
    return 0;
  }
}

std::string &lm::string::trimLeft(std::string &str) {
  auto it2 = std::find_if(str.begin(), str.end(), [](char ch) {
    return !std::isspace<char>(ch, std::locale::classic());
  });
  str.erase(str.begin(), it2);
  return str;
}

std::string &lm::string::trimRight(std::string &str) {
  auto it1 = std::find_if(str.rbegin(), str.rend(), [](char ch) {
    return !std::isspace<char>(ch, std::locale::classic());
  });
  str.erase(it1.base(), str.end());
  return str;
}

std::string &lm::string::trim(std::string &str) {
  trimLeft(str);
  trimRight(str);
  return str;
}

std::string &lm::string::toLower(std::string &str) {
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
}

std::string &lm::string::toUpper(std::string &str) {
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  return str;
}

bool lm::string::toBool(std::string &str) {
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  std::istringstream is(str);
  bool b;
  is >> std::boolalpha >> b;
  return b;
}

char *lm::string::c_str(const std::string &str) {
  char *buf = (char *)malloc((str.size() + 1) * sizeof(char));
  if (buf != nullptr) {
    str.copy(buf, str.size());
    buf[str.size()] = '\0';
  }

  return buf;
}

std::string &lm::string::replace(std::string &str, const std::string &before,
                                 const std::string &after) {
  if (before.empty())
    return str;
  size_t start_pos = 0;
  while ((start_pos = str.find(before, start_pos)) != std::string::npos) {
    str.replace(start_pos, before.length(), after);
    start_pos += after.length(); // In case 'to' contains 'from', like replacing
                                 // 'x' with 'yx'
  }

  return str;
}