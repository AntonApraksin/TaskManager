#ifndef TASKMANAGER_SRC_REPL_PRINT_IVALIDATOR_H_
#define TASKMANAGER_SRC_REPL_PRINT_IVALIDATOR_H_

#include <string>

enum class StateEnum {
  kMain,
  kAdd,
  kExit,
  kHelp,

  kUnknown,
  kDefault,
};

class IValidator {
 public:
  virtual StateEnum MatchState(const std::string& str) = 0;

  virtual ~IValidator() {}
};

#endif  // TASKMANAGER_SRC_REPL_PRINT_IVALIDATOR_H_
