#ifndef TASK_ID_H
#define TASK_ID_H
#include <charconv>
#include <stdexcept>
#include <string_view>
inline int parseTaskId(std::string_view value) {
  int id = 0;
  auto result = std::from_chars(value.data(), value.data() + value.size(), id);
  if (result.ec != std::errc{} || result.ptr != value.data() + value.size() || id <= 0) {
    throw std::invalid_argument("ID must be a positive integer");
  }
  return id;
}
#endif
