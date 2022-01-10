#ifndef TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_
#define TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_

#include "DefaultSmallStep.h"
#include "repl/io_facility/IIoFacility.h"
#include "repl/task_steps/ISmallStepFactory.h"

namespace task_manager {
class DefaultSmallStepFactory : public ISmallStepFactory {
 public:
  explicit DefaultSmallStepFactory(
      const std::shared_ptr<IIoFacility>& io_facility,
      const std::shared_ptr<IValidator>& validator);

  std::shared_ptr<ISmallStep> GetReadTitleSmallStep() override;
  std::shared_ptr<ISmallStep> GetReadDateSmallStep() override;
  std::shared_ptr<ISmallStep> GetReadPrioritySmallStep() override;
  std::shared_ptr<ISmallStep> GetReadStateSmallStep() override;

 private:
  std::shared_ptr<DefaultReadTitleSmallStep> GetDefaultReadTitleSmallStep();
  std::shared_ptr<DefaultReadDateSmallStep> GetDefaultReadDateSmallStep();
  std::shared_ptr<DefaultReadPrioritySmallStep>
  GetDefaultReadPrioritySmallStep();

  std::shared_ptr<DefaultReadStateSmallStep> GetDefaultReadStateSmallStep();

 private:
  std::shared_ptr<DefaultReadTitleSmallStep> default_read_title_small_step_;
  std::shared_ptr<DefaultReadDateSmallStep> default_read_date_small_step_;
  std::shared_ptr<DefaultReadPrioritySmallStep>
      default_read_priority_small_step_;
  std::shared_ptr<DefaultReadStateSmallStep> default_read_state_small_step_;

  std::shared_ptr<IValidator> validator_;
  std::shared_ptr<IIoFacility> io_facility_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_
