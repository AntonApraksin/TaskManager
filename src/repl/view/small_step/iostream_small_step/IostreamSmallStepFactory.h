#ifndef TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_
#define TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_

#include "repl/view/small_step/ISmallStepFactory.h"

class IostreamSmallStepFactory : public ISmallStepFactory {
 public:
  IostreamSmallStepFactory(const std::shared_ptr<IIOFacility>& printer,
                           const std::shared_ptr<IValidator>& validator);

  std::shared_ptr<IostreamSmallStep> GetREPLState(
      IostreamSmallStepEnum sse) override;

 private:
  std::shared_ptr<IostreamReadTitleSmallStep> GetIostreamReadTitleSmallStep();
  std::shared_ptr<IostreamReadDateSmallStep> GetIostreamReadDateSmallStep();
  std::shared_ptr<IostreamReadPrioritySmallStep>
  GetIostreamReadPrioritySmallStep();

 private:
  std::shared_ptr<IostreamReadTitleSmallStep> read_title_repl_sub_state_;
  std::shared_ptr<IostreamReadDateSmallStep> read_date_repl_sub_state_;
  std::shared_ptr<IostreamReadPrioritySmallStep> read_priority_repl_sub_state_;

  std::shared_ptr<IIOFacility> printer_;
  std::shared_ptr<IValidator> validator_;
};

#endif  // TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_
