#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_

#include "IostreamStep.h"
#include "repl/validator/IValidator.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/IStepFactory.h"

class IostreamStepFactory final : public IStepFactory {
 public:
  IostreamStepFactory(const std::shared_ptr<ISmallStepFactory> &state_factory,
                      const std::shared_ptr<IValidator> &validator);

  std::shared_ptr<IStep> GetAddTaskREPLState() override;
  std::shared_ptr<IStep> GetAddTaskREPLState(Task) override;

  std::shared_ptr<IStep> GetEditTaskREPLState(
      IEditTaskStep::TaskWrapperRef) override;
  std::shared_ptr<IStep> GetCompleteTaskREPLState(
      ICompleteTaskStep::TaskWrappers) override;
  std::shared_ptr<IStep> GetDeleteTaskREPLState(
      IDeleteTaskStep::TaskWrappers) override;

  std::shared_ptr<IStep> GetShowAllTasksREPLState(
      IShowAllTasksStep::TaskStorageRef) override;
  std::shared_ptr<IStep> GetShowNTasksREPLState(
      IShowNTasksStep::TaskWrappers) override;
  std::shared_ptr<IStep> GetShowSortedTasksREPLState(
      IShowSortedTasksStep::Tasks) override;

 private:
  std::shared_ptr<IostreamAddTaskStep> iostream_add_task_repl_state_;
  std::shared_ptr<IostreamEditTaskStep> iostream_edit_task_repl_state_;
  std::shared_ptr<IostreamCompleteTaskStep>
      iostream_complete_task_repl_state_;
  std::shared_ptr<IostreamDeleteTaskStep> iostream_delete_task_repl_state_;
  std::shared_ptr<IostreamShowAllTasksStep>
      iostream_show_all_tasks_repl_state_;
  std::shared_ptr<IostreamShowNTasksStep>
      iostream_show_n_tasks_repl_state_;
  std::shared_ptr<IostreamShowSortedTasksStep>
      iostream_show_sorted_tasks_repl_state_;

  std::shared_ptr<ISmallStepFactory> state_factory_;
  std::shared_ptr<IValidator> validator_;
};

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_
