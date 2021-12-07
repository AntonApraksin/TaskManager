#include "IostreamIoFacility.h"

#include <iostream>

void IostreamIoFacility::Print(const std::string &string) {
  std::cout << string;
}

std::string IostreamIoFacility::GetLine() {
  std::string input;
  std::getline(std::cin, input);
  return input;
}
