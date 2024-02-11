#pragma once
#include <iomanip>
#include <iostream>
#include <sstream>

#include "inttypes.h"

namespace util {
template <typename T>
void printHex(T value, uint32_t width) {
  std::cout << std::setfill('0') << std::setw(width) << std::uppercase
            << std::hex << static_cast<int32_t>(value) << std::dec
            << std::nouppercase << "h";
}

template <typename T>
std::string printHexToString(T value, uint32_t width) {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(width) << std::uppercase << std::hex
     << static_cast<int32_t>(value) << std::dec << std::nouppercase << "h";

  return ss.str();
}
}  // namespace util