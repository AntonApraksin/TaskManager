#include "DefaultStateFactory.h"

DefaultStateFactory::DefaultStateFactory(
    const std::shared_ptr<IPrinter>& printer,
    const std::shared_ptr<IValidator>& validator)
    : printer_(printer), validator_(validator) {}

std::shared_ptr<IREPLState> DefaultStateFactory::GetState(StateEnum se) {
  switch (se) {
    case StateEnum::kDefault:
      return GetMainREPLState();
    case StateEnum::kMain:
      return GetMainREPLState();
    case StateEnum::kAdd:
      return GetAddNewREPLState();
    case StateEnum::kHelp:
      return GetHelpREPLState();
    case StateEnum::kExit:
      return {};

    case StateEnum::kUnknown:
      return GetUnknownREPLState();
  }
}

std::shared_ptr<MainREPLState> DefaultStateFactory::GetMainREPLState() {
  if (main_repl_state_ == nullptr) {
    main_repl_state_ = std::make_shared<MainREPLState>(printer_, validator_);
  }
  return main_repl_state_;
}

std::shared_ptr<AddNewREPLState> DefaultStateFactory::GetAddNewREPLState() {
  if (add_new_repl_state_ == nullptr) {
    add_new_repl_state_ =
        std::make_shared<AddNewREPLState>(printer_, validator_);
  }
  return add_new_repl_state_;
}

std::shared_ptr<HelpREPLState> DefaultStateFactory::GetHelpREPLState() {
  if (help_repl_state_ == nullptr) {
    help_repl_state_ = std::make_shared<HelpREPLState>(printer_);
  }
  return help_repl_state_;
}
std::shared_ptr<UnknownREPLState> DefaultStateFactory::GetUnknownREPLState() {
  if (unknown_repl_state_ == nullptr) {
    unknown_repl_state_ = std::make_shared<UnknownREPLState>(printer_);
  }
  return unknown_repl_state_;
}
