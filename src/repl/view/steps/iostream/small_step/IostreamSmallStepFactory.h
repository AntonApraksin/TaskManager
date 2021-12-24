#ifndef TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_
#define TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_

#include "repl/io_facility/IIoFacility.h"
#include "repl/view/steps/ISmallStepFactory.h"

namespace task_manager {
class IostreamSmallStepFactory : public ISmallStepFactory {
 public:
  explicit IostreamSmallStepFactory(
      const std::shared_ptr<IIoFacility>& io_facility,
      const std::shared_ptr<IValidator>& validator);

  std::shared_ptr<ISmallStep> GetReadTitleSmallStep() override;
  std::shared_ptr<ISmallStep> GetReadDateSmallStep() override;
  std::shared_ptr<ISmallStep> GetReadPrioritySmallStep() override;
  std::shared_ptr<ISmallStep> GetReadStateSmallStep() override;

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
  std::shared_ptr<IIoFacility> io_facility_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_
