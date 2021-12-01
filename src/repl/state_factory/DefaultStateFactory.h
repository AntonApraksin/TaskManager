#ifndef TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_
#define TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_

#include "repl/state_factory/IStateFactory.h"

class DefaultStateFactory : public IStateFactory {
 public:
  DefaultStateFactory(const std::shared_ptr<IIOFacility>& printer,
                      const std::shared_ptr<IValidator>& validator);

  std::shared_ptr<IREPLSubState> GetREPLState(SubStateEnum sse) override;

 private:
  std::shared_ptr<ReadTitleREPLSubState> GetReadTitleREPLSubState();
  std::shared_ptr<ReadDateREPLSubState> GetReadDateREPLSubState();
  std::shared_ptr<ReadConfirmationREPLSubState>
  GetReadConfirmationREPLSubState();
  std::shared_ptr<ReadPriorityREPLSubState> GetReadPriorityREPLSubState();

 private:
  std::shared_ptr<ReadTitleREPLSubState> read_title_repl_sub_state_;
  std::shared_ptr<ReadDateREPLSubState> read_date_repl_sub_state_;
  std::shared_ptr<ReadConfirmationREPLSubState>
      read_confirmation_repl_sub_state_;
  std::shared_ptr<ReadPriorityREPLSubState> read_priority_repl_sub_state_;

  std::shared_ptr<IIOFacility> printer_;
  std::shared_ptr<IValidator> validator_;
};

#endif  // TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_
