#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_

#include "repl/io_facility/IIoFacility.h"
#include "repl/validator/IValidator.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/IStepFactory.h"
#include "repl/view/steps/iostream/step/IostreamStep.h"

namespace task_manager {
class IostreamStepFactory final : public IStepFactory {
 public:
  IostreamStepFactory(const std::shared_ptr<IIoFacility> &io_facility,
                      const std::shared_ptr<IValidator> &validator,
                      const std::shared_ptr<ISmallStepFactory> &state_factory);

  std::shared_ptr<IStep> GetAddTaskStep() override;
  std::shared_ptr<IStep> GetAddTaskStep(SolidTask) override;

  std::shared_ptr<IStep> GetEditTaskStep(SolidTask) override;
  std::shared_ptr<IStep> GetCompleteTaskStep(SolidTasks) override;
  std::shared_ptr<IStep> GetDeleteTaskStep(SolidTasks) override;

  std::shared_ptr<IStep> GetShowStep(SolidTasks) override;

  std::shared_ptr<IStep> GetShowHelpStep() override;

  std::shared_ptr<IStep> GetReportMessageStep(std::string) override;

 private:
  std::shared_ptr<IostreamAddTaskStep> iostream_add_task_step_;
  std::shared_ptr<IostreamEditTaskStep> iostream_edit_task_step_;
  std::shared_ptr<IostreamCompleteTaskStep> iostream_complete_task_step_;
  std::shared_ptr<IostreamDeleteTaskStep> iostream_delete_task_step_;
  std::shared_ptr<IostreamShowStep> iostream_show_step_;
  std::shared_ptr<IostreamShowHelpStep> iostream_show_help_step_;
  std::shared_ptr<IostreamReportMessageStep> iostream_report_message_step_;

  std::shared_ptr<ISmallStepFactory> state_factory_;
  std::shared_ptr<IValidator> validator_;
  std::shared_ptr<IIoFacility> io_facility_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_
