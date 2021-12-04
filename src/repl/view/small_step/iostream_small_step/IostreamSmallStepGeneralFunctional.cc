#include "IostreamSmallStepGeneralFunctional.h"

#include <iostream>

std::string PrintAndGet(const std::string& str) {
  std::cout << '[' << str << ']' << ": ";
  std::string result;
  std::getline(std::cin, result);
  return result;
}
