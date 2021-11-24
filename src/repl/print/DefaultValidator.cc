#include "DefaultValidator.h"

#include <algorithm>
#include <cctype>

StateEnum DefaultValidator::MatchState(const std::string& str) {
  std::string state(str);
  std::transform(state.cbegin(), state.cend(), state.begin(),
                 [](auto c) { return std::tolower(c); });
  if (state == "add") {
    return StateEnum::kAdd;
  }
  if (state == "exit") {
    return StateEnum::kExit;
  }
  if (state == "help") {
    return StateEnum::kHelp;
  }
  return StateEnum::kUnknown;
}
