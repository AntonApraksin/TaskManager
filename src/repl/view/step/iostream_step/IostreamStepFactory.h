#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_

#include "repl/view/step/IStepFactory.h"

#include "repl/view/step/iostream_step/IostreamStep.h"
#include "repl/state_factory/IStateFactory.h" // TODO: move StateFactory to a better place
#include "repl/io_facility/IValidator.h" // TODO: move IValidator to a better place

class IostreamStepFactory final : public IStepFactory
{
 public:
  IostreamStepFactory(const std::shared_ptr<IStateFactory> &state_factory,
                      const std::shared_ptr<IValidator> &validator);

  std::shared_ptr<IStep> GetAddTaskREPLState() override;
  std::shared_ptr<IStep> GetAddTaskREPLState(Task) override;

  std::shared_ptr<IStep> GetEditTaskREPLState(IEditTaskREPLState::TaskWrapperRef) override;
  std::shared_ptr<IStep> GetCompleteTaskREPLState(ICompleteTaskREPLState::TaskWrappers) override;
  std::shared_ptr<IStep> GetDeleteTaskREPLState(IDeleteTaskREPLState::TaskWrappers) override;

  std::shared_ptr<IStep> GetShowAllTasksREPLState(IShowAllTasksREPLState::TaskStorageRef) override;
  std::shared_ptr<IStep> GetShowNTasksREPLState(IShowNTasksREPLState::TaskWrappers) override;
  std::shared_ptr<IStep> GetShowSortedTasksREPLState(IShowSortedTasksREPLState::Tasks) override;

 private:
  std::shared_ptr<IostreamAddTaskREPLState> iostream_add_task_repl_state_;
  std::shared_ptr<IostreamEditTaskREPLState> iostream_edit_task_repl_state_;
  std::shared_ptr<IostreamCompleteTaskREPLState> iostream_complete_task_repl_state_;
  std::shared_ptr<IostreamDeleteTaskREPLState> iostream_delete_task_repl_state_;
  std::shared_ptr<IostreamShowAllTasksREPLState> iostream_show_all_tasks_repl_state_;
  std::shared_ptr<IostreamShowNTasksREPLState> iostream_show_n_tasks_repl_state_;
  std::shared_ptr<IostreamShowSortedTasksREPLState> iostream_show_sorted_tasks_repl_state_;

  std::shared_ptr<IStateFactory> state_factory_;
  std::shared_ptr<IValidator> validator_;
};

#endif //TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEPFACTORY_H_
