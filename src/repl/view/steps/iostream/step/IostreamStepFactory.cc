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
std::shared_ptr<IStep> IostreamStepFactory::GetAddTaskStep(
    SolidTask solid_task) {
  GENERATE_GETTER_FOR_POINTER(IostreamAddTaskStep, iostream_add_task_step_,
                              (io_facility_, state_factory_, validator_))
  iostream_add_task_step_->SetSolidTask(solid_task);
  return iostream_add_task_step_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetEditTaskStep(
    SolidTask solid_task) {
  GENERATE_GETTER_FOR_POINTER(IostreamEditTaskStep, iostream_edit_task_step_,
                              (io_facility_, state_factory_, validator_))
  iostream_edit_task_step_->SetSolidTask(std::move(solid_task));
  return iostream_edit_task_step_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetCompleteTaskStep(
    SolidTasks solid_tasks) {
  GENERATE_GETTER_FOR_POINTER(IostreamCompleteTaskStep,
                              iostream_complete_task_step_,
                              (io_facility_, validator_))
  iostream_complete_task_step_->SetSolidTasks(std::move(solid_tasks));
  return iostream_complete_task_step_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetDeleteTaskStep(
    SolidTasks solid_tasks) {
  GENERATE_GETTER_FOR_POINTER(IostreamDeleteTaskStep,
                              iostream_delete_task_step_,
                              (io_facility_, validator_))
  iostream_delete_task_step_->SetSolidTasks(std::move(solid_tasks));
  return iostream_delete_task_step_;
}
std::shared_ptr<IStep> IostreamStepFactory::GetShowStep(
    SolidTasks solid_tasks) {
  GENERATE_GETTER_FOR_POINTER(IostreamShowStep, iostream_show_step_,
                              (io_facility_))
  iostream_show_step_->SetSolidTasks(std::move(solid_tasks));
  return iostream_show_step_;
}

std::shared_ptr<IStep> IostreamStepFactory::GetShowHelpStep() {
  GENERATE_GETTER_FOR_POINTER(IostreamShowHelpStep, iostream_show_help_step_,
                              (io_facility_))
  return iostream_show_help_step_;
}

std::shared_ptr<IStep> IostreamStepFactory::GetReportMessageStep(
    std::string arg) {
  GENERATE_GETTER_FOR_POINTER(IostreamReportMessageStep,
                              iostream_report_message_step_, (io_facility_))
  iostream_report_message_step_->SetMessage(std::move(arg));
  return iostream_report_message_step_;
}