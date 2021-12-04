#include "IostreamStepFactory.h"

#define GENERATE_GETTER_FOR_POINTER(cls, name, args) \
  if (name == nullptr) {                             \
    name = std::make_shared<cls> args;               \
  }

std::shared_ptr<IStep> IostreamStepFactory::GetAddTaskREPLState() {
  GENERATE_GETTER_FOR_POINTER(IostreamAddTaskREPLState,
                              iostream_add_task_repl_state_,
                              (state_factory_, validator_))
  return iostream_add_task_repl_state_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetAddTaskREPLState(Task task) {
  GENERATE_GETTER_FOR_POINTER(IostreamAddTaskREPLState,
                              iostream_add_task_repl_state_,
                              (state_factory_, validator_))
  iostream_add_task_repl_state_->SetTask(task);
  return iostream_add_task_repl_state_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetEditTaskREPLState(
    IEditTaskREPLState::TaskWrapperRef task_wrapper) {
  GENERATE_GETTER_FOR_POINTER(IostreamEditTaskREPLState,
                              iostream_edit_task_repl_state_,
                              (task_wrapper, state_factory_, validator_));
  iostream_edit_task_repl_state_->SetTaskWrapper(
      task_wrapper);  // TODO: Do not repeat here
  return iostream_edit_task_repl_state_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetCompleteTaskREPLState(
    ICompleteTaskREPLState::TaskWrappers task_wrappers) {
  GENERATE_GETTER_FOR_POINTER(IostreamCompleteTaskREPLState,
                              iostream_complete_task_repl_state_,
                              (task_wrappers, validator_));
  iostream_complete_task_repl_state_->SetTaskWrappers(std::move(task_wrappers));
  return iostream_complete_task_repl_state_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetDeleteTaskREPLState(
    IDeleteTaskREPLState::TaskWrappers task_wrappers) {
  GENERATE_GETTER_FOR_POINTER(IostreamDeleteTaskREPLState,
                              iostream_delete_task_repl_state_,
                              (task_wrappers, validator_));
  iostream_delete_task_repl_state_->SetTaskWrappers(std::move(task_wrappers));
  return iostream_delete_task_repl_state_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetShowAllTasksREPLState(
    IShowAllTasksREPLState::TaskStorageRef task_storage) {
  GENERATE_GETTER_FOR_POINTER(IostreamShowAllTasksREPLState,
                              iostream_show_all_tasks_repl_state_,
                              (task_storage));
  iostream_show_all_tasks_repl_state_->SetTaskStorage(task_storage);
  return iostream_show_all_tasks_repl_state_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetShowNTasksREPLState(
    IShowNTasksREPLState::TaskWrappers task_wrappers) {
  GENERATE_GETTER_FOR_POINTER(IostreamShowNTasksREPLState,
                              iostream_show_n_tasks_repl_state_,
                              (task_wrappers))
  iostream_show_n_tasks_repl_state_->SetTaskWrappers(std::move(task_wrappers));
  return iostream_show_n_tasks_repl_state_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetShowSortedTasksREPLState(
    IShowSortedTasksREPLState::Tasks tasks) {
  GENERATE_GETTER_FOR_POINTER(IostreamShowSortedTasksREPLState,
                              iostream_show_sorted_tasks_repl_state_, (tasks));
  iostream_show_sorted_tasks_repl_state_->SetTasks(std::move(tasks));
  return iostream_show_sorted_tasks_repl_state_;
}
IostreamStepFactory::IostreamStepFactory(
    const std::shared_ptr<ISmallStepFactory> &state_factory,
    const std::shared_ptr<IValidator> &validator)
    : state_factory_(state_factory), validator_(validator) {}
