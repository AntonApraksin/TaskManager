#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_

#include "IostreamStep.h"
#include "repl/io_facility/IIoFacility.h"
#include "repl/validator/IValidator.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/IStepFactory.h"

class IostreamStepFactory final : public IStepFactory {
 public:
  IostreamStepFactory(const std::shared_ptr<IIoFacility> &io_facility,
                      const std::shared_ptr<IValidator> &validator,
                      const std::shared_ptr<ISmallStepFactory> &state_factory);

  std::shared_ptr<IStep> GetAddTaskStep() override;
  std::shared_ptr<IStep> GetAddTaskStep(Task) override;

  std::shared_ptr<IStep> GetEditTaskStep(
      IEditTaskStep::TaskWrapperRef) override;
  std::shared_ptr<IStep> GetCompleteTaskStep(
      ICompleteTaskStep::TaskWrappers) override;
  std::shared_ptr<IStep> GetDeleteTaskStep(
      IDeleteTaskStep::TaskWrappers) override;

  std::shared_ptr<IStep> GetShowAllTasksStep(
      IShowAllTasksStep::TaskStorageRef) override;
  std::shared_ptr<IStep> GetShowNTasksStep(
      IShowNTasksStep::TaskWrappers) override;
  std::shared_ptr<IStep> GetShowSortedTasksStep(
      IShowSortedTasksStep::Tasks) override;

  std::shared_ptr<IStep> GetShowHelpStep() override;

  std::shared_ptr<IStep> GetReportMessageStep(MessageEnum) override;
  std::shared_ptr<IStep> GetReportMessageStep(MessageEnum, TaskId) override;

 private:
  std::shared_ptr<IostreamAddTaskStep> iostream_add_task_step_;
  std::shared_ptr<IostreamEditTaskStep> iostream_edit_task_step_;
  std::shared_ptr<IostreamCompleteTaskStep> iostream_complete_task_step_;
  std::shared_ptr<IostreamDeleteTaskStep> iostream_delete_task_step_;
  std::shared_ptr<IostreamShowAllTasksStep> iostream_show_all_tasks_step_;
  std::shared_ptr<IostreamShowNTasksStep> iostream_show_n_tasks_step_;
  std::shared_ptr<IostreamShowSortedTasksStep> iostream_show_sorted_tasks_step_;
  std::shared_ptr<IostreamShowHelpStep> iostream_show_help_step_;
  std::shared_ptr<IostreamReportMessageStep> iostream_report_message_step_;

  std::shared_ptr<ISmallStepFactory> state_factory_;
  std::shared_ptr<IValidator> validator_;
  std::shared_ptr<IIoFacility> io_facility_;
};

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_
