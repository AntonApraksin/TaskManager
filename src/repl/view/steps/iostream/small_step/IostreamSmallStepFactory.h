#ifndef TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_
#define TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_

#include "repl/view/steps/ISmallStepFactory.h"

class IostreamSmallStepFactory : public ISmallStepFactory {
 public:
  explicit IostreamSmallStepFactory(
      const std::shared_ptr<IValidator>& validator);

  std::shared_ptr<IostreamSmallStep> GetREPLState(
      IostreamSmallStepEnum sse) override;

 private:
  std::shared_ptr<IostreamReadTitleSmallStep> GetIostreamReadTitleSmallStep();
  std::shared_ptr<IostreamReadDateSmallStep> GetIostreamReadDateSmallStep();
  std::shared_ptr<IostreamReadPrioritySmallStep>
  GetIostreamReadPrioritySmallStep();

  std::shared_ptr<IostreamReadStateSmallStep> GetIostreamReadStateSmallStep();

 private:
  std::shared_ptr<IostreamReadTitleSmallStep> iostream_read_title_small_step_;
  std::shared_ptr<IostreamReadDateSmallStep> iostream_read_date_small_step_;
  std::shared_ptr<IostreamReadPrioritySmallStep>
      iostream_read_priority_small_step_;
  std::shared_ptr<IostreamReadStateSmallStep> iostream_read_state_small_step_;

  std::shared_ptr<IValidator> validator_;
};

#endif  // TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_
