#include "DefaultStepFactory.h"

#define GENERATE_GETTER_FOR_POINTER(cls, name, args) \
  if (name == nullptr) {                             \
    name = std::make_shared<cls> args;               \
  }

namespace task_manager {
DefaultStepFactory::DefaultStepFactory(
    const std::shared_ptr<IIoFacility> &io_facility,
    const std::shared_ptr<IValidator> &validator,
    const std::shared_ptr<ISmallStepFactory> &state_factory)
    : state_factory_(state_factory),
      validator_(validator),
      io_facility_(io_facility) {}

std::shared_ptr<IStep> DefaultStepFactory::GetAddTaskStep() {
  GENERATE_GETTER_FOR_POINTER(DefaultAddTaskStep, default_add_task_step_,
                              (io_facility_, state_factory_, validator_))
  return default_add_task_step_;
}
std::shared_ptr<IStep> DefaultStepFactory::GetAddTaskStep(
    SolidTask solid_task) {
  GENERATE_GETTER_FOR_POINTER(DefaultAddTaskStep, default_add_task_step_,
                              (io_facility_, state_factory_, validator_))
  default_add_task_step_->SetSolidTask(solid_task);
  return default_add_task_step_;
}
std::shared_ptr<IStep> DefaultStepFactory::GetEditTaskStep(
    SolidTask solid_task) {
  GENERATE_GETTER_FOR_POINTER(DefaultEditTaskStep, default_edit_task_step_,
                              (io_facility_, state_factory_, validator_))
  default_edit_task_step_->SetSolidTask(std::move(solid_task));
  return default_edit_task_step_;
}
std::shared_ptr<IStep> DefaultStepFactory::GetCompleteTaskStep(
    SolidTasks solid_tasks) {
  GENERATE_GETTER_FOR_POINTER(DefaultCompleteTaskStep,
                              default_complete_task_step_,
                              (io_facility_, validator_))
  default_complete_task_step_->SetSolidTasks(std::move(solid_tasks));
  return default_complete_task_step_;
}
std::shared_ptr<IStep> DefaultStepFactory::GetDeleteTaskStep(
    SolidTasks solid_tasks) {
  GENERATE_GETTER_FOR_POINTER(DefaultDeleteTaskStep, default_delete_task_step_,
                              (io_facility_, validator_))
  default_delete_task_step_->SetSolidTasks(std::move(solid_tasks));
  return default_delete_task_step_;
}
std::shared_ptr<IStep> DefaultStepFactory::GetShowStep(SolidTasks solid_tasks) {
  GENERATE_GETTER_FOR_POINTER(DefaultShowStep, default_show_step_,
                              (io_facility_))
  default_show_step_->SetSolidTasks(std::move(solid_tasks));
  return default_show_step_;
}

std::shared_ptr<IStep> DefaultStepFactory::GetShowHelpStep() {
  GENERATE_GETTER_FOR_POINTER(DefaultShowHelpStep, default_show_help_step_,
                              (io_facility_))
  return default_show_help_step_;
}

std::shared_ptr<IStep> DefaultStepFactory::GetReportMessageStep(
    std::string arg) {
  GENERATE_GETTER_FOR_POINTER(DefaultReportMessageStep,
                              default_report_message_step_, (io_facility_))
  default_report_message_step_->SetMessage(std::move(arg));
  return default_report_message_step_;
}
}  // namespace task_manager
