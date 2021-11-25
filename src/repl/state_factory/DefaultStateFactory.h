#ifndef TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_
#define TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_

#include "repl/state_factory/IStateFactory.h"

class DefaultStateFactory : public IStateFactory {
 public:
  DefaultStateFactory(const std::shared_ptr<IPrinter>& printer,
                      const std::shared_ptr<IValidator>& validator);

  std::shared_ptr<IREPLState> GetState(StateEnum se) override;

 private:
  std::shared_ptr<MainREPLState> GetMainREPLState();
  std::shared_ptr<AddNewREPLState> GetAddNewREPLState();
  std::shared_ptr<HelpREPLState> GetHelpREPLState();
  std::shared_ptr<UnknownREPLState> GetUnknownREPLState();

 private:
  std::shared_ptr<MainREPLState> main_repl_state_;
  std::shared_ptr<AddNewREPLState> add_new_repl_state_;
  std::shared_ptr<HelpREPLState> help_repl_state_;
  std::shared_ptr<UnknownREPLState> unknown_repl_state_;

  std::shared_ptr<IPrinter> printer_;
  std::shared_ptr<IValidator> validator_;
};

#endif  // TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_
