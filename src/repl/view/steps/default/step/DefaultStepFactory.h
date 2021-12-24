#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_

#include "repl/io_facility/IIoFacility.h"
#include "repl/validator/IValidator.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/IStepFactory.h"
#include "repl/view/steps/default/step/DefaultStep.h"

namespace task_manager {
class DefaultStepFactory final : public IStepFactory {
 public:
  DefaultStepFactory(const std::shared_ptr<IIoFacility> &io_facility,
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
  std::shared_ptr<DefaultAddTaskStep> default_add_task_step_;
  std::shared_ptr<DefaultEditTaskStep> default_edit_task_step_;
  std::shared_ptr<DefaultCompleteTaskStep> default_complete_task_step_;
  std::shared_ptr<DefaultDeleteTaskStep> default_delete_task_step_;
  std::shared_ptr<DefaultShowStep> default_show_step_;
  std::shared_ptr<DefaultShowHelpStep> default_show_help_step_;
  std::shared_ptr<DefaultReportMessageStep> default_report_message_step_;

  std::shared_ptr<ISmallStepFactory> state_factory_;
  std::shared_ptr<IValidator> validator_;
  std::shared_ptr<IIoFacility> io_facility_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_
