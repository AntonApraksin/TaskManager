#include "IostreamStepFactory.h"

#define GENERATE_GETTER_FOR_POINTER(cls, name, args) \
  if (name == nullptr) {                             \
    name = std::make_shared<cls> args;               \
  }

IostreamStepFactory::IostreamStepFactory(
    const std::shared_ptr<IIoFacility> &io_facility,
    const std::shared_ptr<IValidator> &validator,
    const std::shared_ptr<ISmallStepFactory> &state_factory)
    : state_factory_(state_factory),
      validator_(validator),
      io_facility_(io_facility) {}

std::shared_ptr<IStep> IostreamStepFactory::GetAddTaskStep() {
  GENERATE_GETTER_FOR_POINTER(IostreamAddTaskStep, iostream_add_task_step_,
                              (io_facility_, state_factory_, validator_))
  return iostream_add_task_step_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetAddTaskStep(Task task) {
  GENERATE_GETTER_FOR_POINTER(IostreamAddTaskStep, iostream_add_task_step_,
                              (io_facility_, state_factory_, validator_))
  iostream_add_task_step_->SetTask(task);
  return iostream_add_task_step_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetEditTaskStep(
    IEditTaskStep::TaskWrapperRef task_wrapper) {
  GENERATE_GETTER_FOR_POINTER(
      IostreamEditTaskStep, iostream_edit_task_step_,
      (io_facility_, state_factory_, validator_, task_wrapper));
  iostream_edit_task_step_->SetTaskWrapper(
      task_wrapper);  // TODO: Do not repeat here
  return iostream_edit_task_step_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetCompleteTaskStep(
    ICompleteTaskStep::TaskWrappers task_wrappers) {
  GENERATE_GETTER_FOR_POINTER(IostreamCompleteTaskStep,
                              iostream_complete_task_step_,
                              (io_facility_, validator_, task_wrappers));
  iostream_complete_task_step_->SetTaskWrappers(std::move(task_wrappers));
  return iostream_complete_task_step_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetDeleteTaskStep(
    IDeleteTaskStep::TaskWrappers task_wrappers) {
  GENERATE_GETTER_FOR_POINTER(IostreamDeleteTaskStep,
                              iostream_delete_task_step_,
                              (io_facility_, validator_, task_wrappers));
  iostream_delete_task_step_->SetTaskWrappers(std::move(task_wrappers));
  return iostream_delete_task_step_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetShowAllTasksStep(
    IShowAllTasksStep::TaskStorageRef task_storage) {
  GENERATE_GETTER_FOR_POINTER(IostreamShowAllTasksStep,
                              iostream_show_all_tasks_step_,
                              (io_facility_, task_storage));
  iostream_show_all_tasks_step_->SetTaskStorage(task_storage);
  return iostream_show_all_tasks_step_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetShowNTasksStep(
    IShowNTasksStep::TaskWrappers task_wrappers) {
  GENERATE_GETTER_FOR_POINTER(IostreamShowNTasksStep,
                              iostream_show_n_tasks_step_,
                              (io_facility_, task_wrappers))
  iostream_show_n_tasks_step_->SetTaskWrappers(std::move(task_wrappers));
  return iostream_show_n_tasks_step_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetShowSortedTasksStep(
    IShowSortedTasksStep::Tasks tasks) {
  GENERATE_GETTER_FOR_POINTER(IostreamShowSortedTasksStep,
                              iostream_show_sorted_tasks_step_,
                              (io_facility_, tasks));
  iostream_show_sorted_tasks_step_->SetTasks(std::move(tasks));
  return iostream_show_sorted_tasks_step_;
}

std::shared_ptr<IStep> IostreamStepFactory::GetShowHelpStep() {
  GENERATE_GETTER_FOR_POINTER(IostreamShowHelpStep, iostream_show_help_step_,
                              (io_facility_));
  return iostream_show_help_step_;
}

std::shared_ptr<IStep> IostreamStepFactory::GetReportMessageStep(
    MessageEnum message_enum) {
  GENERATE_GETTER_FOR_POINTER(IostreamReportMessageStep,
                              iostream_report_message_step_, (io_facility_));
  iostream_report_message_step_->SetMessage(message_enum);
  return iostream_report_message_step_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetReportMessageStep(
    MessageEnum message_enum, TaskId task_id) {
  GENERATE_GETTER_FOR_POINTER(IostreamReportMessageStep,
                              iostream_report_message_step_, (io_facility_));
  iostream_report_message_step_->SetMessage(message_enum);
  iostream_report_message_step_->SetTaskId(task_id);
  return iostream_report_message_step_;
}