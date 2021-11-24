#include "DefaultStateFactory.h"

DefaultStateFactory::DefaultStateFactory(
    const std::shared_ptr<IPrinter>& printer,
    const std::shared_ptr<IValidator>& validator)
    : printer_(printer), validator_(validator) {}

std::shared_ptr<IREPLState> DefaultStateFactory::GetState(StateEnum se) {
  switch (se) {
    case StateEnum::kDefault:
      if (main_repl_state_ == nullptr) {
        main_repl_state_ =
            std::make_shared<MainREPLState>(printer_, validator_);
      }
      return main_repl_state_;

    case StateEnum::kMain:
      if (main_repl_state_ == nullptr) {
        main_repl_state_ =
            std::make_shared<MainREPLState>(printer_, validator_);
      }
      return main_repl_state_;

    case StateEnum::kAdd:
      if (add_new_repl_state_ == nullptr) {
        add_new_repl_state_ =
            std::make_shared<AddNewREPLState>(printer_, validator_);
      }
      return add_new_repl_state_;

    case StateEnum::kHelp:
      if (help_repl_state_ == nullptr) {
        help_repl_state_ = std::make_shared<HelpREPLState>(printer_);
      }
      return help_repl_state_;

    case StateEnum::kExit:
      return {};

    case StateEnum::kUnknown:
      return {};
  }
}
